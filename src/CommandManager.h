/******************************************************************************
 * @file    CommandManager.h
 * @author  Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief   Application command manager
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

/**
 * @brief Application command manager
 */
class CommandManager : public juce::ApplicationCommandManager
{
public:

    CommandManager() = default;
    ~CommandManager() = default;

    enum CommandIDs
    {
        // menu bar
        ShowAboutWindow = 0x100020,
        ShowPreferencesWindow = 0x100021,

        // windows
        CloseWindow = 0x100030,
        MinimiseWindow = 0x100031,
        ToggleFullScreen = 0x100032,
    };

    struct CommandCategories
    {
        inline static const char* General = "General";
        inline static const char* GUI = "GUI";
    };
};