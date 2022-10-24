/******************************************************************************
 * @file    Attachments.h
 * @author  Tim Brewis (@t-bre)
 * @brief   Useful classes for maintaining a connection between a config
 *          parameter and a GUI widget
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>
#include <functional>

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
 *          Do not use juce::ComboBox::onChange, use the onChange callback
 *          function in this class as a replacement
 */
class ComboBoxAttachment : public juce::Value::Listener
{
public:

    //==========================================================================
    ComboBoxAttachment(juce::ComboBox* combo, juce::Value value);
    ~ComboBoxAttachment() override;

    //==========================================================================
    void valueChanged(juce::Value& value) override;

    //==========================================================================
    std::function<void()> onChange; // set this as a change callback

private:

    //==========================================================================
    bool ignoreUpdate = false;
    juce::Value attachedValue;
    juce::ComboBox* attachedCombo;
};

} // namespace gui