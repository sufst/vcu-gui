/******************************************************************************
 * @file   GraphComponent.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @date   2022-07-11
 * @brief  Component for drawing a graph
 *****************************************************************************/

#pragma once

#include "../../Interpolator.h"
#include "../appearance/Colours.h"
#include "../utility/PointComparator.h"
#include "../utility/clip.h"
#include <JuceHeader.h>
#include <memory>

namespace gui
{

using utility::Interpolator; // TODO: does this pollute the namespace when #include'ing this file?
using utility::InterpolatorFactory;
using utility::SplineInterpolator;

/**
 * @brief   A graph drawing component which is optionally editable by mouse events
 *
 * @details Currently supports only a graph in the positive x/y quadrant, but should easily be extendible to all four
 *          quadrants
 */
template <typename ValueType>
class GraphComponent : public juce::Component, public juce::KeyListener
{
public:

    GraphComponent();

    void setRangeX(ValueType min, ValueType max);
    void setRangeY(ValueType min, ValueType max);
    void setEditable(bool shouldBeEditable = true);
    void setInterpolationMethod(const juce::Identifier& identifier);
    void setDrawsInterpolatedCurve(bool shouldDrawInterpolatedCurve = true);

    ValueType getMinX() const;
    ValueType getMinY() const;
    ValueType getMaxX() const;
    ValueType getMaxY() const;
    bool isEditable() const;

    void addPoint(ValueType x, ValueType y);
    void addPoint(const juce::Point<ValueType>& point);
    void clear();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;
    bool keyPressed(const juce::KeyPress& key) override;

protected:

    juce::Rectangle<ValueType> valueBounds;
    juce::Array<juce::Point<ValueType>> points;
    juce::Path interpolatedPath;

    juce::Point<int> transformPointForPaint(const juce::Rectangle<float>& bounds,
                                            const juce::Point<ValueType>& point) const;
    juce::Point<ValueType> transformPointToGraph(const juce::Point<int>& point) const;
    bool pointHitTest(const juce::Point<int>& guiPoint, const juce::Point<ValueType>& graphPoint) const;
    int getPointNearMouseEvent(const juce::MouseEvent& event) const;

    void pointsChanged();

private:

    void paintTicks(juce::Graphics& g) const;
    void paintBorder(juce::Graphics& g) const;
    void paintPoints(juce::Graphics& g) const;
    void paintCurve(juce::Graphics& g) const;

    void recalculateInterpolatedPath();

    void updateCursor();

    /**
     * @brief State representing current graph editing action
     */
    enum class PointEditingState
    {
        None,
        OverPoint,
        Create,
        Move,
        Delete,
    };

    bool editable;
    bool shouldInterpolate;
    PointEditingState pointEditState = PointEditingState::None;
    int movingPointIndex = -1;

    std::unique_ptr<Interpolator<int>> interpolator;

