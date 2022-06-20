/******************************************************************************
 * @file   Application.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Main JUCE application
 *****************************************************************************/

#pragma once

#include "MainWindow.h"
#include <JuceHeader.h>
#include <limits.h>

/**
 * @brief Main JUCE application
 */
class Application : public juce::JUCEApplication
{
public:
    Application() = default;

    void initialise(const juce::String& commandLine) override;
    void shutdown() override;
    void systemRequestedQuit() override;
    void anotherInstanceStarted(const juce::String& commandLine) override;

    const juce::String getApplicationName() override;
    const juce::String getApplicationVersion() override;
    bool moreThanOneInstanceAllowed() override;

private:
    std::unique_ptr<MainWindow> mainWindow;
};