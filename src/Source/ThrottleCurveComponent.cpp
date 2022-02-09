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
    // combo box for selecting interpolation method
    interpolationMethodComboBox.setTitle("Interpolation method");
    
    const auto& methods = ThrottleCurve::getAllInterpolationMethods();
    
    for (int i = 0; i < methods.size(); i++)
    {
        const auto& name = ThrottleCurve::getInterpolationMethodName(methods.getReference(i));
        interpolationMethodComboBox.addItem(name, i + 1);
    }
    
    interpolationMethodComboBox.setSelectedId(1);
    
    // add children
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
    
    // draw border around the component excluding the lower bar
    juce::Colour borderColour = getLookAndFeel().findColour(juce::ComboBox::outlineColourId);
    g.setColour(borderColour);
    g.drawRect(0, 0, getWidth(), getHeight() - lowerBarHeight);
    
    // create a path
    g.setColour(juce::Colours::red);
    juce::Path path;
    
    for (const auto& point : throttleCurve.getPoints())
    {
        auto realPoint = transformCurvePoint(point);
        g.drawEllipse(realPoint.getX(), realPoint.getY(), pointSize, pointSize, pointStroke);
    }
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

//============================================================ Internal utility

/**
 * @brief Transforms a point on a throttle curve to its position on the component
 *
 * @param[in]   point   Point on the throttle curve
 *
 * @return Position to draw the point on the component
 */
juce::Point<int> ThrottleCurveComponent::transformCurvePoint(const ThrottleCurve::Point& point) const
{
    // don't draw in the bottom bar
    // use the full width
    int height = getHeight() - lowerBarHeight;
    int width = getWidth();
    
    // map the point
    int x = width * (static_cast<float>(point.getX()) / static_cast<float>(ThrottleCurve::getInputMax()));
    int y = height * (1 - static_cast<float>(point.getY()) / static_cast<float>(ThrottleCurve::getOutputMax()));
    
    return juce::Point<int>(x, y);
}
