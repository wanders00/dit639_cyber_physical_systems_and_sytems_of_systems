#include "ColorFilter.hpp"

#include "cluon-complete.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//todo add namespace uwu

std::pair<cv::Mat, cv::Mat> ColorFilter::colorFilter(cv::Mat image) {
    
    //HSV values for yellow filtering 
    int lowHYellow = 9;
    int highHYellow = 54;

    int lowSYellow = 20;
    int highSYellow = 196;

    int lowVYellow = 135;
    int highVYellow = 238;

    //HSV values for blue filtering
    int lowHBlue = 109;
    int highHBlue = 135;

    int lowSBlue = 68;
    int highSBlue = 250;

    int lowVBlue = 42;
    int highVBlue = 120;

    int ReflectionLowHBlue = 0;
    int ReflectionHighHBlue = 179;

    int ReflectionLowSBlue = 50;
    int ReflectionHighSBlue = 80;

    int ReflectionLowVBlue = 0;
    int ReflectionHighVBlue = 255;
    
    cv::Mat yellowImg;
    cv::Mat blueImg;
    cv::Mat filteredYellow;
    cv::Mat filteredBlue;
    cv::Mat filteredReflection;
    cv::Mat blueReflectionImg;
    cv::Mat finalBlueImg;
    cv::Mat blueImgAnd;

    // 
    cv::cvtColor(image, yellowImg, cv::COLOR_BGR2HSV);
    blueImg = yellowImg.clone();
    blueReflectionImg = yellowImg.clone();

    // Threshold the HSV image, keep only the yellow pixels
    cv::inRange(yellowImg, cv::Scalar(lowHYellow, lowSYellow, lowVYellow),
              cv::Scalar(highHYellow, highSYellow, highVYellow),
              filteredYellow);

    // Threshold the HSV image, keep only the blue pixels
    cv::inRange(blueImg, cv::Scalar(lowHBlue, lowSBlue, lowVBlue),
              cv::Scalar(highHBlue, highSBlue, highVBlue),
              filteredBlue);

    // Detect the large reflections
    // Colour process to detect large reflections
    cv::inRange(
        blueReflectionImg,
        cv::Scalar(ReflectionLowHBlue, ReflectionLowSBlue, ReflectionLowVBlue),
        cv::Scalar(ReflectionHighHBlue, ReflectionHighSBlue,
                 ReflectionHighVBlue), filteredReflection);

    // Use bitwise AND operator to find reflections in blue cone image
    cv::bitwise_and(filteredBlue, filteredReflection, blueImgAnd);

    // Use bitwise XOR operator to remove found reflections from blue cone image
    cv::bitwise_xor(blueImgAnd, filteredBlue, finalBlueImg);

    std::pair<cv::Mat, cv::Mat> imagePair = std::make_pair(filteredYellow, finalBlueImg);
    

    // Filter image to fill gaps and remove small objects
    std::pair<cv::Mat, cv::Mat> filteredImage;

    //Yellow cones fill holes in objects 
    cv::dilate(imagePair.first, filteredImage.first,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));
    cv::erode(filteredImage.first, filteredImage.first,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));

    // Remove small objects for yellow cones
    cv::erode(filteredImage.first, filteredImage.first,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::dilate(filteredImage.first, filteredImage.first,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));

    // fill holes in objects - blue cones
    cv::dilate(imagePair.second, filteredImage.second,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));
    cv::erode(filteredImage.second, filteredImage.second,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));

    // remove small objects - blue cones
    cv::erode(filteredImage.second, filteredImage.second,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::dilate(filteredImage.second, filteredImage.second,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));

    return filteredImage;
}