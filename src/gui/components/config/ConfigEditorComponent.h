/******************************************************************************
 * @file    ConfigEditorComponent.h
 * @author  Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace gui
{

/**
 * @brief   Base class for components providing editors for configuration
 *          parameters
 */
class ConfigEditorComponent : public juce::Component
{
public:

    //==========================================================================
    ConfigEditorComponent() = default;
};

} // namespace gui