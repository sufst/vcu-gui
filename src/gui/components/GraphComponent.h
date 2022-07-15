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
 * @brief   A graph drawing component which is optionally editable by mouse events
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
        setEditable(true);
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
    void paint(juce::Graphics& g) override
    {
        paintBackground(g);
        paintTicks(g);
        paintBorder(g);
        paintCurve(g);
    }

    /**
     * @brief   Implements juce::Component::mouseDown()
     * 
     * @details If near an existing point, it will be grabbed for move.
     *          Otherwise, a new point will be created.
     */
    void mouseDown(const juce::MouseEvent& event) override 
    {
        // check if should create new
        if (pointEditState == PointEditingState::None)
        {
            movingPointIndex = mouseEventIsNearAnyPoint(event);

            if (movingPointIndex == -1)
            {
                pointEditState = PointEditingState::Create;
            }
        }

        // create new
        if (pointEditState == PointEditingState::Create)
        {
            const auto newPoint = transformPointToGraph(event.getPosition());
            addPoint(newPoint);

            // TODO: this re-searches for the point that was just added
            //       better to instead take note of which index the point was added at
            movingPointIndex = mouseEventIsNearAnyPoint(event);
        }

        // immediately begin move (including after creation)
        pointEditState = PointEditingState::Move;
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    }

    /**
     * @brief   Implements juce::Component::mouseDrag
     * 
     * @details This handles the dragging of points when grabbed for move
     */
    void mouseDrag(const juce::MouseEvent& event) override
    {
        if (pointEditState == PointEditingState::Move)
        {
            jassert(movingPointIndex != -1 && movingPointIndex < points.size());

            auto& point = points.getReference(movingPointIndex);
            point = transformPointToGraph(event.getPosition());
            repaint();
        }
    }

    /**
     * @brief   Implements juce::Component::mouseMove
     * 
     * @details This changes the cursor depending on the current edit context
     */
    void mouseMove(const juce::MouseEvent& event) override 
    {
        if (pointEditState == PointEditingState::Move || mouseEventIsNearAnyPoint(event) != -1) 
            // note first check guarantees second is true and is much faster
        {
            setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        }
        else 
        {
            setMouseCursor(juce::MouseCursor::CrosshairCursor);
        }
    }

    /**
     * @brief   Implements juce::Component::mouseUp()
     * 
     * @details This ends a point move event
     */
    void mouseUp(const juce::MouseEvent& /*event*/) override 
    {
        pointEditState = PointEditingState::None;
        setMouseCursor(juce::MouseCursor::CrosshairCursor);
    }

    /**
     * @brief Allows the points on the graph to be edited by mouse events
     */
    void setEditable(bool shouldBeEditable = true)
    {
        editable = shouldBeEditable;
        setInterceptsMouseClicks(shouldBeEditable, shouldBeEditable);
    }

    /**
     * @brief Returns whether or not the graph is editable
     */
    bool isEditable() const 
    {
        return editable;
    }

protected:

    juce::Rectangle<ValueType> valueBounds;
    juce::Array<juce::Point<ValueType>> points;

private:

    /**
     * @brief 
     */
    enum class PointEditingState
    {
        None,
        Create,
        Move,
    };

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
        const int circleSize = 4;
        const int circleShift = circleSize / 2;

        for (const auto& point : points)
        {
            auto transformedPoint = transformPointForPaint(bounds, point);

            int x = transformedPoint.getX() - circleShift;
            int y = transformedPoint.getY() - circleShift;

            g.drawEllipse(x, y, circleSize, circleSize, circleSize);
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

    /**
     * @brief       Transforms a GUI point to the coordinates system of the graph
     * 
     * @details     Use this in combination with mouse events to let the user add points to the graph
     *  
     * @param[in]   point   The point to transform
     */
    juce::Point<ValueType> transformPointToGraph(const juce::Point<int>& point) const
    {
        const float xScale = static_cast<float>(getMaxX()) / getWidth();
        const float yScale = static_cast<float>(getMaxY()) / getHeight();

        auto x = static_cast<ValueType>(point.getX() * xScale);
        auto y = getMaxY() - static_cast<ValueType>(point.getY() * yScale);

        return {x, y};
    }

    /**
     * @brief 
     * 
     * @param[in]   guiPoint        Point in the component's coordinate system
     * @param[in]   graphPoint      Point in the graph's coordinate system
     * 
     * @return      true            The component point is equivalent to the graph point
     * @return      false           The component point is not equivalent to the graph point
     */
    bool pointHitTest(const juce::Point<int>& guiPoint, const juce::Point<ValueType>& graphPoint) const
    {
        const int clickRadius = 10;

        auto transformedPoint = transformPointForPaint(getLocalBounds().toFloat(), graphPoint);
        int distance = transformedPoint.getDistanceFrom(guiPoint);

        return distance < clickRadius;
    }

    /**
     * @brief       Checks if a mouse event is near a point on the graph, returning the index of the point if it does 
     *              and -1 otherwise
     * 
     * @param[in]   event   Mouse event
     * 
     * @return      -1      Mouse event is not near any points
     * @return      >0      Mouse event is near the point with the returned index
     */
    int mouseEventIsNearAnyPoint(const juce::MouseEvent& event)
    {
        const auto eventPosition = event.getPosition().toInt();

        for (int i = 0; i < points.size(); i++)
        {
            const auto& point = points.getReference(i);

            if (pointHitTest(eventPosition, point))
            {
                return i;
            }
        }

        return -1;
    }

    /**
     * @brief Determines whether or not the graph is editable and handles mouse events
     */
    bool editable;

    /**
     * @brief State of point editing
     */
    PointEditingState pointEditState = PointEditingState::None;

    /**
     * @brief Index of point currently being moved
     */
    int movingPointIndex = -1;
};

} // namespace gui