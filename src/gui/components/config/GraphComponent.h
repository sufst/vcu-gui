/******************************************************************************
 * @file   GraphComponent.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @date   2022-07-11
 * @brief  Component for drawing a graph
 *****************************************************************************/

#pragma once

#include "../../../utility/Interpolator.h"
#include "../../../utility/PointComparator.h"
#include "../../../utility/clip.h"
#include "../../appearance/Colours.h"
#include <JuceHeader.h>
#include <memory>

namespace gui
{

using utility::Interpolator; // TODO: does this pollute the namespace?
using utility::InterpolatorFactory;
using utility::SplineInterpolator;

/**
 * @brief   A base component for drawing points on a graph
 *
 * @details The data model is left to derived classes for flexibility
 *
 * @tparam  ValueType   Data type for points on the graph
 */
template <typename ValueType>
class GraphComponent : public juce::Component, public juce::KeyListener
{
public:

    using PointType = juce::Point<ValueType>;

    //==========================================================================
    GraphComponent();

    //==========================================================================
    void setRangeX(juce::Range<ValueType> range);
    void setRangeY(juce::Range<ValueType> range);
    void setTickSpacing(ValueType xSpacing, ValueType ySpacing);
    void setInterpolationMethod(const juce::Identifier& identifier);

    ValueType getMinX() const;
    ValueType getMaxX() const;
    ValueType getMinY() const;
    ValueType getMaxY() const;

protected:

    //==========================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void updateCursor();

    bool keyPressed(const juce::KeyPress& key,
                    juce::Component* originatingComponent) override;
    bool keyPressed(const juce::KeyPress& key) override;

    void setDeletionState(bool enabled);

    //==========================================================================
    virtual int getNumPoints() const = 0;
    virtual PointType getPoint(int index) const = 0;
    virtual int movePoint(int index, PointType newPosition) = 0;
    virtual void addPoint(PointType newPoint) = 0;
    virtual void removePoint(int index) = 0;

    //==========================================================================
    juce::AffineTransform graphToGuiTransform() const;
    juce::AffineTransform guiToGraphTransform() const;
    juce::Point<int> transformPointForPaint(const PointType& point) const;
    juce::Point<int> transformPointToGraph(const PointType& point) const;

    void pointsChanged();

    //==========================================================================
    void paintBorder(juce::Graphics& g) const;
    void paintTicks(juce::Graphics& g) const;
    void paintPoints(juce::Graphics& g) const;
    void paintCurve(juce::Graphics& g) const;

    void recalculateInterpolatedPath();

    bool pointHitTest(const juce::Point<int>& guiPoint,
                      const juce::Point<ValueType>& graphPoint) const;
    int getPointNearMouseEvent(const juce::MouseEvent& event) const;

    //==========================================================================
    ValueType tickSpacingX = 1;
    ValueType tickSpacingY = 1;

    const PointType origin = {0, 0};
    PointType topRight = {1, 1};
    PointType bottomLeft = {-1, 1};

    juce::Path interpolatedPath;
    std::unique_ptr<Interpolator<ValueType>> interpolator = nullptr;

    //==========================================================================
    enum class PointEditingState
    {
        None,
        OverPoint,
        Create,
        Move,
        Delete,
    };

    PointEditingState pointEditState = PointEditingState::None;
    int movingPointIndex = -1;
    static constexpr int clickRadius = 10;

