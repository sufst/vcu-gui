/******************************************************************************
 * @file    InverterConfigComponent.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "InverterConfigComponent.h"

namespace gui
{

//==============================================================================

/**
 * @brief   Constructor
 */
InverterConfigComponent::InverterConfigComponent(
    std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree)
    : configValueTree(sharedConfigValueTree),
      torqueMapComponent(sharedConfigValueTree)
{
    setupInterpolationCombo();

    addAndMakeVisible(torqueMapComponent);
    addAndMakeVisible(interpolationCombo);
}

/**
 * @brief Setup interpolation method combo box
 */
void InverterConfigComponent::setupInterpolationCombo()
{
    const auto& interpolationMethods
        = utility::InterpolatorFactory<int>::getAllIdentifiers();

    juce::ValueTree torqueMap = configValueTree->getChildWithName(
        ConfigurationValueTree::Children::TorqueMap);
    const juce::String selectedMethod = torqueMap.getProperty(
        ConfigurationValueTree::Properties::InterpolationMethod);

    for (unsigned i = 0; i < interpolationMethods.size(); i++)
    {
        const auto itemId = static_cast<int>(i + 1);
        const auto& method = interpolationMethods.at(i).toString();

        interpolationCombo.addItem(method, itemId);

        if (method == selectedMethod)
        {
            interpolationCombo.setSelectedId(itemId);
        }
    }

    interpolationCombo.onChange = [this]() mutable
    {
        int selectedIndex = interpolationCombo.getSelectedItemIndex();
        juce::String value = interpolationCombo.getItemText(selectedIndex);
        auto map = configValueTree->getChildWithName(
            ConfigurationValueTree::Children::TorqueMap);

        map.setProperty(ConfigurationValueTree::Properties::InterpolationMethod,
                        value,
                        nullptr);
    };
}

//==============================================================================

/**
 * @brief   Implements juce::Component::resized()
 */
void InverterConfigComponent::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    auto footerBounds = bounds.removeFromBottom(50);

    torqueMapComponent.setBounds(bounds);

    footerBounds.removeFromTop(5);
    footerBounds.removeFromBottom(5);
    interpolationCombo.setBounds(footerBounds);
}

} // namespace gui