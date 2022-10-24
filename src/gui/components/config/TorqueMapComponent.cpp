/******************************************************************************
 * @file   TorqueMapComponent.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Component for drawing the torque map
 *****************************************************************************/

#include "TorqueMapComponent.h"

#include "../../../Application.h"
#include "../../../utility/clip.h"

namespace gui
{

//==============================================================================

/**
 * @brief Default constructor
 */
TorqueMapComponent::TorqueMapComponent(juce::ValueTree torqueMapTree)
    : torqueMap(torqueMapTree)
{
    jassert(torqueMapTree.isValid());

    setRangeX({TorqueMapPoint::MinInput, TorqueMapPoint::MaxInput});
    setRangeY({TorqueMapPoint::MinOutput, TorqueMapPoint::MaxOutput});
    setTickSpacing(TorqueMapPoint::MaxInput / 10,
                   TorqueMapPoint::MaxOutput / 10);
    setInterpolationMethod(
        juce::Identifier(torqueMap.interpolationMethod.get()));

    // TODO: this should really use a juce::ChangeListener instead of listening
    //       to the value tree directly
    torqueMap.state.addListener(this);
}

//==============================================================================

/**
 * @brief   Returns the number of points in the torque map
 */
int TorqueMapComponent::getNumPoints() const
{
    return torqueMap.objects.size();
}

/**
 * @brief   Returns the point at the specified index
 *
 * @param   index   Index of point
 */
TorqueMapComponent::PointType TorqueMapComponent::getPoint(int index) const
{
    // TODO: accessing the objects directly is perhaps not ideal?
    auto* point = torqueMap.objects[index];
    return PointType(point->input.get(), point->output.get());
}

/**
 * @brief   Moves the point at the specified index to a new position
 *
 * @param   index           Index of point to move
 * @param   newPosition     New position to put the point
 *
 * @return  New index of the moved point, if it was swapped with another
 */
int TorqueMapComponent::movePoint(int index, PointType newPosition)
{
    // TODO: handle swapping of points
    auto* pointToMove = torqueMap.getPoints()[index];
    pointToMove->input = newPosition.getX();
    pointToMove->output = newPosition.getY();

    // check if point has moved past the x-coordinate of another point
    // swap them if this is the case
    // TODO: this is potentially inefficient
    // TODO: also don't like that torqueMap.objects is accessed directly
    int newIndex = index;

    if (index != 0 && getPoint(index - 1).getX() > getPoint(index).getX())
    {
        torqueMap.getPoints().swap(index, index - 1);
        newIndex = index - 1;
    }
    else if (index != getNumPoints() - 1
             && getPoint(index).getX() > getPoint(index + 1).getX())
    {
        torqueMap.getPoints().swap(index, index + 1);
        newIndex = index + 1;
    }

    return newIndex;
    ;
}

/**
 * @brief   Adds a point to the torque map
 *
 * @param   newPoint Point to be added
 */
void TorqueMapComponent::addPoint(PointType newPoint)
{
    torqueMap.addPoint(newPoint.getX(), newPoint.getY());
}

/**
 * @brief   Removes a point from the torque map
 *
 * @param   index   Index of point to remove
 */
void TorqueMapComponent::removePoint(int index)
{
    torqueMap.removePoint(*torqueMap.getPoints()[index]);
}

//==============================================================================

/**
 * @brief Overrides GraphComponent::paint()
 */
void TorqueMapComponent::paint(juce::Graphics& g)
{
    GraphComponent<TorqueMapPoint::ValueType>::paint(g);
    paintDeadzoneOverlay(g);
}

//==============================================================================

/**
 * @brief   Reloads the torque map if the data structure changes
 */
void TorqueMapComponent::valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&)
{
    pointsChanged();
}

/**
 * @brief   Reloads the torque map if the data structure changes
 */
void TorqueMapComponent::valueTreeChildRemoved(juce::ValueTree&,
                                               juce::ValueTree&,
                                               int)
{
    pointsChanged();
}

//==============================================================================

/**
 * @brief   Returns the x position of the deadzone
 */
int TorqueMapComponent::getDeadzonePosition() const
{
    return getPoint(0).getX();
}

/**
 * @brief   Sets the x position of the deadzone
 *
 */
void TorqueMapComponent::setDeadzonePosition(int newPosition)
{
    movePoint(0, {newPosition, getPoint(0).getY()});
    pointsChanged();
}

/**
 * @brief Returns the bounds of the deadzone
 */
juce::Rectangle<int> TorqueMapComponent::getDeadzoneBounds() const
{
    auto deadzoneEdgePoint
        = transformPointForPaint({getDeadzonePosition(), getHeight()});
    return juce::Rectangle<int>({0, 0}, deadzoneEdgePoint);
}

/**
 * @brief       Paints the deadzone overlay
 *
 * @param[in]   g   JUCE graphics context
 */
