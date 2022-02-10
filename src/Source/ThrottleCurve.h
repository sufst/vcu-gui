/******************************************************************************
 * @file   ThrottleCurve.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Model of a throttle curve
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>
#include <vector>
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
    const juce::Array<Point>& getPoints();
    Point* getPointForMove(int index);
    Point* pointMoved(const Point movedPoint);
    void reset();
    
    // interpolation
    void interpolate();
    Point getInterpolatedPoint(int input);
    void setInterpolationMethod(InterpolationMethod method);
    InterpolationMethod getInterpolationMethod() const;
    juce::StringArray validateCurve();

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
    
    std::array<int, outputMax + 1> cachedOutputs;
    bool cacheValid = false;
    
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
