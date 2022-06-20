/******************************************************************************
 * @file   MainWindow.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Main GUI window
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

/**
 * @brief Main GUI window
 */
class MainWindow : public juce::DocumentWindow
{
public:

    MainWindow(const juce::String& name);

    void closeButtonPressed() override;

private:

    static const int minWidth = 500;
    static const int minHeight = 350;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};