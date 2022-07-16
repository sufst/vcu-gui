/******************************************************************************
 * @file   TorqueMapComponent.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Component for drawing the torque map
 *****************************************************************************/

#include "TorqueMapComponent.h"

#include "../utility/clip.h"

namespace gui
{

/**
 * @brief Constructor
 */
TorqueMapComponent::TorqueMapComponent(VCUConfiguration& config) : deadzonePosition(defaultDeadzone)
{
    torqueMapData = config.getTorqueMap();

    setRangeX(0, inputMax);
    setRangeY(0, outputMax);

    addPoint(defaultDeadzone, 0);
    addPoint(inputMax, outputMax);
}

/**
 * @brief Overrides GraphComponent<int>::paint()
 */
void TorqueMapComponent::paint(juce::Graphics& g)
{
    GraphComponent<int>::paint(g);

    paintDeadzoneOverlay(g);
}

/**
 * @brief Returns the bounds of the deadzone
 */
juce::Rectangle<int> TorqueMapComponent::getDeadzoneBounds() const
{
    auto deadzoneEdgePoint = transformPointForPaint(getLocalBounds().toFloat(), {deadzonePosition, getHeight()});
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

    g.setColour(juce::Colours::skyblue.withLightness(0.9f).withAlpha(0.2f));
    g.fillRect(deadzoneBounds.expanded(0, 5));

    g.setColour(juce::Colours::skyblue);
    g.drawVerticalLine(deadzoneBounds.getWidth(), 0, deadzoneBounds.getHeight());
}

/**
 * @brief   Overrides GraphComponent::mouseDown()
 *
 * @details Mouse events are intercepted if in the deadzone and forwarded otherwise
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
 * @details Mouse events are intercepted if in the deadzone and forwarded otherwise
 */
void TorqueMapComponent::mouseUp(const juce::MouseEvent& event)
{
    if (mouseEventInDeadzone(event))
    {
        movingDeadzone = false;
    }
    else if (!shouldPreventPointEdit(event))
    {
        GraphComponent<int>::mouseUp(event);
    }
}

/**
 * @brief   Overrides GraphComponent::mouseDrag()
 *
 * @details Mouse events are intercepted if in the deadzone and forwarded otherwise
 */
void TorqueMapComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (mouseEventInDeadzone(event) || movingDeadzone)
    {
        if (movingDeadzone)
        {
            jassert(points.size() >= 2);

            auto newDeadzonePosition = transformPointToGraph(event.getPosition()).getX();
            auto maxDeadzonePosition = points[1].getX(); // don't allow deadzone to pass 2nd graph point

            deadzonePosition = utility::clip(newDeadzonePosition, 0, maxDeadzonePosition);
            points.getReference(0).setX(deadzonePosition);

            repaint();
        }

        showDeadzoneTooltip();
    }
    else if (!shouldPreventPointEdit(event))
    {
        GraphComponent<int>::mouseDrag(event);
    }
}

/**
 * @brief   Overrides GraphComponent::mouseMove()
 *
 * @details Mouse events are intercepted if in the deadzone and forwarded otherwise
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
        GraphComponent<int>::mouseMove(event);
    }
}

/**
 * @brief       Checks if a mouse event is in the deadzone
 *
 * @param[in]   event   The mouse event
 */
bool TorqueMapComponent::mouseEventInDeadzone(const juce::MouseEvent& event) const
{
    const int deadzoneEdgeOffset = 2;
    auto point = transformPointToGraph(event.getPosition());
    return point.getX() < deadzonePosition + deadzoneEdgeOffset;
}

/**
 * @brief       Checks if the mouse event is near a point which should not be edited
 *
 * @details     The first and last points should not be edited
 *
 * @param[in]   event   The mouse event
 */
bool TorqueMapComponent::shouldPreventPointEdit(const juce::MouseEvent& event) const
{
    return (pointHitTest(event.getPosition(), points.getFirst())
            || pointHitTest(event.getPosition(), points.getLast()));
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

    auto deadzoneX = transformPointForPaint(getLocalBounds().toFloat(), {deadzonePosition, 0}).getX();

    int tipX = this->getScreenX() + deadzoneX - 10;
    int tipY = juce::Desktop::getMousePosition().getY();

    juce::String tipText
        = juce::String::toDecimalStringWithSignificantFigures(100 * static_cast<float>(deadzonePosition) / inputMax, 2)
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
