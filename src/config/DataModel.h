/******************************************************************************
 * @file    DataModel.h
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#pragma once

#include "Identifiers.h"
#include "TorqueMap.h"
#include <JuceHeader.h>

namespace config
{

/**
 * @brief   Data model for VCU configuration
 */
struct DataModel
{
    //==========================================================================
    DataModel();

    //==========================================================================
    void createDefaultModel();

    //==========================================================================
    juce::ValueTree tree;
};

} // namespace config