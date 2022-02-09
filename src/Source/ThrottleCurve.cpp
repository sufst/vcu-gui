/******************************************************************************
 * @file   ThrottleCurve.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Model of a throttle curve
 *****************************************************************************/

#include "ThrottleCurve.h"

#include <functional>

//==================================================== Constructor / destructor

/**
 * @brief   Constructor
 *
 * @details Uses the default interpolation method
 */
ThrottleCurve::ThrottleCurve()
    :   interpolation(defaultInterpolationMethod)
{
    resetCurveToDefault(curve);
}

/**
 * @brief Constructor specifying interpolation method
 */
ThrottleCurve::ThrottleCurve(InterpolationMethod interpolationMethod)
    :   interpolation(interpolationMethod)
{
    resetCurveToDefault(curve);
}

/**
 * @brief Destructor
 */
ThrottleCurve::~ThrottleCurve()
{
    // nothing to do
}

//================================================================ Curve points

/**
 * @brief Returns a reference to the list of points associated with the curve
 */
juce::Array<juce::Point<int>>& ThrottleCurve::getPoints()
{
    return curve;
}

/**
 * @brief   Adds a new point to the curve
 *
 * @details The points are sorted by x position each time
 */
void ThrottleCurve::addPoint(ThrottleCurve::Point& point)
{
    curve.add(point);
    sortCurve(curve);
}

/**
 * @brief Deletes points within a certain radius of an input point
 *
 * @param[in]   point   Input point
 * @param[in]   radius  Radius around point within which points will be deleted
 */
void ThrottleCurve::deleteNearbyPoints(const Point& point, int radius)
{
    juce::Array<int> toDelete = {};
    
    // work out which points to delete
    for (int i = 0; i < curve.size(); i++)
    {
        if (curve.getReference(i).getDistanceFrom(point) < radius)
        {
            toDelete.add(i);
        }
    }
    
    // delete the points
    int deleteCount = 0;
    for (const auto index : toDelete)
    {
        curve.remove(index - deleteCount);
        deleteCount++;
    }
}

/**
 * @brief       Update the curve when a point has been moved and return a pointer
 *              to the moved point.
 *
 * @details     This sorts the points in the curve, which necessitates returning a
 *              new pointer to the point which has been moved if a further move
 *              of that point is required.s
 *
 * @param[in]   movedPoint  Copy of the point which has been moved
 *
 * @return      Pointer to the point which was moved
 */
ThrottleCurve::Point* ThrottleCurve::pointMoved(const Point movedPoint)
{
    sortCurve(curve);
    int index = curve.indexOf(movedPoint);
    return &curve.getReference(index);
}

//============================================================ Internal utility

/**
 * @brief   Resets the path to its default state
 *
 * @details By default the throttle curve is a linear mapping between input and output
 */
void ThrottleCurve::resetCurveToDefault(juce::Array<ThrottleCurve::Point>& curveToReset)
{
    curveToReset.clear();
    curveToReset.add(Point(0, 0));
    curveToReset.add(Point(inputMax - 1, outputMax - 1));
}

/**
 * @brief Sorts a list of points on a throttle curve
 *
 * @param[in]   curveToSort     The curve to be sorted
 */
void ThrottleCurve::sortCurve(juce::Array<ThrottleCurve::Point>& curveToSort)
{
    std::function compareFunc = [] (ThrottleCurve::Point p1, ThrottleCurve::Point p2) {
        return p1.getX() < p2.getX();
    };
    
    std::sort(curveToSort.begin(), curveToSort.end(), compareFunc);
}

//============================================================== Static utility

/**
 * @brief Returns a list of allowed interpolation methods
 */
const juce::Array<ThrottleCurve::InterpolationMethod>& ThrottleCurve::getAllInterpolationMethods()
{
    return listOfInterpolationMethods;
}

/**
 * @brief Gets the maximum value of the input to the curve
 */
int ThrottleCurve::getInputMax()
{
    return inputMax;
}

/**
 * @brief Gets the maximum value of the output of the curve
 */
int ThrottleCurve::getOutputMax()
{
    return outputMax;
}

/**
 * @brief Returns the name of a particular interpolation method as a string
 */
const juce::String& ThrottleCurve::getInterpolationMethodName(ThrottleCurve::InterpolationMethod method)
{
    return namesOfInterpolationMethods.getReference(static_cast<int>(method));
}

//================================================================= Static data

const juce::Array<ThrottleCurve::InterpolationMethod> ThrottleCurve::listOfInterpolationMethods = {
    ThrottleCurve::InterpolationMethod::Linear, ThrottleCurve::InterpolationMethod::Cosine, ThrottleCurve::InterpolationMethod::Bezier
};

const juce::Array<juce::String> ThrottleCurve::namesOfInterpolationMethods = {
    "Linear", "Cosine", "Bezier"
};
