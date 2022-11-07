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

#if JUCE_MAC
/**
 * @brief Set ups the 'Apple' menu (macOS only)
 */
void MenuBar::setupAppleMenu()
{
    appleMenu.addCommandItem(commandManager.get(),
                             CommandManager::CommandIDs::ShowAboutWindow);

    juce::PopupMenu::MenuItemIterator iter(appleMenu, false);

    while (iter.next())
    {
        iter.getItem().setEnabled(true);
    }

    setMacMainMenu(this, &appleMenu);
}
#endif

/**
 * @brief       Creates a popup menu with the specified command manager commands
 *
 * @param[in]   commands    List of command IDs
 */
juce::PopupMenu MenuBar::createMenuWithCommands(
    std::initializer_list<CommandManager::CommandIDs> commands)
{
    juce::PopupMenu menu;

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
        return createMenuWithCommands({
            CommandManager::OpenFile,
            CommandManager::SaveFile,
        });

    case MenuIndex::Window:
        return createMenuWithCommands({
            CommandManager::CloseWindow,
            CommandManager::MinimiseWindow,
        });

    case MenuIndex::View:
        return createMenuWithCommands({
            CommandManager::ToggleFullScreen,
        });

    case MenuIndex::Help:
        return createMenuWithCommands(
            {CommandManager::ShowAboutWindow, CommandManager::ShowGitHubRepo});

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
    std::initializer_list<juce::CommandID> targetCommands
        = {CommandManager::CommandIDs::ShowAboutWindow,
           CommandManager::CommandIDs::ShowGitHubRepo};

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

    case CommandManager::ShowGitHubRepo:
    {
        result.setInfo("View project on GitHub...",
                       "Opens GitHub repo for project",
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

    case CommandManager::ShowGitHubRepo:
    {
        juce::URL url(GITHUB_REPO_URL);
        url.launchInDefaultBrowser();
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
const std::map<MenuBar::MenuIndex, juce::String> MenuBar::menuNameMap
    = {{MenuBar::MenuIndex::File, "File"},
       {MenuBar::MenuIndex::View, "View"},
       {MenuBar::MenuIndex::Window, "Window"},
       {MenuBar::MenuIndex::Help, "Help"}};

} // namespace gui