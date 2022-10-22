/******************************************************************************
 * @file    MetadataEditor.h
 * @author  Tim Brewis (@t-bre)
 * @brief   Editor for metadata model
 *****************************************************************************/

#pragma once

#include "../../../config/Metadata.h"
#include "ConfigEditorComponent.h"
#include <JuceHeader.h>

namespace gui
{

/**
 * @brief   Configuration component for metadata
 *
 *          TODO: note this is just a proof of concept!
 *                obviously this looks terrible...
 */
class MetadataEditor : public ConfigEditorComponent
{
public:

    //==========================================================================
    MetadataEditor(juce::ValueTree metadataValueTree);

    //==========================================================================
    void resized() override;

private:

    //==========================================================================
    void setupTextEditor(const juce::String& name,
                         juce::Value value,
                         juce::Label& label,
                         juce::TextEditor& editor);

    //==========================================================================
    config::Metadata metadata;

    juce::Label versionLabel;
    juce::TextEditor versionEditor;

    juce::Label nameLabel;
    juce::TextEditor nameEditor;

    juce::Label commentsLabel;
    juce::TextEditor commentsEditor;
};

} // namespace gui