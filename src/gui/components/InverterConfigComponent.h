/******************************************************************************
 * @file    InverterConfigComponent.h
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>
#include <memory>

#include "../../ConfigurationValueTree.h"
#include "TorqueMapComponent.h"

namespace gui
{

/**
 * @brief   Configuration provider for inverter related properties
 */
class InverterConfigComponent : public juce::Component
{
public:

    InverterConfigComponent(std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree);

    void setupInterpolationCombo();

    void resized() override;

private:

    std::shared_ptr<ConfigurationValueTree> configValueTree;

    TorqueMapComponent torqueMapComponent;
    juce::ComboBox interpolationCombo;

};

} // namespace gui