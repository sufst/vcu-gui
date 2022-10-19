/******************************************************************************
 * @file    ConfigParameter.h
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

/**
 * @brief   Variant class for storing parameters
 *
 * @details This is mostly a wrapper class around juce::var which provides some
 *          extra functionality specific to our requirements
 */
class ConfigParameter
{
public:

    //==========================================================================
    ConfigParameter() = delete;
    ConfigParameter(const juce::String& parameterID,
                    const juce::String& parameterName);
    virtual ~ConfigParameter() = default;

    //==========================================================================
    virtual void setValue(const juce::var& newValue);
    virtual const juce::var getValue() const;

    //==========================================================================
    bool isValid() const;

protected:

    //==========================================================================
    void setVar(const juce::var& newValue);
    const juce::var getVar() const;
    virtual bool validate() const;

private:

    //==========================================================================

    /**
     * @brief   Identifier
     */
    juce::String identifier;

    /**
     * @brief   Display name (e.g. for use in GUI)
     */
    juce::String name;

    /**
     * @brief   Variant for storing value
     */
    juce::Value value;
};