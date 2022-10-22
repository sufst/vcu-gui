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
 */
class TorqueMapComponent : public GraphComponent<int>, juce::ValueTree::Listener
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
    void valueTreePropertyChanged(juce::ValueTree& changedTree,
                                  const juce::Identifier& property) override;
    void valueTreeRedirected(juce::ValueTree& redirectedTree) override;

private:

    //==========================================================================
    juce::Rectangle<int> getDeadzoneBounds() const;
    void paintDeadzoneOverlay(juce::Graphics& g) const;
    bool mouseEventInDeadzone(const juce::MouseEvent& event) const;
    bool shouldPreventPointEdit(const juce::MouseEvent& event) const;
    void showDeadzoneTooltip();
    void hideDeadzoneTooltip();

    int deadzonePosition = 0;
    bool movingDeadzone = false;
    std::unique_ptr<juce::TooltipWindow> deadzoneTooltip;

    //==========================================================================
    void loadTorqueMapData();
    void syncTorqueMapData();

    //==========================================================================
    TorqueMap torqueMap;

    //==========================================================================
    const juce::Colour deadzoneColour = sufst::Colours::skyblue;
};

} // namespace gui
