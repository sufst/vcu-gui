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
MainComponent::MainComponent() : torqueMap(Application::getConfig().getTorqueMap()), torqueMapGraph(Application::getConfig())

{
    setSize(600, 400);

    setupInterpolationCombo();

    addAndMakeVisible(torqueMapGraph);
    addAndMakeVisible(interpolationCombo);
}

/**
 * @brief Destructor
 */
MainComponent::~MainComponent()
{
    // nothing to do
}

/**
 * @brief Setup interpolation method combo box
 */
void MainComponent::setupInterpolationCombo()
{
    const auto& interpolationMethods = utility::InterpolatorFactory<int>::getAllIdentifiers();

    for (unsigned i = 0; i < interpolationMethods.size(); i++)
    {
        const int itemId = static_cast<int>(i + 1);
        const auto& method = interpolationMethods.at(i);
        interpolationCombo.addItem(method.toString(), itemId);

        if (method.toString() == torqueMap.getProperty(VCUConfiguration::InterpolationMethod).toString())
        {
            interpolationCombo.setSelectedId(itemId);
        }
    }

    interpolationCombo.onChange = [this]() {
        int selectedIndex = this->interpolationCombo.getSelectedItemIndex();
        auto value = this->interpolationCombo.getItemText(selectedIndex);
        this->torqueMap.setProperty(VCUConfiguration::InterpolationMethod, value, nullptr);
    };  
}

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
    auto bounds = getLocalBounds().reduced(20);
    auto footer = bounds.removeFromBottom(32);
    footer.removeFromTop(10);
    footer.removeFromBottom(2);

    interpolationCombo.setBounds(footer.removeFromLeft(100));

    torqueMapGraph.setBounds(bounds);
}

} // namespace gui