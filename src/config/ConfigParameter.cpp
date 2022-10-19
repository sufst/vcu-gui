/******************************************************************************
 * @file    ConfigParameter.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "ConfigParameter.h"

//==============================================================================

/**
 * @brief       Create a new parameter with the specified identifier
 *
 * @param[in]   parameterID     Identifier
 * @param[in]   parameterName   Display name
 */
ConfigParameter::ConfigParameter(const juce::String& parameterID,
                                 const juce::String& parameterName)
    : identifier(parameterID), name(parameterName)
{
}

//==============================================================================

/**
 * @brief       Sets the value of the parameter
 *
 * @details     This defaults to just setting the variant
 *
 * @param[in]   newValue    New value to set
 */
void ConfigParameter::setValue(const juce::var& newValue)
{
    setVar(newValue);
}

/**
 * @brief   Returns the value of the parameter
 */
const juce::var ConfigParameter::getValue() const
{
    return value.getValue();
}

/**
 * @brief   Returns true if the value is valid
 *
 * @note    See comments on validate()
 */
bool ConfigParameter::isValid() const
{
    return validate();
}

//==============================================================================

/**
 * @brief   Validates the value of the variant
 *
 * @details This class assumes any value is valid, base classes can override
 *          this to perform their own validation logic
 *
 * @return true     The value is valid
 * @return false    The value is invalid
 */
bool ConfigParameter::validate() const
{
    return true;
}

//==============================================================================

/**
 * @brief       Sets the value of the internal variant
 *
 * @param[in]   newValue    New value to assign
 */
void ConfigParameter::setVar(const juce::var& newValue)
{
    value = newValue;
}

/**
 * @brief Gets the value of the internal variant
 */
const juce::var ConfigParameter::getVar() const
{
    return value.getValue();
}