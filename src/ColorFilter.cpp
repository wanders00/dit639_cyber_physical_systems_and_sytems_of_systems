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
    
    cv::Mat yellowImg;
    cv::Mat blueImg;
    cv::Mat filteredYellow;
    cv::Mat filteredBlue;

    // 
    cv::cvtColor(image, yellowImg, cv::COLOR_BGR2HSV);
    blueImg = yellowImg.clone();

    // Threshold the HSV image, keep only the yellow pixels
    cv::inRange(yellowImg, cv::Scalar(lowHYellow, lowSYellow, lowVYellow),
              cv::Scalar(highHYellow, highSYellow, highVYellow),
              filteredYellow);

    // Threshold the HSV image, keep only the blue pixels
    cv::inRange(blueImg, cv::Scalar(lowHBlue, lowSBlue, lowVBlue),
              cv::Scalar(highHBlue, highSBlue, highVBlue),
              filteredBlue);

    std::pair<cv::Mat, cv::Mat> imagePair = std::make_pair(filteredYellow, filteredBlue);

    return imagePair;
}