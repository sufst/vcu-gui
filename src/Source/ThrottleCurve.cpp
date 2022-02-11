/******************************************************************************
 * @file   ThrottleCurve.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Model of a throttle curve
 *****************************************************************************/

#include "ThrottleCurve.h"

#include <cmath>
#include <functional>
#include <vector>

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
    interpolate();
}

/**
 * @brief Constructor specifying interpolation method
 */
ThrottleCurve::ThrottleCurve(InterpolationMethod interpolationMethod)
    :   interpolation(interpolationMethod)
{
    resetCurveToDefault(curve);
    interpolate();
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
const juce::Array<juce::Point<int>>& ThrottleCurve::getPoints()
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
    curve.addIfNotAlreadyThere(point);
    sortCurve(curve);
    cacheValid = false;
}

/**
 * @brief       Deletes a point from the curve
 *
 * @param[in]   point   Point to delete
 */
void ThrottleCurve::deletePoint(const Point& point)
{
    curve.removeFirstMatchingValue(point);    
    cacheValid = false;
}

/**
 * @brief       Gets the raw pointer to a point to allow it to be moved
 *
 * @param[in]   index   Index of point
 */
ThrottleCurve::Point* ThrottleCurve::getPointForMove(int index)
{
    return &curve.getReference(index);
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
    cacheValid = false;
    return &curve.getReference(index);
}

/**
 * @brief Reset the curve, removing points
 */
void ThrottleCurve::reset()
{
    resetCurveToDefault(curve);
}

//================================================================ Interpolation

/**
 * @brief   Run interpolation
 *
 * @details If the points haven't changed, the cached result is used
 */
void ThrottleCurve::interpolate()
{
    // no need to do anything if cache valid
    if (cacheValid)
    {
        return;
    }
    
    // otherwise need to re-compute
    for (int input = 0; input < inputMax + 1; input++)
    {
        // deadzone
        if (input < curve.getFirst().getX())
        {
            cachedOutputs.at(input) = 0;
            continue;
        }
        
        // normal points
        switch (interpolation)
        {
            case InterpolationMethod::Linear:
                cachedOutputs.at(input) = linearInterpolate(input).getY();
                break;

            case InterpolationMethod::Cosine:
                cachedOutputs.at(input) = cosineInterpolate(input).getY();
                break;
                
            case InterpolationMethod::Cubic:
                cachedOutputs.at(input) = splineInterpolate(input, tk::spline::cspline).getY();
                break;
            
            case InterpolationMethod::Hermite:
                cachedOutputs.at(input) = splineInterpolate(input, tk::spline::cspline_hermite).getY();
                break;

            default:
                // something hasn't been implemented!
                jassertfalse;
                break;
        }
    }
    
    cacheValid = true;
}

/**
 * @brief       Sets the interpolation method used by the throttle curve
 *
 * @param[in]   method      Interpolation method
 */
void ThrottleCurve::setInterpolationMethod(InterpolationMethod method)
{
    interpolation = method;
    cacheValid = false;
}

/**
 * @brief   Gets the interpolation method used by the throttle curve
 *
 * @return  Interpolation method
 */
ThrottleCurve::InterpolationMethod ThrottleCurve::getInterpolationMethod() const
{
    return interpolation;
}

/**
 * @brief       Get an interpolated point on the curve
 *
 * @details     The method of interpolation can be set using setInterpolationMethod(...).
 *
 * @param[in]   input   Input in the range [0, inputMax]
 *
 * @return      Interpolated point
 */
ThrottleCurve::Point ThrottleCurve::getInterpolatedPoint(int input)
{
    if (!cacheValid)
    {
        interpolate();
    }
    
    return Point(input, cachedOutputs.at(input));
}

/**
 * @brief       Linear interpolation
 *
 * @param[in]   input   Input in the range [0, inputMax]
 *
 * @return      Interpolated point
 */
ThrottleCurve::Point ThrottleCurve::linearInterpolate(int input)
{
    Point p1(0, 0);
    Point p2(0, 0);
    
    // find the nearest points
    for (int i = 0; i < curve.size(); i++)
    {
        auto& point = curve.getReference(i);
        if (point.getX() < input)
        {
            p1 = point;
            if (input != inputMax)
            {
                p2 = curve.getReference(i + 1);
            }
            else
            {
                p2 = p1;
            }
        }
    }
    
    // linear interpolation
    int xDiff = p2.getX() - p1.getX();
    int yDiff = p2.getY() - p1.getY();
    
    float mu = static_cast<float>(input - p1.getX()) / static_cast<float>(xDiff);
    float m  = static_cast<float>(yDiff) / static_cast<float>(xDiff);

    int y = p1.getY() + m * mu * xDiff;
    
    return Point(input, y);
}

