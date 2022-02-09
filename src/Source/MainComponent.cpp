/******************************************************************************
 * @file   MainComponent.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Main component
 *****************************************************************************/

#include "MainComponent.h"
#include "ThrottleCurve.h"

//==================================================== Constructor / destructor

/**
 * @brief Default constructor
 */
MainComponent::MainComponent()
{
    setSize(600, 400);
    
    // setup combo box
    interpolationMethodComboBox.setTitle("Interpolation type");
    
    const auto& interpolationMethods = ThrottleCurve::getAllInterpolationMethods();
    
    for (int i = 0; i < interpolationMethods.size(); i++)
    {
        // create and add item
        const auto& method = interpolationMethods.getReference(i);
        juce::String methodName = ThrottleCurve::getInterpolationMethodName(method);
        interpolationMethodComboBox.addItem(methodName, i + 1);
        
        // select the default item
        if (method == ThrottleCurve::getDefaultInterpolationMethod())
        {
            interpolationMethodComboBox.setSelectedId(i + 1);
        }
    }
    
    interpolationMethodComboBox.onChange = [this] () {
        int selectedItem = interpolationMethodComboBox.getSelectedItemIndex();
        auto& method = ThrottleCurve::getAllInterpolationMethods().getReference(selectedItem);
        throttleCurveComponent.setInterpolationMethod(method);
    };
    
    // set up buttons
    importProfileButton.setButtonText("Import profile");
    exportProfileButton.setButtonText("Export profile");
    exportCodeButton.setButtonText("Export code");
    
    importProfileButton.onClick = [this] () {
        throttleCurveComponent.importProfile();
    };
    
    exportProfileButton.onClick = [this] () {
        throttleCurveComponent.exportProfile();
    };
    
    exportCodeButton.onClick = [this] () {
        throttleCurveComponent.exportCode();
    };
    
    // add children
    addAndMakeVisible(throttleCurveComponent);
    addAndMakeVisible(interpolationMethodComboBox);
    addAndMakeVisible(importProfileButton);
    addAndMakeVisible(exportProfileButton);
    addAndMakeVisible(exportCodeButton);
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
void MainComponent::paint(juce::Graphics& g)
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
    
    // border
    bounds = bounds.reduced(borderSize);
    
    // reserve a small bar at the bottom
    auto lowerBounds = bounds.removeFromBottom(borderSize * 2);
    lowerBounds.removeFromTop(borderSize / 4);
    lowerBounds.removeFromBottom(borderSize / 4);
    
    // add user inputs at the bottom
    auto spacing = borderSize / 4;
    auto width = (lowerBounds.getWidth() - spacing) / 4;
    
    interpolationMethodComboBox.setBounds(lowerBounds.removeFromLeft(width));
    lowerBounds.removeFromLeft(spacing);
    
    importProfileButton.setBounds(lowerBounds.removeFromLeft(width));
    lowerBounds.removeFromLeft(spacing);
    
    exportProfileButton.setBounds(lowerBounds.removeFromLeft(width));
    lowerBounds.removeFromLeft(spacing);
    
    exportCodeButton.setBounds(lowerBounds.removeFromLeft(width));
    
    // use the rest for the graph
    throttleCurveComponent.setBounds(bounds);
}
