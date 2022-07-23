/******************************************************************************
 * @file   AboutWindow.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  'About' window
 *****************************************************************************/

#include "AboutWindow.h"

namespace gui
{

/**
 * @brief Constructor
 */
AboutWindow::AboutWindow(std::shared_ptr<CommandManager> sharedCommandManager)
    : juce::DialogWindow("About", juce::Colours::white, true), commandManager(sharedCommandManager)
{
    setUsingNativeTitleBar(true);
    setResizable(false, false);
    setAlwaysOnTop(true);
    setVisible(true);
    setSize(430, 150);
    toFront(false);

    setContentNonOwned(&aboutComponent, false);

    commandManager->registerAllCommandsForTarget(this);
}

/**
 * @brief Implements juce::DialogWindow::paint()
 */
void AboutWindow::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(backgroundColourId));
}

/**
 * @brief Implements juce::DialogWindow::closeButtonPressed()
 */
void AboutWindow::closeButtonPressed()
{
    if (onCloseButtonPressed)
    {
        onCloseButtonPressed();
    }
}

/**
 * @brief Implements juce::ApplicationCommandTarget::getAllCommands()
 */
void AboutWindow::getAllCommands(juce::Array<juce::CommandID>& commands)
{
    std::initializer_list<juce::CommandID> targetCommands = {
        CommandManager::CommandIDs::CloseWindow,
    };

    commands.addArray(targetCommands);
}

/**
 * @brief Implements juce::ApplicationCommandTarget::getCommandInfo()
 */
void AboutWindow::getCommandInfo(juce::CommandID commandID,
                                juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case CommandManager::CloseWindow:
        {
            result.setInfo(juce::String("Close"),
                           "Closes the window",
                           CommandManager::CommandCategories::GUI,
                           0);
            result.defaultKeypresses.add(
                juce::KeyPress('w', juce::ModifierKeys::commandModifier, 0));
            break;
        }

        default:
            break;
    }
}

/**
 * @brief Implements juce::ApplicationCommandTarget::perform()
 */
bool AboutWindow::perform(const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandManager::CloseWindow:
        {
            closeButtonPressed();
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
juce::ApplicationCommandTarget* AboutWindow::getNextCommandTarget()
{
    return nullptr;
}

/**
 * @brief Constructor for internal content component
 */
AboutWindow::AboutComponent::AboutComponent()
{
    setupLabels();

    appIconImage.setImage(juce::ImageCache::getFromMemory(
        BinaryData::AppIcon_1024_png, BinaryData::AppIcon_1024_pngSize));

    addAndMakeVisible(appIconImage);
}

/**
 * @brief Sets up static label content
 */
void AboutWindow::AboutComponent::setupLabels()
{
    using Initialiser = std::tuple<juce::Label*,
                                   const juce::String,
                                   float,
                                   juce::Justification,
                                   juce::Colour>;

    juce::String version("Version ");
    version += ProjectInfo::versionString;

    juce::String name(ProjectInfo::projectName);

    const std::initializer_list<Initialiser> initList = {
        {&appNameLabel,
         name,
         50,
         juce::Justification::bottomLeft,
         juce::Colour(225, 225, 225)},
        {&versionLabel,
         version,
         18,
         juce::Justification::topLeft,
         juce::Colour(180, 180, 180)}
    };

    for (const auto& [component, text, fontHeight, justification, textColour] :
         initList)
    {
        addAndMakeVisible(*component);
        component->setText(text, juce::dontSendNotification);
        component->setJustificationType(justification);
        component->setFont(component->getFont().withHeight(fontHeight));
        component->setColour(component->textColourId, textColour);
    }
}

/**
 * @brief Implements juce::Component::resized()
 */
void AboutWindow::AboutComponent::resized()
{
    auto bounds = getLocalBounds();
    int height = bounds.getHeight();

    // app icon
    auto iconBounds = bounds.removeFromLeft(height);
    appIconImage.setBounds(iconBounds.reduced(appIconBorder));

    // app name
    auto nameBounds = bounds.removeFromTop(height / 2);
    appNameLabel.setBounds(nameBounds);

    // app version
    int offset = 4;
    auto versionBounds = bounds.removeFromTop(height / 5);
    versionBounds.setY(versionBounds.getY() - offset);
    versionBounds.setX(versionBounds.getX() + offset);
    versionLabel.setBounds(versionBounds);
}

} // namespace gui