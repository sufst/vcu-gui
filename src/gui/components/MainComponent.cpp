/******************************************************************************
 * @file   MainComponent.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Main component
 *****************************************************************************/

#include "MainComponent.h"
#include "../../Application.h"

namespace gui
{

/**
 * @brief Default constructor
 */
MainComponent::MainComponent()
    : torqueMapGraph(Application::getConfig())
{
    setSize(600, 400);
    addAndMakeVisible(torqueMapGraph);
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
    // fill background
    g.fillAll(
        getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

/**
 * @brief Resize handler
 */
void MainComponent::resized()
{
    auto bounds = getLocalBounds();

    torqueMapGraph.setBounds(bounds.reduced(20));
}

} // namespace gui