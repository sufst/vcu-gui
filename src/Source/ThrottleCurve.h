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
    
    // getters
    const juce::Array<juce::Point<int>>& getPoints() const;
    int getInputMax() const;
    int getOutputMax() const;
    
    // static utility
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
    
    // static
    static const juce::Array<ThrottleCurve::InterpolationMethod> listOfInterpolationMethods;
    static const juce::Array<juce::String> namesOfInterpolationMethods;
    static constexpr InterpolationMethod defaultInterpolationMethod = InterpolationMethod::Linear;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrottleCurve)
};
