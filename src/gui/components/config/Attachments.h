/******************************************************************************
 * @file    Attachments.h
 * @author  Tim Brewis (@t-bre)
 * @brief   Useful classes for maintaining a connection between a config
 *          parameter and a GUI widget
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace gui
{

//==============================================================================

/**
 * @brief   Combo box attachment
 *
 * @details When the value changes, if the new value doesn't match one of the
 *          options in the combo box, the change will be ignored
 *
 * @note    Make sure that the combo box doesn't go out of scope!
 */
class ComboBoxAttachment : public juce::Value::Listener
{
public:

    //==========================================================================
    ComboBoxAttachment(juce::ComboBox* combo, juce::Value value);
    ~ComboBoxAttachment() override;

    //==========================================================================
    void valueChanged(juce::Value& value) override;

private:

    //==========================================================================
    bool ignoreUpdate = false;
    juce::Value attachedValue;
    juce::ComboBox* attachedCombo;
};

} // namespace gui