/******************************************************************************
 * @file    InverterConfigComponent.h
 * @author  Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 *****************************************************************************/

#pragma once

#include "../../../config/DataModel.h"
#include "ConfigEditorComponent.h"
#include "TorqueMapComponent.h"
#include <JuceHeader.h>
#include <memory>

namespace gui
{

/**
 * @brief   Configuration provider for inverter related properties
 */
class InverterConfigComponent : public ConfigEditorComponent
{
public:

    //==========================================================================
    InverterConfigComponent(config::DataModel& configData);

    void setupInterpolationCombo();

    //==========================================================================
    void resized() override;

private:

    //==========================================================================
    TorqueMap torqueMap;
    TorqueMapComponent torqueMapComponent;
    juce::ComboBox interpolationCombo;
};

} // namespace gui