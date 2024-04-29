#ifndef CONTOURDETECTOR_HPP
#define CONTOURDETECTOR_HPP

#include "cluon-complete.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ContourDetector{
    public: 
    //std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> 
    void
    findContours(std::pair<cv::Mat, cv::Mat> filteredImage, cv::Mat originalImage);
};

#endif // CONTOURDETECTOR_HPP