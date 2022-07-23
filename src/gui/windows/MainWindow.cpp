/******************************************************************************
 * @file   MainWindow.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Main GUI window
 *****************************************************************************/

#include "MainWindow.h"

#include "../components/MainComponent.h"
#include <climits>

namespace gui
{

/**
 * @brief       Constructor
 *
 * @param[in]   name    Window name
 */
MainWindow::MainWindow(const juce::String& name,
                       std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree,
                       std::shared_ptr<CommandManager> commandManager)
    : juce::DocumentWindow(
        name,
        juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons),
      menuBar(commandManager)
{
    setUsingNativeTitleBar(true);
    setResizeLimits(minWidth, minHeight, INT_MAX, INT_MAX);
    setResizable(true, false);
    setVisible(true);
    centreWithSize(getWidth(), getHeight());

    jassert(sharedConfigValueTree);
    setContentOwned(new MainComponent(sharedConfigValueTree), true);

    jassert(commandManager);
    commandManager->registerAllCommandsForTarget(this);
}

/**
 * @brief Close button pressed handler
 */
void MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

/**
 * @brief Implements juce::ApplicationCommandTarget::getAllCommands()
 */
void MainWindow::getAllCommands(juce::Array<juce::CommandID>& commands)
{
    std::initializer_list<juce::CommandID> targetCommands = {
        CommandManager::CloseWindow,
        CommandManager::MinimiseWindow,
        CommandManager::ToggleFullScreen,
    };

    commands.addArray(targetCommands);
}

/**
 * @brief Implements juce::ApplicationCommandTarget::getCommandInfo()
 */
void MainWindow::getCommandInfo(juce::CommandID commandID,
                                juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case CommandManager::CloseWindow:
        {
            result.setInfo("Close",
                           "Closes the window",
                           CommandManager::CommandCategories::GUI,
                           0);
            result.defaultKeypresses.add(
                juce::KeyPress('w', juce::ModifierKeys::commandModifier, 0));
            break;
        }

        case CommandManager::MinimiseWindow:
        {
            result.setInfo("Minimise",
                           "Minimises the window",
                           CommandManager::CommandCategories::GUI,
                           0);
            result.defaultKeypresses.add(
                juce::KeyPress('m', juce::ModifierKeys::commandModifier, 0));
            break;
        }

        case CommandManager::ToggleFullScreen:
        {
            const juce::String shortName =
                !isFullScreen() ? "Enter Full Screen" : "Exit Full Screen";
            const juce::String longName =
                !isFullScreen() ? "Enters full screen" : "Exits full screen";

            result.setInfo(
                shortName, longName, CommandManager::CommandCategories::GUI, 0);
            result.defaultKeypresses.add(
                juce::KeyPress('f',
                               juce::ModifierKeys::commandModifier |
                                   juce::ModifierKeys::ctrlModifier,
                               0));
            break;
        }

        default:
            break;
    }
}

/**
 * @brief Implements juce::ApplicationCommandTarget::perform()
 */
bool MainWindow::perform(const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandManager::CloseWindow:
            closeButtonPressed();
            break;

        case CommandManager::MinimiseWindow:
            minimiseButtonPressed();
            break;

        case CommandManager::ToggleFullScreen:
            setFullScreen(!isFullScreen());
            break;

        default:
            return false;
    }

    return true;
}

/**
 * @brief Implements juce::ApplicationCommandTarget::getNextCommandTarget()
 */
juce::ApplicationCommandTarget* MainWindow::getNextCommandTarget()
{
    return static_cast<juce::ApplicationCommandTarget*>(&menuBar);
}

} // namespace gui