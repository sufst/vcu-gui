/******************************************************************************
 * @file    MenuBar.h
 * @author  Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief   Menu bar
 *****************************************************************************/

#pragma once

#include "../../CommandManager.h"
#include "../windows/AboutWindow.h"
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

    //==========================================================================
    MenuBar(std::shared_ptr<CommandManager> sharedCommandManager);
    ~MenuBar() override;

    //==========================================================================
    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex,
                                    const juce::String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
    void menuBarActivated(bool isActive) override;

    //==========================================================================
    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    juce::ApplicationCommandTarget* getNextCommandTarget() override;
    void getCommandInfo(juce::CommandID commandID,
                        juce::ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;

private:

    //==========================================================================
    juce::PopupMenu createMenuWithCommands(
        std::initializer_list<CommandManager::CommandIDs> commands);

#if (JUCE_MAC)
    void setupAppleMenu();
#endif

    //==========================================================================
#if (JUCE_MAC)
    juce::PopupMenu appleMenu;
#endif

    std::shared_ptr<CommandManager> commandManager;
    std::unique_ptr<AboutWindow> aboutWindow;

    typedef enum
    {
        File = 0,
        View = 1,
        Window = 2,
        Help = 3
    } MenuIndex;

    static const std::map<MenuIndex, juce::String> menuNameMap;
};

} // namespace gui