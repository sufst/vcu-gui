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
    
    // constructor / destructor
    ThrottleCurve();
    ~ThrottleCurve();
    
    // types
    enum class InterpolationMethod {
        Linear = 0, Cosine, Bezier
    };
    
    // static utility
    static const juce::Array<InterpolationMethod>& getAllInterpolationMethods();
    static const juce::String& getInterpolationMethodName(ThrottleCurve::InterpolationMethod method);
    
private:
    
    // state
    juce::Path path;
    InterpolationMethod interpolation;
    
    // static
    static const juce::Array<ThrottleCurve::InterpolationMethod> listOfInterpolationMethods;
    static const juce::Array<juce::String> namesOfInterpolationMethods;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrottleCurve)
};
