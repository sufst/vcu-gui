/******************************************************************************
 * @file   MainComponent.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Main component
 *****************************************************************************/

#include "MainComponent.h"

namespace gui
{

//==============================================================================

/**
 * @brief Default constructor
 */
MainComponent::MainComponent(config::DataModel& configData)
    : configData(configData), inverterComponent(configData)
{
    setSize(600, 400);

    // TODO: re-integrate
    // configValueTree->addListener(this);

    auto& lf = getLookAndFeel();
    auto tabColour = lf.findColour(juce::DocumentWindow::backgroundColourId);
    // TODO: these can be put in a proper initializer list loop once all the
    //       classes for the different tabs have been created
    tabComponent.addTab("Metadata", tabColour, nullptr, false);
    tabComponent.addTab("Inverter", tabColour, &inverterComponent, false);
    tabComponent.addTab("Sensors", tabColour, nullptr, false);
    tabComponent.addTab("Testbenches", tabColour, nullptr, false);
    tabComponent.addTab("Summary", tabColour, nullptr, false);

    addAndMakeVisible(tabComponent);
}

/**
 * @brief Destructor
 */
MainComponent::~MainComponent()
{
    // nothing to do
}

//==============================================================================

/**
 * @brief Implements juce::Component::paint()
 */
void MainComponent::paint(juce::Graphics& g)
{
    auto& lf = getLookAndFeel();
    auto backgroundColour
        = lf.findColour(juce::ResizableWindow::backgroundColourId);

    // TODO: this no longer works because the entire component is covered by
    //       its children! need an overlay type solution
    if (fileIsBeingDragged)
    {
        backgroundColour = backgroundColour.brighter(0.05f);
    }

    g.fillAll(backgroundColour);
}

/**
 * @brief Implements juce::Component::resized()
 */
void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    tabComponent.setBounds(bounds);
}

//==============================================================================

/**
 * @brief   Implements juce::FileDragAndDropTarget::isInterestedInFileDrag()
 *
 * @details Only accepts:
 *          - A single file
 *          - Ending with .xml
 */
bool MainComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    if (files.size() != 1)
    {
        return false;
    }

    return files[0].endsWithIgnoreCase(".xml");
}

/**
 * @brief   Implements juce::FileDragAndDropTarget::filesDropped()
 */
void MainComponent::filesDropped(const juce::StringArray& files,
                                 int /*x*/,
                                 int /*y*/)
{
    const auto& fileName = files[0];
    jassert(fileName.endsWithIgnoreCase(".xml"));

    // TODO: re-implement file loading
    // configValueTree->loadFromFile(juce::File(fileName));

    // fileIsBeingDragged = false;
    // repaint();
}

/**
 * @brief   Implements juce::FileDragAndDropTarget::fileDragEnter()
 */
void MainComponent::fileDragEnter(const juce::StringArray& /*files*/,
                                  int /*x*/,
                                  int /*y*/)
{
    fileIsBeingDragged = true;
    repaint();
}

/**
 * @brief   Implements juce::FileDragAndDropTarget::fileDragExit()
 */
void MainComponent::fileDragExit(const juce::StringArray& /*files*/)
{
    fileIsBeingDragged = false;
    repaint();
}

} // namespace gui