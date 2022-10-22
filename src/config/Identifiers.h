/******************************************************************************
 * @file    Identifiers.h
 * @author  Tim Brewis (@t-bre)
 * @brief   Identifiers for configuration parameters
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>
#define DECLARE_ID(name) const juce::Identifier name(#name);

namespace config
{

/**
 * @brief   Identifiers for configuration parameters and groups
 *
 * @details UPPER_CASE for group names (e.g. torque map)
 *          PascalCase for parameter names
 *
 */
namespace IDs
{
DECLARE_ID(CONFIGURATION)

DECLARE_ID(TORQUE_MAP)
DECLARE_ID(TorqueMapPoint)
DECLARE_ID(Input)
DECLARE_ID(Output)
DECLARE_ID(InterpolationMethod)

DECLARE_ID(METADATA)
DECLARE_ID(ConfigName)
DECLARE_ID(VersionNumber)
DECLARE_ID(Comments)

} // namespace IDs
} // namespace config

#undef DECLARE_ID