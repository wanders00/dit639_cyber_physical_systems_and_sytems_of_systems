#include "ContourDetector.hpp"

#include "cluon-complete.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/*Function to find contours of an image
* @param filteredImage: pair of blue and yellow images to find contours
* @param originalImage: original image to draw contours on
* @return pair of vectors of points
    
*/
//std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> 
void
ContourDetector::findContours(std::pair<cv::Mat, cv::Mat> filteredImage, cv::Mat originalImage) {
    const float CLUSTER_RANGE = 100.0;

    // Yellow contour calculation
    std::vector<std::vector<cv::Point>> yellowContours;
    cv::findContours(filteredImage.first, yellowContours, cv::RETR_TREE,
                    cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    //Draw yellow contours in original image
    cv::drawContours(originalImage, yellowContours, -1, cv::Scalar(255, 0, 0), 2);

    // Blue contour calculation
    std::vector<std::vector<cv::Point>> blueContours;
    cv::findContours(filteredImage.second, blueContours, cv::RETR_TREE,
                    cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    //Draw blue contours in original image
    cv::drawContours(originalImage, blueContours, -1, cv::Scalar(255, 0, 0), 2);

    // Get moments and  mass centers for YELLOW:
    std::vector<cv::Point2f> yellowMassCenters(yellowContours.size());
    
    for (unsigned int i = 0; i < yellowContours.size(); i++) {
        cv::Moments yellowMoments = cv::moments(yellowContours[i], false);                 // Getting the moments
        if (yellowMoments.m00 != 0) { // Check to avoid division by zero
            yellowMassCenters[i] = cv::Point2f(yellowMoments.m10 / yellowMoments.m00, yellowMoments.m01 / yellowMoments.m00);   // Mass centers
            yellowMassCenters[i].y = yellowMassCenters[i].y + 240;
        }
    }
    
    // OPTION: Draw targets on the YELLOW mass centers

    // Get moments and  mass centers for BLUE:
    std::vector<cv::Point2f> blueMassCenters(blueContours.size());
    
    for (unsigned int i = 0; i < blueContours.size(); i++) {
        cv::Moments blueMoments = cv::moments(blueContours[i], false);  // Getting the moments
        if (blueMoments.m00 != 0) {     // Check to avoid division by zero
            blueMassCenters[i] = cv::Point2f(blueMoments.m10 / blueMoments.m00, blueMoments.m01 / blueMoments.m00);   // Mass centers
            blueMassCenters[i].y = blueMassCenters[i].y + 240;
        }
    }
    
    // OPTION: Draw targets on the BLUE mass centers    
        
    cv::imshow("Original", originalImage); //show contours in the original image

}