/******************************************************************************
 * @file   TorqueMapComponent.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Component for drawing the torque map
 *****************************************************************************/

#pragma once

#include "../../../config/TorqueMap.h"
#include "../../appearance/Colours.h"
#include "GraphComponent.h"
#include <JuceHeader.h>
#include <memory>

namespace gui
{

using config::TorqueMap;
using config::TorqueMapPoint;

/**
 * @brief   Component for editing torque map
 *
 *          TODO: what happens when the config is reloaded from file??
 */
class TorqueMapComponent : public GraphComponent<TorqueMapPoint::ValueType>,
                           public juce::ValueTree::Listener
{
public:

    //==========================================================================
    TorqueMapComponent(juce::ValueTree torqueMapTree);

    //==========================================================================
    void paint(juce::Graphics& g) override;

    //==========================================================================
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

    //==========================================================================
    void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override;
    void
    valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override;

private:

    //==========================================================================
    int getNumPoints() const override;
    PointType getPoint(int index) const override;
    int movePoint(int index, PointType newPosition) override;
    void addPoint(PointType newPoint) override;
    void removePoint(int index) override;

    //==========================================================================
    int getDeadzonePosition() const;
    void setDeadzonePosition(int newPosition);
    juce::Rectangle<int> getDeadzoneBounds() const;
    void paintDeadzoneOverlay(juce::Graphics& g) const;
    bool mouseEventInDeadzone(const juce::MouseEvent& event) const;
    bool shouldPreventPointEdit(const juce::MouseEvent& event) const;
    void showDeadzoneTooltip();
    void hideDeadzoneTooltip();

    bool movingDeadzone = false;
    std::unique_ptr<juce::TooltipWindow> deadzoneTooltip;

    //==========================================================================
    TorqueMap torqueMap;

    //==========================================================================
    const juce::Colour deadzoneColour = sufst::Colours::skyblue;
};

} // namespace gui
