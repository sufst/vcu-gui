/******************************************************************************
 * @file   MainComponent.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Main component
 *****************************************************************************/

#include "MainComponent.h"

#include "../../Interpolator.h"

namespace gui
{

/**
 * @brief Default constructor
 */
MainComponent::MainComponent(std::shared_ptr<ConfigurationValueTree> sharedConfigValueTree)
    : configValueTree(sharedConfigValueTree), inverterComponent(sharedConfigValueTree)
{
    setSize(600, 400);

    configValueTree->addListener(this);

    auto& lf = getLookAndFeel();
    tabComponent.addTab("Inverter", lf.findColour(juce::DocumentWindow::backgroundColourId), &inverterComponent, false);
    addAndMakeVisible(tabComponent);
}

/**
 * @brief Destructor
 */
MainComponent::~MainComponent()
{
    // nothing to do
}

/**
 * @brief Painter
 *
 * @param[in]   g   Graphics context
 */
void MainComponent::paint(juce::Graphics& g)
{
    auto& lf = getLookAndFeel();
    auto backgroundColour = lf.findColour(juce::ResizableWindow::backgroundColourId);

    if (fileIsBeingDragged)
    {
        backgroundColour = backgroundColour.brighter(0.05f);
    }

    g.fillAll(backgroundColour);
}

/**
 * @brief Resize handler
 */
void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    tabComponent.setBounds(bounds);
}

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
void MainComponent::filesDropped(const juce::StringArray& files, int /*x*/, int /*y*/)
{
    const auto& fileName = files[0];
    jassert(fileName.endsWithIgnoreCase(".xml"));

    configValueTree->loadFromFile(juce::File(fileName));

    fileIsBeingDragged = false;
    repaint();
}

/**
 * @brief   Implements juce::FileDragAndDropTarget::fileDragEnter()
 */
void MainComponent::fileDragEnter(const juce::StringArray& /*files*/, int /*x*/, int /*y*/)
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

/**
 * @brief Implements juce::ValueTree::Listener::valueTreeRedirected()
 */
void MainComponent::valueTreeRedirected(juce::ValueTree& redirectedTree)
{
    // if (redirectedTree == configValueTree->getRoot())
    // {
    //     auto torqueMap = configValueTree->getChildWithName(ConfigurationValueTree::Children::TorqueMap);

    //     // TODO: this should be replaced by something (1) faster (2) that is its own function!
    //     juce::String interpolationMethod
    //         = torqueMap.getProperty(ConfigurationValueTree::Properties::InterpolationMethod);

    //     for (int i = 0; i < interpolationCombo.getNumItems(); i++)
    //     {
    //         if (interpolationCombo.getItemText(i) == interpolationMethod)
    //         {
    //             interpolationCombo.setSelectedItemIndex(i);
    //             break;
    //         }
    //     }
    // }
}

} // namespace gui