/******************************************************************************
 * @file   ConfigurationValueTree.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Data model for VCU configuration
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>
#include <memory>

/**
 * @brief Stores the VCU configuration profile and notifies registered listeners when it changes
 */
class ConfigurationValueTree final
{
public:

    ConfigurationValueTree();

    void addListener(juce::ValueTree::Listener* newListener);
    juce::ValueTree getRoot() const;
    juce::ValueTree getChildWithName(const juce::Identifier& identifier) const;

    std::unique_ptr<juce::XmlDocument> exportXml() const;
    void loadFromFile(const juce::File& file);
    static juce::ValueTree createTorqueMapPoint(int input, int output);

    /**
     * @brief Property type identifiers
     */
    class Properties
    {
    public:

        // metadata
        inline static const juce::Identifier ProfileName = "ProfileName";
        inline static const juce::Identifier ApplicationVersion = "ApplicationVersion";

        // torque map
        inline static const juce::Identifier InterpolationMethod = "InterpolationMethod";
        inline static const juce::Identifier InputValue = "InputValue";
        inline static const juce::Identifier OutputValue = "OutputValue";

    private:

        Properties() = default;
    };

    /**
     * @brief Child type identifiers
     */
    class Children
    {
    public:

        inline static const juce::Identifier TorqueMap = "TorqueMap";
        inline static const juce::Identifier TorqueMapPoint = "TorqueMapPoint";

    private:

        Children() = default;
    };

    /**
     * @brief Tree root identifier
     */
    inline static const juce::Identifier Root = "VCUConfiguration";

private:

    static juce::ValueTree createEmptyConfiguration();
    juce::ValueTree tree;
};
