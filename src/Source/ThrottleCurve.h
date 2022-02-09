/******************************************************************************
 * @file   ThrottleCurve.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Model of a throttle curve
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>
#include "../../lib/spline/src/spline.h"

/**
 * @brief Model of a throttle curve
 */
class ThrottleCurve
{
public:
    
    // types
    enum class InterpolationMethod {
        Linear = 0, Cosine, Cubic, Hermite
    };
    
    typedef juce::Point<int> Point;
    
    // constructor / destructor
    ThrottleCurve();
    ThrottleCurve(InterpolationMethod interpolationMethod);
    ~ThrottleCurve();
    
    // points
    void addPoint(Point& point);
    void deleteNearbyPoints(const Point& point, int radius);
    juce::Array<Point>& getPoints();
    Point* pointMoved(const Point movedPoint);
    
    // interpolation
    Point getInterpolatedPoint(int input);
    void setInterpolationMethod(InterpolationMethod method);

    // static utility
    static int getInputMax();
    static int getOutputMax();
    static const juce::Array<InterpolationMethod>& getAllInterpolationMethods();
    static const juce::String& getInterpolationMethodName(ThrottleCurve::InterpolationMethod method);
    static const InterpolationMethod getDefaultInterpolationMethod();
    
private:
    
    // constants
    static const int inputResolution = 10;
    static const int outputResolution = 16;
    static const int inputMax = (1 << inputResolution) - 1;
    static const int outputMax = (1 << outputResolution) - 1;
    
    // state
    juce::Array<Point> curve;
    InterpolationMethod interpolation;
    
    // internal utility
    void resetCurveToDefault(juce::Array<Point>& curveToReset);
    void sortCurve(juce::Array<Point>& curveToSort);
    
    Point linearInterpolate(int input);
    Point cosineInterpolate(int input);
    Point splineInterpolate(int input, tk::spline::spline_type type);
    
    // static
    static const juce::Array<ThrottleCurve::InterpolationMethod> listOfInterpolationMethods;
    static const juce::Array<juce::String> namesOfInterpolationMethods;
    static constexpr InterpolationMethod defaultInterpolationMethod = InterpolationMethod::Cubic;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrottleCurve)
};
