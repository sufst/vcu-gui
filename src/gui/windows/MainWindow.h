/******************************************************************************
 * @file   MainWindow.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Main GUI window
 *****************************************************************************/

#pragma once

#include "../../CommandManager.h"
#include "../../config/DataModel.h"
#include "../components/MainComponent.h"
#include "../menubar/MenuBar.h"
#include <JuceHeader.h>
#include <memory>

namespace gui
{

/**
 * @brief Main GUI window
 */
class MainWindow : public juce::DocumentWindow,
                   public juce::ApplicationCommandTarget
{
public:

    //==========================================================================
    MainWindow(const juce::String& name,
               config::DataModel& config,
               std::shared_ptr<CommandManager> sharedCommandManager);
    ~MainWindow() override;

    //==========================================================================
    void closeButtonPressed() override;

    //==========================================================================
    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo(juce::CommandID commandID,
                        juce::ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;
    juce::ApplicationCommandTarget* getNextCommandTarget() override;

private:

    //==========================================================================
    void loadConfig();
    void saveConfig();

    //==========================================================================
    config::DataModel configData;
    MenuBar menuBar;
    MainComponent mainComponent;
    std::shared_ptr<CommandManager> commandManager;
    std::unique_ptr<juce::FileChooser> fileChooser;

    //==========================================================================
    static const int minWidth = 500;
    static const int minHeight = 350;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

} // namespace gui