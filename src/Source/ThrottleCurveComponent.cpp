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
    // nothing to do
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
    
    // draw border around the component
    juce::Colour borderColour = getLookAndFeel().findColour(juce::ComboBox::outlineColourId);
    g.setColour(borderColour);
    g.drawRect(0, 0, getWidth(), getHeight());
    
    // create a path
    g.setColour(juce::Colours::red);
    juce::Path path;
    
    for (const auto& point : throttleCurve.getPoints())
    {
        auto realPoint = transformCurvePointToCanvas(point);
        g.drawEllipse(realPoint.getX(), realPoint.getY(), pointSize, pointSize, pointStroke);
    }
}

/**
 * @brief Resize handler
 */
void ThrottleCurveComponent::resized()
{
    // nothing to do
}

//====================================================================== Events

/**
 * @brief Handle a mouse down event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseDown(const juce::MouseEvent& event)
{
    // add new point
    if (!currentlyMovingPoint)
    {
        auto point = transformCanvasPointToCurve(event.getPosition());
        throttleCurve.addPoint(point);
    }
    
    // trigger a re-paint
    repaint();
}

/**
 * @brief Handle a mouse up event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseUp(const juce::MouseEvent& event)
{
    

}

/**
 * @brief Handle a mouse drag event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseDrag(const juce::MouseEvent& event)
{
    
}

//============================================================ Internal utility

/**
 * @brief Transforms a point on a throttle curve to its position on the component
 *
 * @param[in]   point   Point on the throttle curve
 *
 * @return Position to draw the point on the component
 */
juce::Point<int> ThrottleCurveComponent::transformCurvePointToCanvas(const ThrottleCurve::Point& point) const
{
    int x = getWidth() * (static_cast<float>(point.getX()) / static_cast<float>(ThrottleCurve::getInputMax()));
    int y = getHeight() * (1 - static_cast<float>(point.getY()) / static_cast<float>(ThrottleCurve::getOutputMax()));
    
    return juce::Point<int>(x, y);
}

/**
 * @brief Transforms a point on a throttle curve to its position on the component
 *
 * @param[in]   point   Point on the component
 *
 * @return Position to place the point on the throttle curve
 */
ThrottleCurve::Point ThrottleCurveComponent::transformCanvasPointToCurve(const juce::Point<int>& point) const
{
    int x = ThrottleCurve::getInputMax() * (static_cast<float>(point.getX()) / static_cast<float>(getWidth()));
    int y = ThrottleCurve::getOutputMax() * (1 - static_cast<float>(point.getY()) / static_cast<float>(getHeight()));
    
    return ThrottleCurve::Point(x, y);
}
