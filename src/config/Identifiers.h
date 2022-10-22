/******************************************************************************
 * @file    Identifiers.h
 * @author  Tim Brewis (@t-bre)
 * @brief   Identifiers for configuration parameters
 *****************************************************************************/

#pragma once

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
DECLARE_ID(Point)
DECLARE_ID(Input)
DECLARE_ID(Output)
DECLARE_ID(InterpolationMethod)

} // namespace IDs
} // namespace config

#undef DECLARE_ID