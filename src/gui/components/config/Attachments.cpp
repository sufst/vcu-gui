/******************************************************************************
 * @file    Attachments.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "Attachments.h"

namespace gui
{

//==============================================================================

/**
 * @brief       Creates a new attachment between a combo box and a value
 *
 * @param[in]   combo   Combo box to attach to
 * @param[in]   value   Value to attach
 */
ComboBoxAttachment::ComboBoxAttachment(juce::ComboBox* combo, juce::Value value)
    : attachedValue(value), attachedCombo(combo)
{
    jassert(attachedCombo);
    attachedValue.addListener(this);

    attachedCombo->onChange = [this]()
    {
        attachedValue.setValue(attachedCombo->getText());
        ignoreUpdate = true;

        if (onChange)
        {
            onChange();
        }
    };
}

/**
 * @brief   Destructor
 */
ComboBoxAttachment::~ComboBoxAttachment()
{
    attachedValue.removeListener(this);
}

/**
 * @brief   Implements juce::Value::Listener::valueChanged()
 */
void ComboBoxAttachment::valueChanged(juce::Value& value)
{
    // ignoreUpdate is used to prevent a change in the attached combo box
    // from updating that combo box in this callback!
    if (!ignoreUpdate)
    {
        const auto str = value.toString();

        // TODO: slow search?
        for (int i = 0; i < attachedCombo->getNumItems(); i++)
        {
            if (str == attachedCombo->getItemText(i))
            {
                attachedCombo->setSelectedItemIndex(i);
            }
        }
    }
    else
    {
        ignoreUpdate = false;
    }
}

} // namespace gui