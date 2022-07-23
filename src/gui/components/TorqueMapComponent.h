/******************************************************************************
 * @file   TorqueMapComponent.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Component for drawing the torque map
 *****************************************************************************/

#pragma once

#include "../../ConfigurationValueTree.h"
#include "../appearance/Colours.h"
#include "GraphComponent.h"
#include <JuceHeader.h>
#include <memory>

namespace gui
{

class TorqueMapComponent : public GraphComponent<int>, juce::ValueTree::Listener
{
public:

    TorqueMapComponent(std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree);

    void paint(juce::Graphics& g) override;

    // mouse events
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

    // configuration change handlers
    void valueTreePropertyChanged(juce::ValueTree& changedTree, const juce::Identifier& property) override;
    void valueTreeRedirected(juce::ValueTree& redirectedTree) override;

private:

    // deadzone drawing
    juce::Rectangle<int> getDeadzoneBounds() const;
    void paintDeadzoneOverlay(juce::Graphics& g) const;
    bool mouseEventInDeadzone(const juce::MouseEvent& event) const;
    bool shouldPreventPointEdit(const juce::MouseEvent& event) const;
    void showDeadzoneTooltip();
    void hideDeadzoneTooltip();

    int deadzonePosition = 0;
    bool movingDeadzone = false;
    std::unique_ptr<juce::TooltipWindow> deadzoneTooltip;

    // torque map
    void loadTorqueMapData();
    void syncTorqueMapData();

    std::shared_ptr<ConfigurationValueTree> configValueTree;

    // constant data
    // TODO: this needs to be part of the torque map
    static const int inputResolution = 10;
    static const int outputResolution = 15;
    static const int inputMax = (1 << inputResolution) - 1;
    static const int outputMax = (1 << outputResolution) - 1;

    const juce::Colour deadzoneColour = sufst::Colours::skyblue;
};

} // namespace gui
