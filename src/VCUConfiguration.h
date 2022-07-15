/******************************************************************************
 * @file   VCUConfiguration.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Data model for VCU configuration
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

class VCUConfiguration
{
public:

    VCUConfiguration();

    juce::ValueTree getTorqueMap() const;

    static const juce::Identifier TorqueMap;
    static const juce::Identifier TorqueMapPoint;
    static const juce::Identifier TorqueMapInputValue;
    static const juce::Identifier TorqueMapOutputValue;
    static const juce::Identifier ProfileName;

    static juce::ValueTree createTorqueMapPoint(int input, int output);

private:

    static juce::ValueTree createEmptyConfiguration();

    static const juce::Identifier Root;

    juce::ValueTree tree;
};