/******************************************************************************
 * @file    ConfigParameterGroup.h
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#pragma once

#include "ConfigParameter.h"
#include <JuceHeader.h>

/**
 * @brief   Group of config parameters
 *
 * @note    Based on juce::AudioProcessorParameterGroup
 */
class ConfigParameterGroup
{
public:

    //==========================================================================
    ConfigParameterGroup(const juce::String& groupID,
                         const juce::String& groupName);

    //==========================================================================

    /**
     * @brief   Contains either a ConfigParameter or a ConfigParameterGroup
     *
     * @note    Pretty much an exact clone of juce::AudioProcessorParameterNode
     */
    class ConfigParameterNode
    {
    public:

        ConfigParameterNode() = delete;
        ~ConfigParameterNode() = default;

        ConfigParameterGroup* getParent() const;
        ConfigParameter* getParameter() const;
        ConfigParameterGroup* getGroup() const;

    private:

        ConfigParameterNode(std::unique_ptr<ConfigParameter> parameter,
                            ConfigParameterGroup* parentGroup);
        ConfigParameterNode(std::unique_ptr<ConfigParameterGroup> grp,
                            ConfigParameterGroup* parentGroup);

        std::unique_ptr<ConfigParameter> param;
        std::unique_ptr<ConfigParameterGroup> group;
        ConfigParameterGroup* parent = nullptr;

        friend class ConfigParameterGroup;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConfigParameterNode)
    };

    //==========================================================================

    template <typename ChildType>
    void addChild(std::unique_ptr<ChildType> child)
    {
        append(std::move(child));
    }

private:

    //==========================================================================
    void append(std::unique_ptr<ConfigParameter>);
    void append(std::unique_ptr<ConfigParameterGroup>);

    //==========================================================================
    juce::String identifier;
    juce::String name;
    juce::OwnedArray<ConfigParameterNode> children;

    //==========================================================================
    static constexpr const char* subgroupSeparator = ".";
};