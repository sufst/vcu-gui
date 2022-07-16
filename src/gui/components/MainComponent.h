/******************************************************************************
 * @file   MainComponent.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Main GUI component
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

#include "../../Application.h"
#include "TorqueMapComponent.h"

namespace gui
{

/**
 * @brief Main GUI component
 */
class MainComponent : public juce::Component
{
public:

    // constructor / destructor
    MainComponent();
    ~MainComponent() override;

    // graphics
    void paint(juce::Graphics&) override;
    void resized() override;

private:

    void setupInterpolationCombo();
    void setupButtons();

    juce::ValueTree torqueMap;
    std::unique_ptr<juce::FileChooser> fileChooser;

    // constants
    static const int borderSize = 20;

    // children
    TorqueMapComponent torqueMapGraph;
    juce::ComboBox interpolationCombo;
    juce::TextButton exportProfileButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace gui