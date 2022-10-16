/******************************************************************************
 * @file    MenuBar.cpp
 * @author  Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief   Menu bar
 *****************************************************************************/

#include "MenuBar.h"

namespace gui
{

//==============================================================================

/**
 * @brief Default constructor
 */
MenuBar::MenuBar(std::shared_ptr<CommandManager> sharedCommandManager)
    : commandManager(sharedCommandManager)
{
    jassert(commandManager);
    commandManager->registerAllCommandsForTarget(this);
    setApplicationCommandManagerToWatch(commandManager.get());

    createMainMenu();

#if JUCE_MAC
    setupAppleMenu();
#endif
}

/**
 * @brief Destructor
 */
MenuBar::~MenuBar()
{
#if JUCE_MAC
    setMacMainMenu(nullptr);
#endif
}

//==============================================================================

/**
 * @brief Creates the main menu
 */
void MenuBar::createMainMenu()
{
    mainMenu.addCommandItem(commandManager.get(),
                            CommandManager::CommandIDs::ShowAboutWindow);
}

#if JUCE_MAC
/**
 * @brief Set ups the 'Apple' menu (macOS only)
 */
void MenuBar::setupAppleMenu()
{
    juce::PopupMenu::MenuItemIterator iter(mainMenu, false);

    while (iter.next())
    {
        iter.getItem().setEnabled(true);
    }

    setMacMainMenu(this, &mainMenu);
}
#endif

/**
 * @brief   Creates the 'File' menu
 */
juce::PopupMenu MenuBar::createFileMenu()
{
    juce::PopupMenu menu;

    std::initializer_list<CommandManager::CommandIDs> commands = {
        CommandManager::ImportConfig,
        CommandManager::ExportConfig,
    };

    for (const auto& cmd : commands)
    {
        menu.addCommandItem(commandManager.get(), cmd);
    }

    return menu;
}

/**
 * @brief Creates the 'Window' menu
 */
juce::PopupMenu MenuBar::createWindowMenu()
{
    juce::PopupMenu menu;

    std::initializer_list<CommandManager::CommandIDs> commands = {
        CommandManager::CloseWindow,
        CommandManager::MinimiseWindow,
    };

    for (const auto& cmd : commands)
    {
        menu.addCommandItem(commandManager.get(), cmd);
    }

    return menu;
}

/**
 * @brief Creates the 'View' menu
 */
juce::PopupMenu MenuBar::createViewMenu()
{
    juce::PopupMenu menu;

    std::initializer_list<CommandManager::CommandIDs> commands = {
        CommandManager::ToggleFullScreen,
    };

    for (const auto& cmd : commands)
    {
        menu.addCommandItem(commandManager.get(), cmd);
    }

    return menu;
}

/**
 * @brief Implements juce::MenuBarModel::getMenuBarNames()
 */
juce::StringArray MenuBar::getMenuBarNames()
{
    juce::StringArray names;

    for (const auto& [key, value] : menuNameMap)
    {
        names.add(value);
    }

    return names;
}

/**
 * @brief Implements juce::MenuBarModel::getMenuForIndex()
 */
juce::PopupMenu MenuBar::getMenuForIndex(int topLevelMenuIndex,
                                         const juce::String& menuName)
{
    (void) menuName;

    switch (topLevelMenuIndex)
    {
    case MenuIndex::File:
        return createFileMenu();

    case MenuIndex::Window:
        return createWindowMenu();

    case MenuIndex::View:
        return createViewMenu();

    default:
        return juce::PopupMenu();
    }
}

/**
 * @brief Implements juce::MenuBarModel::menuItemSelected()
 */
// NOLINTBEGIN(bugprone-easily-swappable-parameters)
void MenuBar::menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/)
{
}
// NOLINTEND(bugprone-easily-swappable-parameters)

/**
 * @brief Implements juce::MenuBarModel::menuBarActivated()
 */
void MenuBar::menuBarActivated(bool /*isActive*/)
{
}

//==============================================================================

/**
 * @brief Implements juce::ApplicationCommandTarget::getAllCommands()
 */
void MenuBar::getAllCommands(juce::Array<juce::CommandID>& commands)
{
    std::initializer_list<juce::CommandID> targetCommands = {
        CommandManager::CommandIDs::ShowAboutWindow,
    };

    commands.addArray(targetCommands);
}

/**
 * @brief Implements juce::ApplicationCommandTarget::getCommandInfo()
 */
void MenuBar::getCommandInfo(juce::CommandID commandID,
                             juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandManager::ShowAboutWindow:
    {
        result.setInfo(juce::String("About ") + ProjectInfo::projectName,
                       "Shows about window",
                       CommandManager::CommandCategories::GUI,
                       0);
        break;
    }

    default:
        break;
    }
}

/**
 * @brief Implements juce::ApplicationCommandTarget::perform()
 */
bool MenuBar::perform(const InvocationInfo& info)
{
    switch (info.commandID)
    {
    case CommandManager::ShowAboutWindow:
    {
        if (!aboutWindow)
        {
            aboutWindow = std::make_unique<AboutWindow>(commandManager);

            aboutWindow->onCloseButtonPressed
                = [this]() { aboutWindow.reset(); };
        }
        break;
    }

    default:
        return false;
    }

    return true;
}

/**
 * @brief Implements juce::ApplicationCommandTarget::getNextCommandTarget()
 */
juce::ApplicationCommandTarget* MenuBar::getNextCommandTarget()
{
    return nullptr;
}

//==============================================================================

/**
 * @brief   Map between menu indexes and identifying strings
 *
 * @note    This is done as a map and not an array to allow the menus to be
 *          re-ordered without having to re-order the array.
 */
const std::map<MenuBar::MenuIndex, juce::String> MenuBar::menuNameMap = {
    {MenuBar::MenuIndex::File, "File"},
    {MenuBar::MenuIndex::View, "View"},
    {MenuBar::MenuIndex::Window, "Window"},
};

} // namespace gui