void TorqueMapComponent::paintDeadzoneOverlay(juce::Graphics& g) const
{
    auto deadzoneBounds = getDeadzoneBounds();

    g.setColour(deadzoneColour.withLightness(0.5f).withAlpha(0.2f));
    g.fillRect(deadzoneBounds.expanded(0, 5));

    g.setColour(deadzoneColour);
    g.drawVerticalLine(deadzoneBounds.getWidth(),
                       0,
                       deadzoneBounds.toFloat().getHeight());
}

//==============================================================================

/**
 * @brief   Overrides GraphComponent::mouseDown()
 *
 * @details Mouse events are intercepted if in the deadzone and forwarded
 *          otherwise
 */
void TorqueMapComponent::mouseDown(const juce::MouseEvent& event)
{
    if (mouseEventInDeadzone(event))
    {
        if (!movingDeadzone)
        {
            movingDeadzone = true;
        }
    }
    else if (!shouldPreventPointEdit(event))
    {
        GraphComponent<int>::mouseDown(event);
    }
}

/**
 * @brief   Overrides GraphComponent::mouseUp()
 *
 * @details Mouse events are intercepted if in the deadzone and forwarded
 *          otherwise
 */
void TorqueMapComponent::mouseUp(const juce::MouseEvent& event)
{
    if (mouseEventInDeadzone(event))
    {
        movingDeadzone = false;
    }
    else if (!shouldPreventPointEdit(event))
    {
        GraphComponent<TorqueMapPoint::ValueType>::mouseUp(event);
    }
}

/**
 * @brief   Overrides GraphComponent::mouseDrag()
 *
 * @details Mouse events are intercepted if in the deadzone and forwarded
 *          otherwise
 */
void TorqueMapComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (mouseEventInDeadzone(event) || movingDeadzone)
    {
        if (movingDeadzone)
        {
            jassert(getNumPoints() >= 2);

            auto newDeadzonePosition
                = transformPointToGraph(event.getPosition()).getX();
            auto maxDeadzonePosition
                = getPoint(1).getX(); // don't allow deadzone to pass 2nd graph

            setDeadzonePosition(juce::Range(0, maxDeadzonePosition)
                                    .clipValue(newDeadzonePosition));
            pointsChanged();
        }

        showDeadzoneTooltip();
    }
    else if (!shouldPreventPointEdit(event))
    {
        GraphComponent<TorqueMapPoint::ValueType>::mouseDrag(event);
    }
}

/**
 * @brief   Overrides GraphComponent::mouseMove()
 *
 * @details Mouse events are intercepted if in the deadzone and forwarded
 *          otherwise
 */
void TorqueMapComponent::mouseMove(const juce::MouseEvent& event)
{
    if (mouseEventInDeadzone(event))
    {
        setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
        showDeadzoneTooltip();
    }
    else if (!shouldPreventPointEdit(event))
    {
        hideDeadzoneTooltip();
        GraphComponent<TorqueMapPoint::ValueType>::mouseMove(event);
    }
}

/**
 * @brief       Checks if a mouse event is in the deadzone
 *
 * @param[in]   event   The mouse event
 */
bool TorqueMapComponent::mouseEventInDeadzone(
    const juce::MouseEvent& event) const
{
    const int deadzoneEdgeOffset = 2;
    auto point = transformPointToGraph(event.getPosition());
    return point.getX() < getDeadzonePosition() + deadzoneEdgeOffset;
}

/**
 * @brief       Checks if the mouse event is near a point which should not be
 *              edited
 *
 * @details     The first and last points should not be edited
 *
 * @param[in]   event   The mouse event
 */
bool TorqueMapComponent::shouldPreventPointEdit(
    const juce::MouseEvent& event) const
{
    return (pointHitTest(event.getPosition(), getPoint(0))
            || pointHitTest(event.getPosition(), getPoint(getNumPoints() - 1)));
}

/**
 * @brief Shows the deadzone tooltip
 */
void TorqueMapComponent::showDeadzoneTooltip()
{
    if (deadzoneTooltip == nullptr)
    {
        deadzoneTooltip = std::make_unique<juce::TooltipWindow>(this, 0);
    }

    auto deadzoneX = transformPointForPaint({getDeadzonePosition(), 0}).getX();

    int tipX = this->getScreenX() + deadzoneX - 10;
    int tipY = juce::Desktop::getMousePosition().getY();

    juce::String tipText = juce::String::toDecimalStringWithSignificantFigures(
                               100 * static_cast<float>(getDeadzonePosition())
                                   / TorqueMapPoint::MaxInput,
                               2)
                           + "%";

    deadzoneTooltip->displayTip({tipX, tipY}, tipText);
    deadzoneTooltip->setVisible(true);
}

/**
 * @brief Hide the deadzone tooltip
 */
void TorqueMapComponent::hideDeadzoneTooltip()
{
    deadzoneTooltip.reset();
}

} // namespace gui