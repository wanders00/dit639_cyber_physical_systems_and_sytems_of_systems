#include "ContourDetector.hpp"

#include "cluon-complete.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * Function to find contours of an image
 *
 * @param filteredImage: pair of blue and yellow images to find contours
 * @param originalImage: original image to draw contours on
 *
 * @return pair of vectors of points
 */
std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> 
ContourDetector::findContours(std::pair<cv::Mat, cv::Mat> filteredImage, cv::Mat originalImage) {
    const float CLUSTER_RANGE = 20.0;

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
    
    // LLM: Used GitHub Copilot to debug and fix this loop
    for (auto& contour : yellowContours) {
        cv::Moments yellowMoments = cv::moments(contour, false);                 // Getting the moments
        if (yellowMoments.m00 != 0) { // Check to avoid division by zero
            yellowMassCenters.push_back(cv::Point2f(yellowMoments.m10 / yellowMoments.m00, yellowMoments.m01 / yellowMoments.m00));   // Mass centers
        }
    }
     
    // See if there exists multiple mass centers for Yellow
    if (yellowMassCenters.size() >= 2) {

        // If they do, check if they are close to each other
        for (unsigned int i = 0; i < (yellowMassCenters.size() - 1); i++) {
            float y_value = yellowMassCenters[i].y - yellowMassCenters[i + 1].y;

            // Check if the mass centers are close to each other
            if (y_value < CLUSTER_RANGE) {
                float x_value = yellowMassCenters[i].x - yellowMassCenters[i + 1].x;

                if (x_value < CLUSTER_RANGE) {
                    // Then remove one from the top of the frame to remove cluster
                    yellowMassCenters.erase(yellowMassCenters.begin() + i);
                    i--;
                }
            }
        }
    }
  
    // Draw circles on the YELLOW mass centers
    for (auto& massCenter : yellowMassCenters) {
        cv::circle(originalImage, massCenter, 5, cv::Scalar(0, 255, 0), -1);
    }

    // Get moments and  mass centers for BLUE:
    std::vector<cv::Point2f> blueMassCenters(blueContours.size());
    
    for (auto& contour : blueContours) {
        cv::Moments blueMoments = cv::moments(contour, false);                 // Getting the moments
        if (blueMoments.m00 != 0) { // Check to avoid division by zero
            blueMassCenters.push_back(cv::Point2f(blueMoments.m10 / blueMoments.m00, blueMoments.m01 / blueMoments.m00));   // Mass centers
        }
    }

    // See if there exists multiple mass centers for Blue
    if (blueMassCenters.size() >= 2) {

        // If they do, check if they are close to each other
        for (unsigned int i = 0; i < (blueMassCenters.size() - 1); i++) {
            float y_value = blueMassCenters[i].y - blueMassCenters[i + 1].y;

            // Check if the mass centers are close to each other
            if (y_value < CLUSTER_RANGE) {
                float x_value = blueMassCenters[i].x - blueMassCenters[i + 1].x;

                if (x_value < CLUSTER_RANGE) {
                    // Then remove one from the top of the frame to remove cluster
                    blueMassCenters.erase(blueMassCenters.begin() + i);
                    i--;
                }
            }
        }
    }

    
    
    // Draw circles on the BLUE mass centers
    for (auto& massCenter : blueMassCenters) {
        cv::circle(originalImage, massCenter, 5, cv::Scalar(0, 255, 0), -1);
    }

    // Create a pair of vectors of points
    std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> massCenters = std::make_pair(yellowMassCenters, blueMassCenters);
    return massCenters;
}