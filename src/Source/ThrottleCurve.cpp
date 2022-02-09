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
const juce::Array<juce::Point<int>>& ThrottleCurve::getPoints() const
{
    return curve;
}

/**
 * @brief Adds a new point to the curve
 *
 * @details 
 */
void ThrottleCurve::addPoint(ThrottleCurve::Point& point)
{
    curve.add(point);
    sortCurve(curve);
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
    curveToReset.add(Point(inputMax / 2, outputMax / 2));
    //curveToReset.add(Point(inputMax, outputMax));
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
