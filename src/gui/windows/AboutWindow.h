/******************************************************************************
 * @file   AboutWindow.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  'About' window
 *****************************************************************************/

#pragma once

#include "../../CommandManager.h"
#include <JuceHeader.h>
#include <functional>
#include <memory>

namespace gui
{

/**
 * @brief 'About' window
 */

class AboutWindow : public juce::DialogWindow,
                    public juce::ApplicationCommandTarget
{
public:

    //==========================================================================
    AboutWindow(std::shared_ptr<CommandManager> sharedCommandManager);

    //==========================================================================
    void paint(juce::Graphics& g) override;

    //==========================================================================
    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo(juce::CommandID commandID,
                        juce::ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;
    juce::ApplicationCommandTarget* getNextCommandTarget() override;

    //==========================================================================
    void closeButtonPressed() override;

    std::function<void()> onCloseButtonPressed;

private:

    //==========================================================================

    /**
     * @brief Internal content component class
     */
    class AboutComponent : public juce::Component
    {
    public:

        AboutComponent();
        void resized() override;

    private:

        void setupLabels();

        juce::Label appNameLabel;
        juce::Label versionLabel;
        juce::Label commitHashLabel;
        juce::ImageComponent appIconImage;

        const int appIconBorder = 12;
    };

    //==========================================================================
    AboutComponent aboutComponent;
    std::shared_ptr<CommandManager> commandManager;
};

} // namespace gui