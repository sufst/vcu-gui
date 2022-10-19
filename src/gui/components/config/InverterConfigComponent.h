/******************************************************************************
 * @file    InverterConfigComponent.h
 * @author  Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 *****************************************************************************/

#pragma once

#include "../../../ConfigurationValueTree.h"
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
    InverterConfigComponent(
        std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree);

    void setupInterpolationCombo();

    //==========================================================================
    void resized() override;

private:

    //==========================================================================
    std::shared_ptr<ConfigurationValueTree> configValueTree;

    TorqueMapComponent torqueMapComponent;
    juce::ComboBox interpolationCombo;
};

} // namespace gui