    const juce::Colour pointColour = sufst::Colours::sfsyellow;
    const juce::Colour lineColour = sufst::Colours::white;
    const juce::Colour borderColour = sufst::Colours::midgrey;
    const juce::Colour tickColour = sufst::Colours::darkgrey;
};

/**
 * @brief Default constructor
 */
template <typename ValueType>
GraphComponent<ValueType>::GraphComponent()
{
    setRangeX(0, 1);
    setRangeY(0, 1);
    setEditable(true);
    setSize(100, 100); // need to start with non-zero size for point transformations
    setWantsKeyboardFocus(true);
    setDrawsInterpolatedCurve(true);

    setInterpolationMethod(SplineInterpolator<ValueType>::identifier);

    addKeyListener(this);
}

/**
 * @brief       Set the range of the x-axis
 *
 * @param[in]   min     Minimum value
 * @param[in]   max     Maximum value
 */
template <typename ValueType>
void GraphComponent<ValueType>::setRangeX(ValueType min, ValueType max)
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
template <typename ValueType>
void GraphComponent<ValueType>::setRangeY(ValueType min, ValueType max)
{
    valueBounds.setY(min);
    valueBounds.setHeight(max - min);
}

/**
 * @brief Returns the minimum value of the x-axis
 */
template <typename ValueType>
ValueType GraphComponent<ValueType>::getMinX() const
{
    return valueBounds.getX();
}

/**
 * @brief Returns the minimum value of the y-axis
 */
template <typename ValueType>
ValueType GraphComponent<ValueType>::getMinY() const
{
    return valueBounds.getY();
}

/**
 * @brief Returns the maximum value of the x-axis
 */
template <typename ValueType>
ValueType GraphComponent<ValueType>::getMaxX() const
{
    return valueBounds.getWidth() - valueBounds.getX();
}

/**
 * @brief Returns the maximum value of the y-axis
 */
template <typename ValueType>
ValueType GraphComponent<ValueType>::getMaxY() const
{
    return valueBounds.getHeight() - valueBounds.getY();
}

/**
 * @brief       Adds a point to the graph
 *
 * @param[in]   x   x-coordinate of the point
 * @param[in]   y   y-coordinate of the point
 */
template <typename ValueType>
void GraphComponent<ValueType>::addPoint(ValueType x, ValueType y)
{
    addPoint({x, y});
}

/**
 * @brief       Adds a point to the graph
 *
 * @param[in]   point   The point to add
 */
template <typename ValueType>
void GraphComponent<ValueType>::addPoint(const juce::Point<ValueType>& point)
{
    static PointComparator<ValueType> comparator;
    points.addSorted(comparator, point);

    pointsChanged();
}

/**
 * @brief Clears all points from the graph
 */
template <typename ValueType>
void GraphComponent<ValueType>::clear()
{
    points.clear();
}

/**
 * @brief       Changes the interpolation method
 *
 * @param[in]   identifier     The new interpolator to use
 */
template <typename ValueType>
void GraphComponent<ValueType>::setInterpolationMethod(const juce::Identifier& identifier)
{
    interpolator = InterpolatorFactory<ValueType>::makeInterpolator(identifier);
    jassert(interpolator);
}

/**
 * @brief       Sets whether or not the interpolated curve should be calculated and drawn
 *
 * @param[in]   shouldDrawInterpolatedCurve     Set true to draw interpolated curve
 */
template <typename ValueType>
void GraphComponent<ValueType>::setDrawsInterpolatedCurve(bool shouldDrawInterpolatedCurve)
{
    shouldInterpolate = shouldDrawInterpolatedCurve;
}

/**
 * @brief Implements juce::Component::paint()
 */
template <typename ValueType>
void GraphComponent<ValueType>::paint(juce::Graphics& g)
{
    paintTicks(g);
    paintBorder(g);

    if (shouldInterpolate)
    {
        paintCurve(g);
    }

    paintPoints(g);
}

/**
 * @brief   Implements juce::Component::resized()
 *
 * @details This applies an affine transform to the interpolated path to resize it to the available bounds
 *
 * @note    The component must start with a non-zero size, else the calls to resized() on app initialisation
 *          will result in an invalid (infinite) transform matrix which throws an exception when it is applied
 */
template <typename ValueType>
void GraphComponent<ValueType>::resized()
{
    auto bounds = getLocalBounds().toFloat();
    auto transform = interpolatedPath.getTransformToScaleToFit(bounds, false);
    interpolatedPath.applyTransform(transform);
}

/**
 * @brief   Implements juce::Component::mouseDown()
 *
 * @details If near an existing point, it will be grabbed for move.
 *          Otherwise, a new point will be created.
 */
template <typename ValueType>
void GraphComponent<ValueType>::mouseDown(const juce::MouseEvent& event)
{
    int pointIndex = getPointNearMouseEvent(event);

    // check if should create new or move
    if (pointEditState == PointEditingState::None || pointEditState == PointEditingState::OverPoint)
    {
        // create new
        if (pointIndex == -1)
        {
            const auto newPoint = transformPointToGraph(event.getPosition());
            addPoint(newPoint);

            // TODO: this effectively searches for the point that was just added, could be made more efficient
            movingPointIndex = getPointNearMouseEvent(event);
        }
        // move
        else
        {
            movingPointIndex = pointIndex;
        }

        // always move, including after creation
        pointEditState = PointEditingState::Move;
    }

    // check if should delete AFTER previous, since this goes to None state
    if (pointEditState == PointEditingState::Delete && pointIndex != -1)
    {
        points.remove(pointIndex);
        pointsChanged();
    }

    updateCursor();
}

/**
 * @brief   Implements juce::Component::mouseDrag
 *
 * @details This handles the dragging of points when grabbed for move
 */
template <typename ValueType>
void GraphComponent<ValueType>::mouseDrag(const juce::MouseEvent& event)
{

    if (pointEditState == PointEditingState::Move)
    {
        jassert(movingPointIndex != -1 && movingPointIndex < points.size());

        // move the point
        auto& point = points.getReference(movingPointIndex);
        point = transformPointToGraph(event.getPosition());

        // check if point has moved past the x-coordinate of another point
        // swap them if this is the case
        if (movingPointIndex != 0 && points[movingPointIndex - 1].getX() > point.getX())
        {
            points.swap(movingPointIndex, movingPointIndex - 1);
            movingPointIndex = movingPointIndex - 1;
        }
        else if (movingPointIndex != points.size() - 1
                 && points[movingPointIndex].getX() > points[movingPointIndex + 1].getX())
        {
            points.swap(movingPointIndex, movingPointIndex + 1);
            movingPointIndex = movingPointIndex + 1;
        }

        pointsChanged();
    }
}

/**
 * @brief   Implements juce::Component::mouseMove()
 *
 * @details This changes the cursor depending on the current edit context
 */
template <typename ValueType>
void GraphComponent<ValueType>::mouseMove(const juce::MouseEvent& event)
{
    bool nearPoint = getPointNearMouseEvent(event) != -1;

    if (pointEditState == PointEditingState::None && nearPoint)
    {
        pointEditState = PointEditingState::OverPoint;
    }
    else if (pointEditState == PointEditingState::OverPoint && !nearPoint)
    {
        pointEditState = PointEditingState::None;
    }

    updateCursor();
}

/**
 * @brief   Implements juce::Component::mouseUp()
 *
 * @details This ends a point move event
 */
template <typename ValueType>
void GraphComponent<ValueType>::mouseUp(const juce::MouseEvent& /*event*/)
{
    if (pointEditState != PointEditingState::Delete)
    {
        pointEditState = PointEditingState::None;
    }

    updateCursor();
}

/**
 * @brief   Implements juce::KeyListener::keyPressed()
 *
 * @details This is used to check for a 'delete' key press, toggling the delete point mode if the graph is editable
 */
template <typename ValueType>
bool GraphComponent<ValueType>::keyPressed(const juce::KeyPress& key, juce::Component* /*originatingComponent*/)
{
    if (editable && key.isKeyCode(juce::KeyPress::backspaceKey))
    {
        pointEditState
            = (pointEditState == PointEditingState::Delete) ? PointEditingState::None : PointEditingState::Delete;

        updateCursor();
        return true;
    }

    return false;
}

/**
 * @brief   Implements juce::KeyListener::keyPressed()
 *
 * @note    This second definition is needed to prevent -Woverloaded-virtual
 */
template <typename ValueType>
bool GraphComponent<ValueType>::keyPressed(const juce::KeyPress& key)
{
    return keyPressed(key, nullptr);
}

/**
 * @brief Updates the cursor based on the point editing state
 */
template <typename ValueType>
void GraphComponent<ValueType>::updateCursor()
{
    switch (pointEditState)
    {
    case PointEditingState::Delete:
        setMouseCursor(
            juce::MouseCursor(juce::ImageCache::getFromMemory(BinaryData::Delete_png, BinaryData::Delete_pngSize),
                              1,
                              7,
                              5));
        break;

    case PointEditingState::Move:
    case PointEditingState::OverPoint:
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        break;

    case PointEditingState::Create:
    case PointEditingState::None:
    default:
        setMouseCursor(juce::MouseCursor::CrosshairCursor);
        break;
    }
}

/**
 * @brief Allows the points on the graph to be edited by mouse events
 */
template <typename ValueType>
void GraphComponent<ValueType>::setEditable(bool shouldBeEditable)
{
    editable = shouldBeEditable;
    setInterceptsMouseClicks(shouldBeEditable, shouldBeEditable);
}

/**
 * @brief Returns whether or not the graph is editable
 */
template <typename ValueType>
bool GraphComponent<ValueType>::isEditable() const
{
    return editable;
}

/**
 * @brief   Call this when the points have changed (in derived class)
 */
template <typename ValueType>
void GraphComponent<ValueType>::pointsChanged()
{
    interpolator->invalidateCache();
    recalculateInterpolatedPath();
    repaint();
}

/**
 * @brief   Re-calculates the interpolated path
 */
template <typename ValueType>
void GraphComponent<ValueType>::recalculateInterpolatedPath()
{
    auto bounds = getLocalBounds().toFloat();
    interpolatedPath.clear();

    if (points.size() > 1)
    {
        auto start = transformPointForPaint(bounds, points.getFirst()).toFloat();
        interpolatedPath.startNewSubPath(start.getX(), start.getY());

        interpolator->process(points, 500);

        for (const auto& point : interpolator->getInterpolatedPoints())
        {
            auto transformedPoint = transformPointForPaint(bounds, point);
            interpolatedPath.lineTo(transformedPoint.toFloat());
        }
    }
}

/**
 * @brief       Paint graph ticks
 *
 * @param[in]   g   JUCE graphics context
 */
template <typename ValueType>
void GraphComponent<ValueType>::paintTicks(juce::Graphics& g) const
{
    const int numTicksX = 40;
    const int numTicksY = 20;

    auto bounds = getLocalBounds().toFloat();

    g.setColour(tickColour);

    for (int i = 0; i < numTicksX; i++)
    {
        int x = i * getWidth() / numTicksX;
        g.drawVerticalLine(x, 0, bounds.getHeight());
    }

    for (int i = 0; i < numTicksY; i++)
    {
        int y = i * getHeight() / numTicksY;
        g.drawHorizontalLine(y, 0, bounds.getWidth());
    }
}

/**
 * @brief       Paint graph border
 *
 * @param[in]   g   JUCE graphics context
 */
template <typename ValueType>
void GraphComponent<ValueType>::paintBorder(juce::Graphics& g) const
{
    g.setColour(borderColour);
    g.drawRect(0, 0, getWidth(), getHeight(), 1);
}

/**
 * @brief       Paint graph points
 *
 * @param[in]   g   JUCE graphics context
 */
template <typename ValueType>
void GraphComponent<ValueType>::paintPoints(juce::Graphics& g) const
{
    auto bounds = getLocalBounds().toFloat();

    const float circleSize = 4;
    const float circleShift = circleSize / 2;

    g.setColour(pointColour);

    for (const auto& point : points)
    {
        auto transformedPoint = transformPointForPaint(bounds, point).toFloat();

        const auto x = static_cast<float>(transformedPoint.getX() - circleShift);
        const auto y = static_cast<float>(transformedPoint.getY() - circleShift);

        g.drawEllipse(x, y, circleSize, circleSize, circleSize);
    }
}

/**
 * @brief       Paint graph curve
 *
 * @param[in]   g   JUCE graphics context
 */
template <typename ValueType>
void GraphComponent<ValueType>::paintCurve(juce::Graphics& g) const
{
    if (points.size() < 2)
    {
        return;
    }

    g.setColour(lineColour);
    g.strokePath(interpolatedPath, juce::PathStrokeType(1));
}

/**
 * @brief       Transforms a graph point to the coordinates system used for painting
 *
 * @param[in]
 * @param[in]   point   Point on the graph
 */
template <typename ValueType>
juce::Point<int> GraphComponent<ValueType>::transformPointForPaint(const juce::Rectangle<float>& bounds,
                                                                   const juce::Point<ValueType>& point) const
{
    const float xScale = bounds.getWidth() / static_cast<float>(valueBounds.getWidth());
    const float yScale = bounds.getHeight() / static_cast<float>(valueBounds.getHeight());
    const auto fPoint = point.toFloat();

    auto x = static_cast<int>(fPoint.getX() * xScale);
    auto y = static_cast<int>(bounds.getHeight() - fPoint.getY() * yScale);

    return {x, y};
}

/**
 * @brief       Transforms a GUI point to the coordinates system of the graph
 *
 * @details     Use this in combination with mouse events to let the user add points to the graph
 *
 * @param[in]   point   The point to transform
 */
template <typename ValueType>
juce::Point<ValueType> GraphComponent<ValueType>::transformPointToGraph(const juce::Point<int>& point) const
{
    const auto xScale = static_cast<double>(getMaxX()) / getWidth();
    const auto yScale = static_cast<double>(getMaxY()) / getHeight();

    auto x = static_cast<ValueType>(point.getX() * xScale);
    auto y = getMaxY() - static_cast<ValueType>(point.getY() * yScale);

    x = utility::clip(x, getMinX(), getMaxX());
    y = utility::clip(y, getMinY(), getMaxY());

    return {x, y};
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
template <typename ValueType>
int GraphComponent<ValueType>::getPointNearMouseEvent(const juce::MouseEvent& event) const
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
 * @brief       Checks if a point in the GUI is equivalent to a point on the graph
 *
 * @param[in]   guiPoint        Point in the component's coordinate system
 * @param[in]   graphPoint      Point in the graph's coordinate system
 *
 * @return      true            The component point is equivalent to the graph point
 * @return      false           The component point is not equivalent to the graph point
 */
template <typename ValueType>
bool GraphComponent<ValueType>::pointHitTest(const juce::Point<int>& guiPoint,
                                             const juce::Point<ValueType>& graphPoint) const
{
    const int clickRadius = 10;

    auto transformedPoint = transformPointForPaint(getLocalBounds().toFloat(), graphPoint);
    int distance = transformedPoint.getDistanceFrom(guiPoint);

    return distance < clickRadius;
}

} // namespace gui