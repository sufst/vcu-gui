/******************************************************************************
 * @file   MetadataEditor.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 *****************************************************************************/

#include "MetadataEditor.h"

namespace gui
{

/**
 * @brief       Creates a new editor
 *
 * @param[in]   metadataValueTree   Value tree containing metadata
 */
MetadataEditor::MetadataEditor(juce::ValueTree metadataValueTree)
    : metadata(metadataValueTree)
{
    setupTextEditor("Name:",
                    metadata.configName.getPropertyAsValue(),
                    nameLabel,
                    nameEditor);
    setupTextEditor("Version:",
                    metadata.versionNumber.getPropertyAsValue(),
                    versionLabel,
                    versionEditor);
    setupTextEditor("Comments:",
                    metadata.comments.getPropertyAsValue(),
                    commentsLabel,
                    commentsEditor);

    commentsEditor.setMultiLine(true);
}

/**
 * @brief       Sets up text editors in a consistent way
 *
 * @param[in]   name    Name for label
 * @param[in]   value   Value in metadata config to attach editor to
 * @param[in]   label   Label component
 * @param[in]   editor  Text editor component
 */
void MetadataEditor::setupTextEditor(const juce::String& name,
                                     juce::Value value,
                                     juce::Label& label,
                                     juce::TextEditor& editor)
{
    editor.getTextValue().referTo(value);

    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::topLeft);

    addAndMakeVisible(label);
    addAndMakeVisible(editor);
}

/**
 * @brief   Implements juce::Component::resized()
 */
void MetadataEditor::resized()
{
    constexpr int spacing = 20;
    constexpr int labelWidth = 100;
    auto bounds = getLocalBounds().reduced(spacing);

    std::initializer_list<std::tuple<juce::Label*, juce::Component*, int>> sizes
        = {{&nameLabel, &nameEditor, spacing},
           {&versionLabel, &versionEditor, spacing},
           {&commentsLabel, &commentsEditor, spacing * 4}};

    for (const auto& [label, component, height] : sizes)
    {
        // TODO: the label should use a flexbox layout here
        auto itemBounds = bounds.removeFromTop(height);
        auto labelBounds = itemBounds.removeFromLeft(labelWidth);

        label->setBounds(labelBounds);
        component->setBounds(itemBounds);

        bounds.removeFromTop(height / 2);
    }
}

} // namespace gui