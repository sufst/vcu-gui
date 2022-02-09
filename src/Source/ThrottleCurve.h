/******************************************************************************
 * @file   ThrottleCurve.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Model of a throttle curve
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

/**
 * @brief Model of a throttle curve
 */
class ThrottleCurve
{
public:
    
    // types
    enum class InterpolationMethod {
        Linear = 0, Cosine, Bezier
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
    Point getInterpolatedPoint(int index1, int index2, float mu);
    Point getInterpolatedPoint(const Point& p1, const Point& p2, float mu);
    
    // static utility
    static int getInputMax();
    static int getOutputMax();
    static const juce::Array<InterpolationMethod>& getAllInterpolationMethods();
    static const juce::String& getInterpolationMethodName(ThrottleCurve::InterpolationMethod method);
    
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
    
    Point linearInterpolate(const Point& p1, const Point& p2, float mu);
    Point cosineInterpolate(const Point& p1, const Point& p2, float mu);
    
    // static
    static const juce::Array<ThrottleCurve::InterpolationMethod> listOfInterpolationMethods;
    static const juce::Array<juce::String> namesOfInterpolationMethods;
    static constexpr InterpolationMethod defaultInterpolationMethod = InterpolationMethod::Cosine;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrottleCurve)
};
