/*
 * Based on template by Christian Berger:
 * https://github.com/chalmers-revere/working-with-rec-files/blob/master/cpp-opencv/src/template-opencv.cpp
 */

// Include the single-file, header-only middleware libcluon to create high-performance microservices
#include "cluon-complete.hpp"
// Include the OpenDLV Standard Message Set that contains messages that are usually exchanged for automotive or robotic applications
#include "opendlv-standard-message-set.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ColorFilter.hpp"

int32_t main(int32_t argc, char **argv) {
    int32_t retCode{1};
    ColorFilter colorFilter;

    // Parse the command line parameters as we require the user to specify some mandatory information on startup.
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ((0 == commandlineArguments.count("cid")) ||
        (0 == commandlineArguments.count("name")) ||
        (0 == commandlineArguments.count("width")) ||
        (0 == commandlineArguments.count("height"))) {
        std::cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << std::endl;
        std::cerr << "         --cid:    CID of the OD4Session to send and receive messages" << std::endl;
        std::cerr << "         --name:   name of the shared memory area to attach" << std::endl;
        std::cerr << "         --width:  width of the frame" << std::endl;
        std::cerr << "         --height: height of the frame" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=253 --name=img --width=640 --height=480 --verbose" << std::endl;
    } else {
        // Extract the values from the command line parameters
        const std::string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};

        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
        if (sharedMemory && sharedMemory->valid()) {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

            // Interface to a running OpenDaVINCI session where network messages are exchanged.
            // The instance od4 allows you to send and receive messages.
            cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};

            opendlv::proxy::GroundSteeringRequest gsr;
            std::mutex gsrMutex;
            auto onGroundSteeringRequest = [&gsr, &gsrMutex](cluon::data::Envelope &&env) {
                // The envelope data structure provide further details, such as sampleTimePoint as shown in this test case:
                // https://github.com/chrberger/libcluon/blob/master/libcluon/testsuites/TestEnvelopeConverter.cpp#L31-L40
                std::lock_guard<std::mutex> lck(gsrMutex);
                gsr = cluon::extractMessage<opendlv::proxy::GroundSteeringRequest>(std::move(env));
                //std::cout << "lambda: groundSteering = " << gsr.groundSteering() << std::endl;
            };

            od4.dataTrigger(opendlv::proxy::GroundSteeringRequest::ID(), onGroundSteeringRequest);

            std::ofstream out;
            if (commandlineArguments.count("output") != 0) {
                out.open(commandlineArguments["output"]);
            }

            int64_t previousFrameTime = 0;

            // Endless loop; end the program by pressing Ctrl-C.
            while (od4.isRunning()) {
                // OpenCV data structure to hold an image.
                cv::Mat img;

                // Wait for a notification of a new frame.
                sharedMemory->wait();

                // Lock the shared memory.
                sharedMemory->lock();
                {
                    // Copy the pixels from the shared memory into our own data structure.
                    cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
                    img = wrapped.clone();
                }

                cluon::data::TimeStamp frameTimeStamp = sharedMemory->getTimeStamp().second;

                int64_t frameTime = cluon::time::toMicroseconds(frameTimeStamp);

                sharedMemory->unlock();

                // If you want to access the latest received ground steering, don't forget to lock the mutex:
                {
                    std::lock_guard<std::mutex> lck(gsrMutex);
                    //std::cout << "main: groundSteering = " << gsr.groundSteering() << std::endl;
                    std::string output =
                        "group_13;" +
                        std::to_string(frameTime) +
                        ";" +
                        "0" + // '0' temporary value for ground steering
                        ";" + std::to_string(gsr.groundSteering()) +
                        ";" +
                        "\n";

                    // Only print if the timestamp is not the same as the previous one
                    if (frameTime != previousFrameTime) {
                        std::cout << output;
                        if (out.is_open()) {
                            out << output;
                        }
                    }

                    // Update the previous timestamp
                    previousFrameTime = frameTime;
                }

                cv::Mat original = img.clone();
                std::pair<cv::Mat, cv::Mat> filteredImage =
                    colorFilter.colorFilter(original);
                
                cv::Mat combinedImage;
                cv::bitwise_or(filteredImage.first, filteredImage.second, combinedImage);

                // Display image on your screen.
                if (VERBOSE) {
                    // cv::imshow("Filtered Yellow Image", filteredImage.first);
                    // cv::imshow("Filtered Blue Image", filteredImage.second);
                    cv::imshow("Combined Image", combinedImage);
                    cv::imshow(sharedMemory->name().c_str(), img);
                    cv::waitKey(1);
                }
            }

            if (out.is_open()) {
                out.close();
            }
        }
        retCode = 0;
    }
    return retCode;
}
