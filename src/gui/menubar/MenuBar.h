/******************************************************************************
 * @file    MenuBar.h
 * @author  Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief   Menu bar
 *****************************************************************************/

#pragma once

#include "../../CommandManager.h"
#include <JuceHeader.h>
#include <map>
#include <memory>

namespace gui
{

/**
 * @brief Menu bar
 */
class MenuBar : public juce::MenuBarModel, public juce::ApplicationCommandTarget
{
public:

    MenuBar(std::shared_ptr<CommandManager> sharedCommandManager);
    ~MenuBar() override;

    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
    void menuBarActivated(bool isActive) override;

    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    juce::ApplicationCommandTarget* getNextCommandTarget() override;
    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;

private:

    void createMainMenu();
    juce::PopupMenu createWindowMenu();
    juce::PopupMenu createViewMenu();

#if JUCE_MAC
    void setupAppleMenu();
#endif

    juce::PopupMenu mainMenu;
    std::shared_ptr<CommandManager> commandManager;

    typedef enum
    {
        View = 0,
        Window = 1,
    } MenuIndex;

    static const std::map<MenuIndex, juce::String> menuNameMap;
};

} // namespace gui