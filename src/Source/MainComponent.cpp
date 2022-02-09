/******************************************************************************
 * @file   MainComponent.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Main component
 *****************************************************************************/

#include "MainComponent.h"

//==================================================== Constructor / destructor

/**
 * @brief Default constructor
 */
MainComponent::MainComponent()
{
    setSize(600, 400);
    addAndMakeVisible(throttleCurveComponent);
}

/**
 * @brief Destructor
 */
MainComponent::~MainComponent()
{
    // nothing to do
}

//==================================================================== Graphics

/**
 * @brief Painter
 *
 * @param[in]   g   Graphics context
 */
void MainComponent::paint (juce::Graphics& g)
{
    // fill background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

/**
 * @brief Resize handler
 */
void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    throttleCurveComponent.setBounds(bounds.reduced(20));
}
