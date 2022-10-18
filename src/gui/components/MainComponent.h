/******************************************************************************
 * @file   MainComponent.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Main GUI component
 *****************************************************************************/

#pragma once

#include "TabbedComponent.h"
#include "config/InverterConfigComponent.h"
#include <JuceHeader.h>

namespace gui
{

/**
 * @brief Main GUI component
 */
class MainComponent : public juce::Component,
                      public juce::FileDragAndDropTarget,
                      public juce::ValueTree::Listener
{
public:

    //==========================================================================
    MainComponent(
        std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree);
    ~MainComponent() override;

    //==========================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    //==========================================================================
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void fileDragEnter(const juce::StringArray& files, int x, int y) override;
    void fileDragExit(const juce::StringArray& files) override;

    //==========================================================================
    void valueTreeRedirected(juce::ValueTree& redirectedTree) override;

private:

    //==========================================================================
    void setupInterpolationCombo();
    void setupButtons();

    //==========================================================================
    std::shared_ptr<ConfigurationValueTree> configValueTree;

    std::unique_ptr<juce::FileChooser> fileChooser;
    bool fileIsBeingDragged = false;

    TabbedComponent tabComponent;
    InverterConfigComponent inverterComponent;

    //==========================================================================
    static const int borderSize = 20;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace gui