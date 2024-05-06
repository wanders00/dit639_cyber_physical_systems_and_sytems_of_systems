#ifndef ANGLECALCULATOR_HPP
#define ANGLECALCULATOR_HPP

#include "cluon-complete.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class AngleCalculator
{
    public:
    
    float calculateTurnAngle(std::vector<cv::Point2f> yellowCenters, std::vector<cv::Point2f> blueCenters);

    float calculateAngleBetweenCenters(std::vector<cv::Point2f> massCenters);

    float distanceBetweenTwoMassCenters(
        float xInPoint1,
        float yInPoint1,
        float xInPoint2,
        float yInPoint2
    );
    
    float calculateSteeringAngle(float turnAngle);
    
    std::vector<cv::Point2f> sortMassCenters(std::vector<cv::Point2f> massCenters);
};

#endif