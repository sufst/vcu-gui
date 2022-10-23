/******************************************************************************
 * @file    InverterEditor.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "InverterEditor.h"

#include "../../../config/Identifiers.h"

namespace gui
{

//==============================================================================

/**
 * @brief   Constructor
 */
InverterEditor::InverterEditor(config::DataModel& configData)
    : torqueMap(configData.tree.getChildWithName(config::IDs::TORQUE_MAP)),
      torqueMapComponent(
          configData.tree.getChildWithName(config::IDs::TORQUE_MAP)),
      interpolationAttachment(
          &interpolationCombo,
          torqueMap.interpolationMethod.getPropertyAsValue())
{
    setupInterpolationCombo();

    addAndMakeVisible(torqueMapComponent);
    addAndMakeVisible(interpolationCombo);
}

/**
 * @brief Setup interpolation method combo box
 */
void InverterEditor::setupInterpolationCombo()
{
    const auto& interpolationMethods = utility::InterpolatorFactory<
        TorqueMapPoint::ValueType>::getAllIdentifiers();

    for (unsigned i = 0; i < interpolationMethods.size(); i++)
    {
        const auto itemId = static_cast<int>(i + 1);
        const auto& method = interpolationMethods.at(i).toString();

        interpolationCombo.addItem(method, itemId);

        if (method == torqueMap.interpolationMethod.get())
        {
            interpolationCombo.setSelectedId(itemId);
        }
    }
}

//==============================================================================

/**
 * @brief   Implements juce::Component::resized()
 */
void InverterEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    auto footerBounds = bounds.removeFromBottom(50);

    torqueMapComponent.setBounds(bounds);

    footerBounds.removeFromTop(5);
    footerBounds.removeFromBottom(5);
    interpolationCombo.setBounds(footerBounds);
}

} // namespace gui