/******************************************************************************
 * @file   TorqueMapComponent.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Component for drawing the torque map
 *****************************************************************************/

#include "TorqueMapComponent.h"

#include "../../Application.h"
#include "../utility/clip.h"

namespace gui
{

/**
 * @brief Constructor
 */
TorqueMapComponent::TorqueMapComponent(std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree)
    : configValueTree(sharedConfigValueTree)
{
    configValueTree->addListener(this);

    setRangeX(0, inputMax);
    setRangeY(0, outputMax);

    loadTorqueMapData();
}

/**
 * @brief Loads torque map data from the value tree
 */
void TorqueMapComponent::loadTorqueMapData()
{
    // interpolation method
    auto torqueMap = configValueTree->getChildWithName(ConfigurationValueTree::Children::TorqueMap);

    setInterpolationMethod(torqueMap.getProperty(ConfigurationValueTree::Properties::InterpolationMethod).toString());

    // data points
    clear();

    int numChildren = torqueMap.getNumChildren();

    for (int i = 0; i < numChildren; i++)
    {
        const auto& child = torqueMap.getChild(i);

        if (child.hasType(ConfigurationValueTree::Children::TorqueMapPoint))
        {
            int input = child.getProperty(ConfigurationValueTree::Properties::InputValue);
            int output = child.getProperty(ConfigurationValueTree::Properties::OutputValue);

            addPoint({input, output});
        }
    }

    // deadzone
    deadzonePosition = points.getFirst().getX();
}

/**
 * @brief Updates the torque map when the points on the graph change
 */
void TorqueMapComponent::syncTorqueMapData()
{
    // TODO: this function isn't called that often, but rewriting the entire set of points is not a very efficient way
    //       to update the tree
    auto torqueMap = configValueTree->getChildWithName(ConfigurationValueTree::Children::TorqueMap);
    torqueMap.removeAllChildren(nullptr);

    for (const auto& point : points)
    {
        torqueMap.addChild(ConfigurationValueTree::createTorqueMapPoint(point.getX(), point.getY()), -1, nullptr);
    }
}

/**
 * @brief Overrides GraphComponent<int>::paint()
 */
void TorqueMapComponent::paint(juce::Graphics& g)
{
    GraphComponent<int>::paintTicks(g);
    GraphComponent<int>::paintBorder(g);
    paintScaledCurve(g);
    GraphComponent<int>::paintCurve(g);
    GraphComponent<int>::paintPoints(g);
    paintDeadzoneOverlay(g);
}

/**
 * @brief Paints a scaled version of the graph curve
 */
void TorqueMapComponent::paintScaledCurve(juce::Graphics& g)
{
    if (points.size() < 2)
    {
        return;
    }

    // scale path by affine transform
    auto pathBounds = interpolatedPath.getBounds();
    const float xShift = pathBounds.getWidth();
    const float yShift = pathBounds.getHeight();

    juce::Path path = interpolatedPath; // TODO: another level of caching here?
    path.applyTransform(juce::AffineTransform().translated(-xShift, -yShift));
    path.applyTransform(juce::AffineTransform().scaled(1, scaleFactor));
    path.applyTransform(juce::AffineTransform().translated(xShift, yShift));

    // draw scaled path
    g.setColour(scaledLineColour);
    g.strokePath(path, juce::PathStrokeType(1));
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

    g.setColour(deadzoneColour.withLightness(0.5f).withAlpha(0.2f));
    g.fillRect(deadzoneBounds.expanded(0, 5));

    g.setColour(deadzoneColour);
    g.drawVerticalLine(deadzoneBounds.getWidth(), 0, deadzoneBounds.toFloat().getHeight());
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
        syncTorqueMapData();
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

            pointsChanged();
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

/**
 * @brief Implements juce::ValueTree::Listener::valueTreeRedirected()
 */
void TorqueMapComponent::valueTreeRedirected(juce::ValueTree& /*redirectedTree*/)
{
    loadTorqueMapData();
}

/**
 * @brief Implements juce::ValueTree::Listener::valueTreePropertyChanged()
 */
void TorqueMapComponent::valueTreePropertyChanged(juce::ValueTree& changedTree, const juce::Identifier& property)
{
    auto torqueMap = configValueTree->getChildWithName(ConfigurationValueTree::Children::TorqueMap);

    if (changedTree == torqueMap)
    {
        if (property == ConfigurationValueTree::Properties::InterpolationMethod)
        {
            setInterpolationMethod(
                torqueMap.getProperty(ConfigurationValueTree::Properties::InterpolationMethod).toString());
            repaint();
        }
        else if (property == ConfigurationValueTree::Properties::ScaleFactor)
        {
            auto scaleFactorProperty = torqueMap.getProperty(ConfigurationValueTree::Properties::ScaleFactor);
            scaleFactor = static_cast<float>(scaleFactorProperty);
            repaint();
        }
    }
}

} // namespace gui
