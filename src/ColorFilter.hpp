#ifndef COLORFILTER_HPP
#define COLORFILTER_HPP

#include "cluon-complete.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ColorFilter{
    public: 
        std::pair<cv::Mat, cv::Mat> colorFilter(cv::Mat image);
};

#endif // COLORFILTER_HPP