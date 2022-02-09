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
    
    // delete the points (unless they are the first and last ones)
    int deleteCount = 0;
    for (const auto index : toDelete)
    {
        int realIndex = index - deleteCount;
        
        if ((realIndex != 0) && (realIndex != curve.size() - 1))
        {
            curve.remove(index - deleteCount);
            deleteCount++;
        }
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

//================================================================ Interpolation

/**
 * @brief Sets the interpolation method used by the throttle curve
 *
 * @param[in]   method      Interpolation method
 */
void ThrottleCurve::setInterpolationMethod(InterpolationMethod method)
{
    interpolation = method;
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
    switch (interpolation)
    {
        case InterpolationMethod::Linear:
            return linearInterpolate(input);

        case InterpolationMethod::Cosine:
            return cosineInterpolate(input);
            
        case InterpolationMethod::Cubic:
            return splineInterpolate(input, tk::spline::cspline);
        
        case InterpolationMethod::Hermite:
            return splineInterpolate(input, tk::spline::cspline_hermite);

        default:
            // something hasn't been implemented!
            jassertfalse;
            return Point(input, outputMax / 2);
    }
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
    
    for (const auto& point : curve)
    {
        x.push_back(static_cast<double>(point.getX()));
        y.push_back(static_cast<double>(point.getY()));
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