    //==========================================================================
    const juce::Colour pointColour = sufst::Colours::sfsyellow;
    const juce::Colour lineColour = sufst::Colours::white;
    const juce::Colour borderColour = sufst::Colours::midgrey;
    const juce::Colour tickColour = sufst::Colours::darkgrey;
};

//==============================================================================

/**
 * @brief   Default constructor
 */
template <typename ValueType>
GraphComponent<ValueType>::GraphComponent()
{
    setSize(100, 100); // this prevents infinite affine transforms
    setWantsKeyboardFocus(true);
    addKeyListener(this);
}

//==============================================================================

/**
 * @brief   Sets the range of the x-axis
 *
 * @param   range   Range [min, max]
 */
template <typename ValueType>
void GraphComponent<ValueType>::setRangeX(juce::Range<ValueType> range)
{
    bottomLeft.setX(range.getStart());
    topRight.setX(range.getEnd());
}

/**
 * @brief   Sets the range of the y-axis
 *
 * @param   range   Range [min, max]
 */
template <typename ValueType>
void GraphComponent<ValueType>::setRangeY(juce::Range<ValueType> range)
{
    bottomLeft.setY(range.getStart());
    topRight.setY(range.getEnd());
}

/**
 * @brief   Sets the spacing between the ticks
 *
 * @param   xSpacing    Spacing between the x ticks
 * @param   ySpacing    Spacing between the y ticks
 */
template <typename ValueType>
void GraphComponent<ValueType>::setTickSpacing(ValueType xSpacing,
                                               ValueType ySpacing)
{
    tickSpacingX = xSpacing;
    tickSpacingY = ySpacing;
}

/**
 * @brief       Changes the interpolation method
 *
 * @param[in]   identifier     The new interpolator to use
 */
template <typename ValueType>
void GraphComponent<ValueType>::setInterpolationMethod(
    const juce::Identifier& identifier)
{
    interpolator = InterpolatorFactory<ValueType>::makeInterpolator(identifier);
    jassert(interpolator);
    pointsChanged();
}

/**
 * @brief   Returns the minimum value of the X axis
 */
template <typename ValueType>
ValueType GraphComponent<ValueType>::getMinX() const
{
    return bottomLeft.getX();
}

/**
 * @brief   Returns the maximum value of the X axis
 */
template <typename ValueType>
ValueType GraphComponent<ValueType>::getMaxX() const
{
    return topRight.getX();
}

/**
 * @brief   Returns the minimum value of the Y axis
 */
template <typename ValueType>
ValueType GraphComponent<ValueType>::getMinY() const
{
    return bottomLeft.getY();
}

/**
 * @brief   Returnsthe maximum value of the Y axis
 */
template <typename ValueType>
ValueType GraphComponent<ValueType>::getMaxY() const
{
    return topRight.getY();
}

//==============================================================================

/**
 * @brief   Implements juce::Component::paint()
 */
template <typename ValueType>
void GraphComponent<ValueType>::paint(juce::Graphics& g)
{
    paintBorder(g);
    paintTicks(g);

    if (interpolator)
    {
        paintCurve(g);
    }

    paintPoints(g);
}

/**
 * @brief       Paints the graph border
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
 * @brief       Paints the graph ticks
 *
 * @param[in]   g   JUCE graphics context
 */
template <typename ValueType>
void GraphComponent<ValueType>::paintTicks(juce::Graphics& g) const
{
    // ticks
    g.setColour(tickColour);

    typedef struct
    {
        int numTicks;
        enum
        {
            pos,
            neg
        } direction;
        enum
        {
            y,
            x
        } axis;
    } TickParams;

    std::initializer_list<TickParams> tickParamsList = {
        {std::abs(getMaxX()) / tickSpacingX, TickParams::pos, TickParams::x},
        {std::abs(getMinX()) / tickSpacingX, TickParams::neg, TickParams::x},
        {std::abs(getMaxY()) / tickSpacingY, TickParams::pos, TickParams::y},
        {std::abs(getMinY()) / tickSpacingY, TickParams::neg, TickParams::y}};

    for (const auto& params : tickParamsList)
    {
        for (int i = 0; i <= params.numTicks; i++)
        {
            auto dirMult = (params.direction == TickParams::pos) ? 1 : -1;

            if (params.axis == TickParams::x)
            {
                int x = i * dirMult * tickSpacingX;
                auto point = transformPointForPaint({x, 0});
                g.drawVerticalLine(point.getX(),
                                   0.f,
                                   static_cast<float>(getHeight()));
            }
            else
            {
                int y = i * dirMult * tickSpacingY;
                auto point = transformPointForPaint({0, y});
                g.drawHorizontalLine(point.getY(),
                                     0.f,
                                     static_cast<float>(getWidth()));
            }
        }
    }

    // x/y axes
    PointType centreLeft = transformPointForPaint({getMinX(), 0});
    PointType topCentre = transformPointForPaint({0, getMaxY()});

    g.setColour(borderColour);
    g.drawVerticalLine(topCentre.getX(), 0.f, static_cast<float>(getHeight()));
    g.drawHorizontalLine(centreLeft.getY(),
                         0.f,
                         static_cast<float>(getWidth()));
}

/**
 * @brief       Paint graph points
 *
 * @param[in]   g   JUCE graphics context
 */
template <typename ValueType>
void GraphComponent<ValueType>::paintPoints(juce::Graphics& g) const
{
    const float circleSize = 4;
    const float circleShift = circleSize / 2;

    g.setColour(pointColour);

    for (int i = 0; i < getNumPoints(); i++)
    {
        auto transformedPoint = transformPointForPaint(getPoint(i)).toFloat();

        const auto x
            = static_cast<float>(transformedPoint.getX() - circleShift);
        const auto y
            = static_cast<float>(transformedPoint.getY() - circleShift);

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
    if (getNumPoints() < 2)
    {
        return;
    }

    g.setColour(lineColour);
    g.strokePath(interpolatedPath, juce::PathStrokeType(1));
}

/**
 * @brief   Implements juce::Component::resized()
 *
 * @details This applies an affine transform to the interpolated path to
 *          resize it to the available bounds
 *
 * @note    The component must start with a non-zero size, else the calls to
 *          resized() on app initialisation will result in an invalid (infinite)
 *          transform matrix which throws an exception when it is applied
 */
template <typename ValueType>
void GraphComponent<ValueType>::resized()
{
    pointsChanged();

    if (interpolator && getNumPoints() >= 2)
    {
        auto p1 = transformPointForPaint(getPoint(0));
        auto p2 = transformPointForPaint(getPoint(getNumPoints() - 1));
        juce::Rectangle<int> bounds(p1, p2);

        auto transform
            = interpolatedPath.getTransformToScaleToFit(bounds.toFloat(),
                                                        false);
        interpolatedPath.applyTransform(transform);
    }
}

/**
 * @brief   Call this when the points have changed (in derived class)
 */
template <typename ValueType>
void GraphComponent<ValueType>::pointsChanged()
{
    if (interpolator)
    {
        interpolator->invalidateCache();
        recalculateInterpolatedPath();
    }
    repaint();
}

/**
 * @brief   Re-calculates the interpolated path
 */
template <typename ValueType>
void GraphComponent<ValueType>::recalculateInterpolatedPath()
{
    if (!interpolator)
    {
        return;
    }

    interpolatedPath.clear();

    if (getNumPoints() > 1)
    {
        // compute interpolation
        // TODO: this is pretty expensive, can it be made more efficient?
        juce::Array<PointType> points;

        for (int i = 0; i < getNumPoints(); i++)
        {
            points.add(getPoint(i));
        }

        interpolator->process(points, 500);

        // construct path
        auto start = transformPointForPaint(getPoint(0)).toFloat();
        interpolatedPath.startNewSubPath(start.getX(), start.getY());

        for (const auto& point : interpolator->getInterpolatedPoints())
        {
            auto transformedPoint = transformPointForPaint(point);
            interpolatedPath.lineTo(transformedPoint.toFloat());
        }
    }
}

//==============================================================================

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
    if (pointEditState == PointEditingState::None
        || pointEditState == PointEditingState::OverPoint)
    {
        // create new
        if (pointIndex == -1)
        {
            const auto newPoint = transformPointToGraph(event.getPosition());

            // TODO: add point
            addPoint(newPoint);

            // TODO: this effectively searches for the point that was just
            // added, could be made more efficient
            movingPointIndex = getPointNearMouseEvent(event);
        }
        // move
        else
        {
            movingPointIndex = pointIndex;
        }

        // always move, including after creation
        pointEditState = PointEditingState::Move;
        pointsChanged();
    }

    if (pointEditState == PointEditingState::Delete && pointIndex != -1)
    {
        removePoint(pointIndex);
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
        jassert(movingPointIndex != -1 && movingPointIndex < getNumPoints());
        auto newPosition = transformPointToGraph(event.getPosition());
        movingPointIndex = movePoint(movingPointIndex, newPosition);
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
 * @brief Updates the cursor based on the point editing state
 */
template <typename ValueType>
void GraphComponent<ValueType>::updateCursor()
{
    switch (pointEditState)
    {
    case PointEditingState::Delete:
        setMouseCursor(juce::MouseCursor(
            juce::ImageCache::getFromMemory(BinaryData::Delete_png,
                                            BinaryData::Delete_pngSize),
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

//==============================================================================

/**
 * @brief   Implements juce::KeyListener::keyPressed()
 *
 * @details This is used to check for a 'delete' key press, toggling the delete
 *          point mode if the graph is editable
 */
template <typename ValueType>
bool GraphComponent<ValueType>::keyPressed(
    const juce::KeyPress& key,
    juce::Component* /*originatingComponent*/)
{
    if (key.isKeyCode(juce::KeyPress::backspaceKey))
    {
        setDeletionState(true);
        return true;
    }
    
    if (key.isKeyCode(juce::KeyPress::escapeKey))
    {
        if (pointEditState == PointEditingState::Delete)
        {
            setDeletionState(false);
            return true;
        }
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

template <typename ValueType>
void GraphComponent<ValueType>::setDeletionState(bool enabled)
{
    pointEditState = enabled ? PointEditingState::Delete : PointEditingState::None;
    updateCursor();
}

//==============================================================================

/**
 * @brief   Returns an affine transform representing the transformation from
 *          a point on the graph to a point in the component
 */
template <typename ValueType>
juce::AffineTransform GraphComponent<ValueType>::graphToGuiTransform() const
{
    const float xRange
        = static_cast<float>(getMaxX()) - static_cast<float>(getMinX());
    const float yRange
        = static_cast<float>(getMaxY()) - static_cast<float>(getMinY());
    const float xScale = getWidth() / xRange;
    const float yScale = getHeight() / yRange;

    return juce::AffineTransform()
        .scaled(xScale, -yScale)
        .translated(-xScale * getMinX(), yScale * getMaxY());
}

/**
 * @brief   Returns an affine transform representing the transformation from
 *          a point on the component to a point on the graph
 */
template <typename ValueType>
juce::AffineTransform GraphComponent<ValueType>::guiToGraphTransform() const
{
    return graphToGuiTransform().inverted();
}

/**
 * @brief       Transforms a point on the graph to a point on the juce::Graphics
 *              painting area
 *
 * @details     The coordinates system for the graph is the "normal" system
 *              with the +y going up and +x going right. In the JUCE graphics
 *              system, +y goes down and +x goes right. This requires a
 *              transformation to convert between the two systems.
 *
 * @param[in]   point   Point to transform
 */
template <typename ValueType>
juce::Point<int>
GraphComponent<ValueType>::transformPointForPaint(const PointType& point) const
{
    auto x = point.getX();
    auto y = point.getY();
    graphToGuiTransform().transformPoint(x, y);
    return {x, y};
}

/**
 * @brief       The inverse transform of transformPointForPaint()
 */
template <typename ValueType>
juce::Point<int>
GraphComponent<ValueType>::transformPointToGraph(const PointType& point) const
{
    auto x = point.getX();
    auto y = point.getY();
    guiToGraphTransform().transformPoint(x, y);
    return {x, y};
}

/**
 * @brief       Checks if a mouse event is near a point on the graph,
 returning
 *              the index of the point if it does and -1 otherwise
 *
 * @param[in]   event   Mouse event
 *
 * @return      -1      Mouse event is not near any points
 * @return      >0      Mouse event is near the point with the returned index
 */
template <typename ValueType>
int GraphComponent<ValueType>::getPointNearMouseEvent(
    const juce::MouseEvent& event) const
{
    const auto eventPosition = event.getPosition().toInt();

    for (int i = 0; i < getNumPoints(); i++)
    {
        const auto point = getPoint(i);

        if (pointHitTest(eventPosition, point))
        {
            return i;
        }
    }

    return -1;
}

/**
 * @brief       Checks if a point in the GUI is equivalent to a point on the
 *              graph
 *
 * @param[in]   guiPoint        Point in the component's coordinate system
 * @param[in]   graphPoint      Point in the graph's coordinate system
 *
 * @return      true            The component point is equivalent to the graph
 *                              point
 *
 * @return      false           The component point is not equivalent to the
 *                              graph point
 */
template <typename ValueType>
bool GraphComponent<ValueType>::pointHitTest(
    const juce::Point<int>& guiPoint,
    const juce::Point<ValueType>& graphPoint) const
{
    auto transformedPoint = transformPointForPaint(graphPoint);
    int distance = transformedPoint.getDistanceFrom(guiPoint);

    return distance < clickRadius;
}

} // namespace gui