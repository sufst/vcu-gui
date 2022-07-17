/******************************************************************************
 * @file   VCUConfiguration.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Data model for VCU configuration
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>
#include <memory>

/**
 * @brief Stores the VCU configuration profile and notifies registered listeners when it changes
 */
class VCUConfiguration : public juce::ChangeBroadcaster
{
public:

    VCUConfiguration();

    juce::ValueTree getTorqueMap() const;

    std::unique_ptr<juce::XmlDocument> exportXml() const;
    void loadFromXml(juce::XmlDocument& xml);

    static const juce::Identifier TorqueMap;
    static const juce::Identifier TorqueMapPoint;
    static const juce::Identifier TorqueMapInputValue;
    static const juce::Identifier TorqueMapOutputValue;
    static const juce::Identifier ProfileName;
    static const juce::Identifier InterpolationMethod;

    static juce::ValueTree createTorqueMapPoint(int input, int output);

private:

    static juce::ValueTree createEmptyConfiguration();

    static const juce::Identifier Root;

    juce::ValueTree tree;
};