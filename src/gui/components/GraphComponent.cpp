/******************************************************************************
 * @file   GraphComponent.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @date   2022-07-11
 * @brief  Component for drawing a graph
 *****************************************************************************/

#include "GraphComponent.h"

namespace gui
{

/**
 * @brief Default constructor
 */
GraphComponent::GraphComponent()
{
    setRangeX(0, 1);
    setRangeY(0, 1);
}

/**
 * @brief Constructor
 */
GraphComponent::GraphComponent(std::shared_ptr<juce::Path> path)
    :   sharedPath(path)
{
    jassert(sharedPath);
    
    auto pathStart = sharedPath->getPointAlongPath(0.f);
    auto pathEnd = sharedPath->getPointAlongPath(sharedPath->getLength());

    setRangeX(pathStart.getX(), pathEnd.getX());
    setRangeY(pathStart.getY(), pathEnd.getY());
}

/**
 * @brief 
 * 
 * @param newPath 
 */
void GraphComponent::setPath(std::shared_ptr<juce::Path> newPath)
{
    jassert(newPath);
    sharedPath = newPath;
}

/**
 * @brief       Set the range of the x-axis
 * 
 * @param[in]   min     Minimum value
 * @param[in]   max     Maximum value
 */
void GraphComponent::setRangeX(float min, float max)
{
    valueBounds.setX(min);
    valueBounds.setWidth(max - min);
}

/**
 * @brief       Set the range of the y-axis
 * 
 * @param[in]   min     Minimum value
 * @param[in]   max     Maximum value
 */
void GraphComponent::setRangeY(float min, float max)
{
    valueBounds.setY(max);
    valueBounds.setHeight(max - min);
}

/**
 * @brief       Painter
 * 
 * @param[in]   g   JUCE graphics context
 */
void GraphComponent::paint(juce::Graphics& g)
{
    paintBackground(g);
    paintTicks(g);
    paintBorder(g);
    paintCurve(g);
}


/**
 * @brief       Paint graph background
 * 
 * @param[in]   g   JUCE graphics context
 */
void GraphComponent::paintBackground(juce::Graphics& g) const
{
    g.fillAll(juce::Colours::white);
}

/**
 * @brief       Paint graph ticks
 * 
 * @param[in]   g   JUCE graphics context
 */
void GraphComponent::paintTicks(juce::Graphics& g) const 
{
    const int numTicksX = 40;
    const int numTicksY = 20;

    auto bounds = getLocalBounds().toFloat();

    g.setColour(juce::Colours::lightgrey);

    for (int i = 0; i < numTicksX; i++)
    {
        float x = i * getWidth() / numTicksX;
        g.drawVerticalLine(x, 0, bounds.getHeight());
    }

    for (int i = 0; i < numTicksY; i++)
    {
        float y = i * getHeight() / numTicksY;
        g.drawHorizontalLine(y, 0, getWidth());
    }
}

/**
 * @brief       Paint graph border
 * 
 * @param[in]   g   JUCE graphics context
 */
void GraphComponent::paintBorder(juce::Graphics& g) const
{
    g.setColour(juce::Colours::lightblue);
    g.drawRect(0, 0, getWidth(), getHeight(), 2);
}

/**
 * @brief       Paint graph curve
 * 
 * @param[in]   g   JUCE graphics context
 */
void GraphComponent::paintCurve(juce::Graphics& g)
{
    if (sharedPath)
    {
        g.setColour(juce::Colours::orange);
        g.strokePath(*sharedPath, juce::PathStrokeType(1));
    }
}

} // namespace gui