#include "AngularVelocityAngleCalculator.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cluon-complete.hpp"

/**
 * Calculate the steering angle based on the angular velocity.
 * @param angularVelocity The Angular Velocity
 * @return The Steering Angle
 */
float AngularVelocityAngleCalculator::calculateSteeringAngle(float angularVelocity) {
    float steeringAngle = 0;

    // If angularVelocity indicates a turn (not 0 deg/s) map to -0.3 to -0.3
    if (angularVelocity != 0) {
        steeringAngle = (angularVelocity / 100.0f) * 0.3f;
    }

    // Cap the steering angle to -0.24 to 0.24
    if (steeringAngle > 0.24f) {
        steeringAngle = 0.24f;
    } else if (steeringAngle < -0.24f) {
        steeringAngle = -0.24f;
    }

    return steeringAngle;
}