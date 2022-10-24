/******************************************************************************
 * @file    Metadata.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "Metadata.h"

#include "Identifiers.h"

namespace config
{

/**
 * @brief       Creates a metadata model
 *
 * @param[in]   v   Value tree holding data
 */
Metadata::Metadata(const juce::ValueTree& v) : state(v)
{
    configName.referTo(state, IDs::ConfigName, nullptr);
    configName.setDefault("New Configuration");
    configName.setValue(configName.getDefault(), nullptr);

    versionNumber.referTo(state, IDs::VersionNumber, nullptr);
    versionNumber.setDefault("1.0.0");
    versionNumber.setValue(versionNumber.getDefault(), nullptr);

    comments.referTo(state, IDs::Comments, nullptr);
    comments.setDefault("-");
    comments.setValue(comments.getDefault(), nullptr);
}

} // namespace config