#include "ImageAngleCalculator.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cluon-complete.hpp"

// constant position of the car on the video
#define CAR_X 320
#define CAR_Y 480

// how close to the turn do we start calculating the angle
#define TURN_DISTANCE 200

// define PI
#define PI 3.14159265

// sorting the vector of massCenters based on Y to get the closest ones to the car.
std::vector<cv::Point2f> AngleCalculator::sortMassCenters(std::vector<cv::Point2f> massCenters) {
    std::sort(massCenters.begin(), massCenters.end(), [](cv::Point2f a, cv::Point2f b) {
        return a.y > b.y;
    });

    return massCenters;
}

/**
 * Calculate the distance between two mass centers
 * @param xInPoint1 the x coordinate of the first mass center
 * @param yInPoint1 the y coordinate of the first mass center
 * @param xInPoint2 the x coordinate of the second mass center
 * @param yInPoint2 the y coordinate of the second mass center
 * @return the distance between the two mass centers
 */
float ImageAngleCalculator::distanceBetweenTwoMassCenters(
    float xInPoint1,
    float yInPoint1,
    float xInPoint2,
    float yInPoint2) {
    float distance = std::sqrt(std::pow(xInPoint2 - xInPoint1, 2) +
                               std::pow(yInPoint2 - yInPoint1, 2) * 1.0);
    return distance;
}

/**
 * Calculate the angle between the two closest cones
 * @param massCenters the centers of the cones
 * @return the angle between the two closest cones
 */
float ImageAngleCalculator::calculateAngleBetweenCenters(
    std::vector<cv::Point2f> massCenters) {
    // sort the massCenters based on Y to get the closest ones to the car.
    massCenters = sortMassCenters(massCenters);

    // get the first two massCenters
    cv::Point2f massCenter1 = massCenters[0];
    cv::Point2f massCenter2 = massCenters[1];

    // calculate the distance between the closest massCenter and the car
    float distanceToClosest = distanceBetweenTwoMassCenters(CAR_X, CAR_Y, massCenter1.x, massCenter1.y);

    // if the closest cone is less then thershold, calculate the angle
    if (distanceToClosest < TURN_DISTANCE) {
        // calculate the angle between the two cones IN DEGREES
        float angle = std::atan2(massCenter1.y - massCenter2.y, massCenter1.x - massCenter2.x) * 180 / PI;

        // convert to the value measured from the right x axis
        angle = 180 - angle;

        // ignore the small turns
        if (angle > 60 && angle < 120) {
            return 90;
        }

        return angle;
    }

    // return no turn
    return 90;
}

/**
 * Calculate the turn angle based on the centers of the cones
 * @param yellowCenters the centers of the yellow cones
 * @param blueCenters the centers of the blue cones
 * @return the turn angle
 */
float ImageAngleCalculator::calculateTurnAngle(std::vector<cv::Point2f> yellowCenters, std::vector<cv::Point2f> blueCenters) {
    // if there are at least two centers of each color
    if (yellowCenters.size() >= 2 && blueCenters.size() >= 2) {
        // calculate the angle between the two closest cones
        float yellowAngle = calculateAngleBetweenCenters(yellowCenters);
        float blueAngle = calculateAngleBetweenCenters(blueCenters);

        // take the average of the two angles
        float turnAngle = (yellowAngle + blueAngle) / 2;

        return turnAngle;
    } else if (yellowCenters.size() >= 2) {  // if there is only data about the yellow centers
        // calculate the angle between the two closest cones
        float yellowAngle = calculateAngleBetweenCenters(yellowCenters);

        return yellowAngle;
    } else if (blueCenters.size() >= 2) {  // if there is only data about the blue centers
        // calculate the angle between the two closest cones
        float blueAngle = calculateAngleBetweenCenters(blueCenters);

        return blueAngle;
    } else {
        return 90;
    }
}

/**
 * Calculate the steering angle based on the turn angle
 * @param turnAngle the angle of the turn
 * @return the steering angle
 */
float ImageAngleCalculator::calculateSteeringAngle(float turnAngle) {
    float steeringAngle = 0;

    // If turnAngle indicates a turn (not 90 deg) map to -0.3 to -0.3
    if (turnAngle <= 89 || turnAngle >= 91) {
        steeringAngle = (turnAngle * 0.003333) - 0.3;
    }

    return steeringAngle;
}

// https://stackoverflow.com/questions/18253065/is-there-any-function-to-sort-a-stdvector-cvpoint2f-vector