/**
 * @brief       Cosine interpolation
 *
 * @param[in]   input   Input in the range [0, inputMax]
 *
 * @return      Interpolated point
 */
ThrottleCurve::Point ThrottleCurve::cosineInterpolate(int input)
{
    Point p1(0, 0);
    Point p2(0, 0);
    
    // find the nearest points
    for (int i = 0; i < curve.size(); i++)
    {
        auto& point = curve.getReference(i);
        if (point.getX() < input)
        {
            p1 = point;
            p2 = curve.getReference(i + 1);
        }
    }
    
    // cosine interpolation
    float mu = static_cast<float>(input - p1.getX()) / static_cast<float>(p2.getX() - p1.getX());
    
    
    float mu2 = (1 - std::cos(mu * juce::MathConstants<float>::pi)) / 2;
    int y = (p1.getY() * (1 - mu2) + p2.getY() * mu2);

    return Point(input, y);
}

/**
 * @brief   Spline interpolation
 *
 * @param[in]   input   Input in the range [0, inputMax]
 * @param[in]   type    Type of spline to use (cubic or Hermite)
 *
 * @return      Interpolated point
 */
ThrottleCurve::Point ThrottleCurve::splineInterpolate(int input, tk::spline::spline_type type)
{
    // default to linear if too few points
    if (curve.size() < 3)
    {
        return linearInterpolate(input);
    }
    
    // spline
    std::vector<double> x, y;
    double currentX;
    double currentY;
    double lastX = -1;
    
    for (const auto& point : curve)
    {
        currentX = static_cast<double>(point.getX());
        currentY = static_cast<double>(point.getY());
        
        // enforce strict monotonicity
        if (lastX == currentX)
        {
            currentX += 1;
        }
        
        // add points to new lists
        x.push_back(currentX);
        y.push_back(currentY);
        
        // record last input
        lastX = currentX;
    }
    
    tk::spline spline(x, y, type);
    
    return Point(input, static_cast<int>(spline(input)));
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
    curveToReset.add(Point(defaultDeadzone, 0));
    curveToReset.add(Point(inputMax, outputMax));
    cacheValid = false;
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
    cacheValid = false;
}

/**
 * @brief Validate the interpolated curve
 *
 * @return String array containing warning text if any issues
 */
juce::StringArray ThrottleCurve::validateCurve()
{
    juce::StringArray warnings;
    bool positiveClipping = false;
    bool negativeClipping = false;
    int positiveClippingStart = 0;
    int negativeClippingStart = 0;
    bool strictlyIncreasing = true;
    
    // check curve
    for (int input = 0; input < inputMax + 1; input++)
    {
        // check for clipping
        int output = getInterpolatedPoint(input).getY();
        
        if (!positiveClipping && output > outputMax)
        {
            positiveClipping = true;
            positiveClippingStart = input;
        }
        else if (!negativeClipping && output < 0)
        {
            negativeClipping = true;
            negativeClippingStart = input;
        }
        
        // check for strictly increasing
        if (input > 0 && output != 0)
        {
            if (output < getInterpolatedPoint(input - 1).getY())
            {
                strictlyIncreasing = false;
            }
        }
    }
    
    // add warnings
    if (positiveClipping)
    {
        float percentStart = 100 * std::round(100 * static_cast<float>(positiveClippingStart) / ThrottleCurve::getInputMax()) / 100;
        warnings.add("Warning: clipping (above max @ " +  juce::String(percentStart) + "% input)");
    }
    
    if (negativeClipping)
    {
        float percentStart = 100 * std::round(100 * static_cast<float>(negativeClippingStart) / ThrottleCurve::getInputMax()) / 100;
        warnings.add("Warning: clipping (below 0 @ " + juce::String(percentStart) + "% input)");
    }
    
    if (!strictlyIncreasing)
    {
        warnings.add("Warning: curve not strictly increasing");
    }
    
    return warnings;
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

/**
 * @brief Returns the default interpolation method
 */
const ThrottleCurve::InterpolationMethod ThrottleCurve::getDefaultInterpolationMethod()
{
    return defaultInterpolationMethod;
}

//================================================================= Static data

const juce::Array<ThrottleCurve::InterpolationMethod> ThrottleCurve::listOfInterpolationMethods = {
    ThrottleCurve::InterpolationMethod::Linear,
    ThrottleCurve::InterpolationMethod::Cosine,
    ThrottleCurve::InterpolationMethod::Cubic,
    ThrottleCurve::InterpolationMethod::Hermite
};

const juce::Array<juce::String> ThrottleCurve::namesOfInterpolationMethods = {
    "Linear", "Cosine", "C2 Spline", "Hermite Spline"
};
