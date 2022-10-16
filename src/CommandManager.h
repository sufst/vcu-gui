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

    //===========================================================================
    CommandManager() = default;
    ~CommandManager() = default;

    //===========================================================================
    enum CommandIDs
    {
        // menu bar
        ShowAboutWindow = 0x100020,
        ShowPreferencesWindow = 0x100021,

        // windows
        CloseWindow = 0x100030,
        MinimiseWindow = 0x100031,
        ToggleFullScreen = 0x100032,

        // config actions
        ImportConfig = 0x100040,
        ExportConfig = 0x100041
    };

    struct CommandCategories
    {
        static constexpr const char* General = "General";
        static constexpr const char* GUI = "GUI";
        static constexpr const char* Config = "Configuration";
    };
};