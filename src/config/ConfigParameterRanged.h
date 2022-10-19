/******************************************************************************
 * @file    ConfigParameterRanged.h
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#pragma once

#include "../utility/clip.h"
#include "ConfigParameter.h"

/**
 * @brief   Ranged configuration parameter
 */
template <typename ValueType>
class ConfigParameterRanged : public ConfigParameter
{
public:

    //==========================================================================
    ConfigParameterRanged() = delete;

    /**
     * @brief       Create a new ranged parameter
     *
     * @param[in]   parameterID     Parameter identifier
     * @param[in]   parameterName   Parameter name
     * @param[in]   minValue        Minimum value
     * @param[in]   maxValue        Maximum value
     * @param[in]   defaultValue    Default/initial value
     * @param[in]   shouldClip      Sets whether the value should be clipped to
     *                              the specified range when setting it
     */
    ConfigParameterRanged(const juce::String& parameterID,
                          const juce::String& parameterName,
                          ValueType minValue,
                          ValueType maxValue,
                          ValueType defaultValue,
                          bool shouldClip)
        : ConfigParameter(parameterID, parameterName), min(minValue),
          max(maxValue), applyClipping(shouldClip)
    {
        // TODO: the length of the parameter list is a bit silly here
        setValue(defaultValue);
    }

    /**
     * @brief   Destructor
     */
    ~ConfigParameterRanged() override = default;

    //==========================================================================

    /**
     * @brief       Sets the value of the parameter, applying clipping to range
     *              if enabled
     *
     * @param[in]   newValue    New value to set
     */
    void setValue(const juce::var& newValue) override
    {
        auto unclipped = static_cast<ValueType>(newValue);
        setVar(applyClipping ? utility::clip(unclipped, min, max) : unclipped);
    }

private:

    //==========================================================================

    /**
     * @brief   Implements ConfigParameter::validate()
     */
    bool validate() const override
    {
        auto value = static_cast<ValueType>(getVar());
        return (min <= value) && (value <= max);
    }

    //==========================================================================
    ValueType min;
    ValueType max;
    bool applyClipping;
};