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
    setWantsKeyboardFocus(true);
    setMouseCursor(juce::MouseCursor::CrosshairCursor);
    addKeyListener(this);
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
    
    // draw interpolated points
    g.setColour(juce::Colours::white);
    int numPoints = getWidth() / 1;
    
    for (int input = 0; input < ThrottleCurve::getInputMax(); input += ThrottleCurve::getInputMax() / numPoints)
    {
        const auto interpolatedPoint = throttleCurve.getInterpolatedPoint(input);
        const auto transformedPoint = transformCurvePointToCanvas(interpolatedPoint);
        g.drawEllipse(transformedPoint.getX(), transformedPoint.getY(), 1, 1, pointStroke / 2);
    }
    
    // draw points
    g.setColour(juce::Colours::red);
    
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
    // check if a point already exists in the clicked location
    // if one does and not deleting it, begin a move
    if (!deleteMode)
    {
        for (int i = 0; i < throttleCurve.getPoints().size(); i++)
        {
            
            if (pointHitTest(event.getPosition(), throttleCurve.getPoints()[i]))
            {
                pMovingPoint = &throttleCurve.getPoints().getReference(i);
                currentlyMovingPoint = true;
                setMouseCursor(juce::MouseCursor::DraggingHandCursor);
                break;
            }
        }
    }
    
    // add / delete if not moving
    if (!currentlyMovingPoint)
    {
        // delete
        if (deleteMode)
        {
            ThrottleCurve::Point point = transformCanvasPointToCurve(event.getPosition());
            throttleCurve.deleteNearbyPoints(point, throttleCurveClickRadius);
        }
        // add
        else
        {
            auto point = transformCanvasPointToCurve(event.getPosition());
            throttleCurve.addPoint(point);
        }
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
    // stop moving points
    if (currentlyMovingPoint)
    {
        currentlyMovingPoint = false;
        pMovingPoint = nullptr;
        setMouseCursor(juce::MouseCursor::CrosshairCursor);
    }
}

/**
 * @brief Handle a mouse drag event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseDrag(const juce::MouseEvent& event)
{
    // move the point
    if (currentlyMovingPoint)
    {
        ThrottleCurve::Point point = transformCanvasPointToCurve(event.getPosition());
        pMovingPoint->setXY(point.getX(), point.getY());
        pMovingPoint = throttleCurve.pointMoved(*pMovingPoint);
    }
    
    // trigger a re-paint
    repaint();
}

/**
 * @brief Handle a mouse move event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseMove(const juce::MouseEvent& event)
{
    // change mouse cursor if a point is within the grab radius
    if (!deleteMode && !currentlyMovingPoint)
    {
        bool hit = false;
        
        for (const auto& point : throttleCurve.getPoints())
        {
            if (pointHitTest(event.getPosition(), point))
            {
                hit = true;
                break;
            }
        }
        
        if (hit)
        {
            setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        }
        else
        {
            setMouseCursor(juce::MouseCursor::CrosshairCursor);
        }
    }
}

/**
 * @brief Handle a key press event
 *
 * @param[in]   key                     Key pressed
 * @param[in]   originatingComponent    Component from which the key press originated
 */
bool ThrottleCurveComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    // toggle delete mode
    if (key.isKeyCode(juce::KeyPress::backspaceKey))
    {
        deleteMode = !deleteMode;
        
        if (deleteMode)
        {
            setMouseCursor(juce::MouseCursor(juce::ImageCache::getFromMemory(BinaryData::Delete_png, BinaryData::Delete_pngSize), 1, 7, 5));
        }
        
        else
        {
            setMouseCursor(juce::MouseCursor::CrosshairCursor);
        }
    }
    return true;
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

/**
 * @brief Hit test between a point on the canvas and a point on the curve
 */

bool ThrottleCurveComponent::pointHitTest(const juce::Point<int>& canvasPoint, const ThrottleCurve::Point& curvePoint)
{
    juce::Point<int> transformedCurvePoint = transformCurvePointToCanvas(curvePoint);
    return (canvasPoint.getDistanceFrom(transformedCurvePoint) < clickRadius);
}
