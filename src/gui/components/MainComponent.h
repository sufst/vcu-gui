/******************************************************************************
 * @file   MainComponent.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Main GUI component
 *****************************************************************************/

#pragma once

#include "../../Application.h"
#include "TorqueMapComponent.h"
#include <JuceHeader.h>

namespace gui
{

/**
 * @brief Main GUI component
 */
class MainComponent : public juce::Component, public juce::FileDragAndDropTarget, public juce::ValueTree::Listener
{
public:

    // constructor / destructor
    MainComponent(std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree);
    ~MainComponent() override;

    // graphics
    void paint(juce::Graphics&) override;
    void resized() override;

    // file drag and drop
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void fileDragEnter(const juce::StringArray& files, int x, int y) override;
    void fileDragExit(const juce::StringArray& files) override;

    // config value tree
    void valueTreeRedirected(juce::ValueTree& redirectedTree) override;

private:

    void setupInterpolationCombo();
    void setupButtons();

    // shared config state
    std::shared_ptr<ConfigurationValueTree> configValueTree;

    // file import/export
    std::unique_ptr<juce::FileChooser> fileChooser;
    bool fileIsBeingDragged = false;

    // constants
    static const int borderSize = 20;

    // children
    TorqueMapComponent torqueMapGraph;
    juce::ComboBox interpolationCombo;
    juce::TextButton exportProfileButton;
    juce::TextButton importProfileButton;
    juce::TextButton exportCodeButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace gui