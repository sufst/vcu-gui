/******************************************************************************
 * @file   ThrottleCurveComponent.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Component for drawing throttle curves
 *****************************************************************************/

#include <JuceHeader.h>
#include <algorithm>
#include <functional>
#include <cmath>

#include "ThrottleCurveComponent.h"

//==================================================== Constructor / destructor

/**
 * @brief Default constructor
 */
ThrottleCurveComponent::ThrottleCurveComponent()
{
    // setup and add children
    interpolationMethodComboBox.setTitle("Interpolation method");
    
    const auto& methods = ThrottleCurve::getAllInterpolationMethods();
    
    for (int i = 0; i < methods.size(); i++)
    {
        const auto& name = ThrottleCurve::getInterpolationMethodName(methods.getReference(i));
        interpolationMethodComboBox.addItem(name, i + 1);
    }
    
    
    addAndMakeVisible(interpolationMethodComboBox);
    
    
}

/**
 * @brief Destructor
 */
ThrottleCurveComponent::~ThrottleCurveComponent()
{
}

//==================================================================== Graphics

/**
 * @brief Painter
 *
 * @param[in]   g   Graphics context
 */
void ThrottleCurveComponent::paint(juce::Graphics& g)
{
    
    // fill background
    juce::Colour backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    g.fillAll(backgroundColour);
    
    // draw border around whole component
    g.setColour(juce::Colours::white);
    g.drawRect(0, 0, getWidth(), getHeight());
}

/**
 * @brief Resize handler
 */
void ThrottleCurveComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // remove a bar from the bottom of the component
    auto lowerBar = bounds.removeFromBottom(lowerBarHeight);
    auto lowerBarWidth = lowerBar.getWidth();
    
    interpolationMethodComboBox.setBounds(lowerBar.removeFromRight(lowerBarWidth / 4));
}
