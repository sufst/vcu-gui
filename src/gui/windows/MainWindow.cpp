/******************************************************************************
 * @file   MainWindow.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Main GUI window
 *****************************************************************************/

#include "MainWindow.h"

#include <climits>

namespace gui
{

//==============================================================================

/**
 * @brief       Constructor
 *
 * @param[in]   name    Window name
 */
MainWindow::MainWindow(const juce::String& name,
                       config::DataModel& configData,
                       std::shared_ptr<CommandManager> sharedCommandManager)
    : juce::DocumentWindow(
        name,
        juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons),
      configData(configData), menuBar(sharedCommandManager),
      mainComponent(configData),
      commandManager(sharedCommandManager) // ^^^^yikes
{
    setUsingNativeTitleBar(true);
    setResizeLimits(minWidth, minHeight, INT_MAX, INT_MAX);
    setResizable(true, false);
    setVisible(true);
    centreWithSize(getWidth(), getHeight());

    setContentNonOwned(&mainComponent, true);

    jassert(commandManager);
    commandManager->registerAllCommandsForTarget(this);
    addKeyListener(commandManager->getKeyMappings());
}

/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
    commandManager->setFirstCommandTarget(nullptr);
}

//==============================================================================

/**
 * @brief Close button pressed handler
 */
void MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

//==============================================================================

/**
 * @brief Implements juce::ApplicationCommandTarget::getAllCommands()
 */
void MainWindow::getAllCommands(juce::Array<juce::CommandID>& commands)
{
    std::initializer_list<juce::CommandID> targetCommands
        = {CommandManager::CloseWindow,
           CommandManager::MinimiseWindow,
           CommandManager::ToggleFullScreen,
           CommandManager::SaveFile,
           CommandManager::OpenFile};

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
        const juce::String shortName
            = !isFullScreen() ? "Enter Full Screen" : "Exit Full Screen";
        const juce::String longName
            = !isFullScreen() ? "Enters full screen" : "Exits full screen";

        result.setInfo(shortName,
                       longName,
                       CommandManager::CommandCategories::GUI,
                       0);
        result.defaultKeypresses.add(
            juce::KeyPress('f',
                           juce::ModifierKeys::commandModifier
                               | juce::ModifierKeys::ctrlModifier,
                           0));
        break;
    }

    case CommandManager::OpenFile:
    {
        result.setInfo("Open...",
                       "Opens an existing configuration file",
                       CommandManager::CommandCategories::Config,
                       0);
        result.defaultKeypresses.add(
            juce::KeyPress('o', juce::ModifierKeys::commandModifier, 0));
        break;
    }

    case CommandManager::SaveFile:
    {
        result.setInfo("Save...",
                       "Saves the current configuration to file",
                       CommandManager::CommandCategories::Config,
                       0);
        result.defaultKeypresses.add(
            juce::KeyPress('s', juce::ModifierKeys::commandModifier, 0));
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

    case CommandManager::OpenFile:
        loadConfig();
        break;

    case CommandManager::SaveFile:
        saveConfig();
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

//==============================================================================

/**
 * @brief   Loads a configuration from file
 */
void MainWindow::loadConfig()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Load Configuration File",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.xml",
        true);

    auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles
                            | juce::FileBrowserComponent::openMode;

    // TODO: re-integrate
    auto onChoose = [this](const juce::FileChooser& chooser)
    {
        if (!chooser.getResults().isEmpty())
        {
            configData.loadFromFile(chooser.getResult());
        }
        fileChooser.reset();
    };

    fileChooser->launchAsync(fileChooserFlags, onChoose);
}

/**
 * @brief   Saves the configuration to a file
 */
void MainWindow::saveConfig()
{
    // TODO: it would be nice if when the config was loaded from a file, it
    //       would automatically save to that same file without opening
    //       the file browser and overwriting it
    fileChooser = std::make_unique<juce::FileChooser>(
        "Save Configuration",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.xml",
        true);

    auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles
                            | juce::FileBrowserComponent::warnAboutOverwriting
                            | juce::FileBrowserComponent::saveMode;

    auto onChoose = [this](const juce::FileChooser& chooser)
    {
        if (!chooser.getResults().isEmpty())
        {
            configData.saveToFile(chooser.getResult());
        }
        fileChooser.reset();
    };

    fileChooser->launchAsync(fileChooserFlags, onChoose);
}

} // namespace gui