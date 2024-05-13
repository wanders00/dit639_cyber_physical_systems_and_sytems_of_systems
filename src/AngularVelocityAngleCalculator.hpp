#ifndef ANG_VEL_ANGLE_CALCULATOR_HPP
#define ANG_VEL_ANGLE_CALCULATOR_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cluon-complete.hpp"

class AngularVelocityAngleCalculator {
   public:
    float calculateSteeringAngle(float angularVelocity);
};

#endif