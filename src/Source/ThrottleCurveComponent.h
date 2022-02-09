/******************************************************************************
 * @file   ThrottleCurveComponent.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Component for drawing throttle curves
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

#include "ThrottleCurveComponent.h"
#include "ThrottleCurve.h"

/**
 * @brief Throttle curve drawing component
 */
class ThrottleCurveComponent
    :   public juce::Component
{
public:
    
    // constructor / destructor
    ThrottleCurveComponent();
    ~ThrottleCurveComponent() override;

    // painting
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    
    // internal components
    juce::ComboBox interpolationMethodComboBox;
    
    // GUI dimensions
    static const int lowerBarHeight = 20;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrottleCurveComponent)
    
};
