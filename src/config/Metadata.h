/******************************************************************************
 * @file    Metadata.h
 * @author  Tim Brewis (@t-bre)
 * @brief   Data model for metadata
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace config
{

/**
 * @brief   Metadata data model
 */
struct Metadata
{
    //==========================================================================
    Metadata(const juce::ValueTree& v);

    //==========================================================================
    juce::CachedValue<juce::String> configName;
    juce::CachedValue<juce::String> versionNumber;
    juce::CachedValue<juce::String> comments;
    juce::ValueTree state;
};

} // namespace config