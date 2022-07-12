/******************************************************************************
 * @file   VCUConfiguration.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Data model for VCU configuration
 *****************************************************************************/

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

private:

    static juce::ValueTree createEmptyConfiguration();

    static const juce::Identifier Root;

    juce::ValueTree tree;
};