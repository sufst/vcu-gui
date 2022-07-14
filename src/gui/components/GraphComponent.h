/******************************************************************************
 * @file   GraphComponent.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @date   2022-07-11
 * @brief  Component for drawing a graph
 *****************************************************************************/

#include "../utility/PointComparator.h"
#include <JuceHeader.h>
#include <memory>

namespace gui
{

/**
 * @brief   Graph drawing component
 * 
 * @details Currently supports only a graph in the positive x/y quadrant, but should easily be extendible to all four
 *          quadrants
 */
template <typename ValueType> class GraphComponent : public juce::Component
{
public:

    /**
     * @brief Default constructor
     */
    GraphComponent()
    {
        setRangeX(0, 1);
        setRangeY(0, 1);
    }

    /**
     * @brief       Set the range of the x-axis
     *
     * @param[in]   min     Minimum value
     * @param[in]   max     Maximum value
     */
    void setRangeX(ValueType min, ValueType max)
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
    void setRangeY(ValueType min, ValueType max)
    {
        valueBounds.setY(max);
        valueBounds.setHeight(max - min);
    }

    /**
     * @brief Returns the maximum x-coordinate value
     */
    ValueType getMaxX() const 
    {
        return valueBounds.getWidth();
    }

    /**
     * @brief Returns the maximum y-coordinate value
     */
    ValueType getMaxY() const 
    {
        return valueBounds.getHeight();
    }

    /**
     * @brief       Adds a point to the graph
     *
     * @param[in]   x   x-coordinate of the point
     * @param[in]   y   y-coordinate of the point
     */
    void addPoint(ValueType x, ValueType y)
    {
        addPoint({x, y});
    }

    /**
     * @brief       Adds a point to the graph
     *
     * @param[in]   point   The point to add
     */
    void addPoint(const juce::Point<ValueType>& point)
    {
        static PointComparator<ValueType> comparator;
        points.addSorted(comparator, point);

        repaint();
    }

    /**
     * @brief Implements juce::Component::paint()
     */
    void paint(juce::Graphics& g)
    {
        paintBackground(g);
        paintTicks(g);
        paintBorder(g);
        paintCurve(g);
    }

private:

    /**
     * @brief       Paint graph background
     *
     * @param[in]   g   JUCE graphics context
     */
    void paintBackground(juce::Graphics& g) const
    {
        g.fillAll(juce::Colours::white);
    }

    /**
     * @brief       Paint graph ticks
     *
     * @param[in]   g   JUCE graphics context
     */
    void paintTicks(juce::Graphics& g) const
    {
        const int numTicksX = 40;
        const int numTicksY = 20;

        auto bounds = getLocalBounds().toFloat();

        g.setColour(juce::Colours::lightgrey);

        for (int i = 0; i < numTicksX; i++)
        {
            int x = i * getWidth() / numTicksX;
            g.drawVerticalLine(x, 0, bounds.getHeight());
        }

        for (int i = 0; i < numTicksY; i++)
        {
            int y = i * getHeight() / numTicksY;
            g.drawHorizontalLine(y, 0, getWidth());
        }
    }

    /**
     * @brief       Paint graph border
     *
     * @param[in]   g   JUCE graphics context
     */
    void paintBorder(juce::Graphics& g) const
    {
        g.setColour(juce::Colours::lightblue);
        g.drawRect(0, 0, getWidth(), getHeight(), 2);
    }

    /**
     * @brief       Paint graph curve
     *
     * @param[in]   g   JUCE graphics context
     */
    void paintCurve(juce::Graphics& g) const
    {
        auto bounds = getLocalBounds().toFloat();
        const int circleSize = 10;
        const int circleShift = circleSize / 2;

        for (const auto& point : points)
        {
            auto transformedPoint = transformPointForPaint(bounds, point);

            int x = transformedPoint.getX() - circleShift;
            int y = transformedPoint.getY() - circleShift;

            g.drawEllipse(x, y, circleSize, circleSize, 2);
        }
    }

    /**
     * @brief       Transforms a graph point to the coordinates system used for painting
     *
     * @param[in]
     * @param[in]   point   Point on the graph
     */
    juce::Point<int> transformPointForPaint(const juce::Rectangle<float>& bounds,
                                                  const juce::Point<ValueType>& point) const
    {
        const float xScale = bounds.getWidth() / valueBounds.getWidth();
        const float yScale = bounds.getHeight() / valueBounds.getHeight();

        auto x = static_cast<int>(point.getX() * xScale);
        auto y = static_cast<int>(bounds.getHeight() - point.getY() * yScale);

        return {x, y};
    }

    juce::Rectangle<ValueType> valueBounds;
    juce::Array<juce::Point<ValueType>> points;
};

} // namespace gui