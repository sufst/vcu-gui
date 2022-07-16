/******************************************************************************
 * @file   TorqueMapComponent.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Component for drawing the torque map
 *****************************************************************************/

#include "../../VCUConfiguration.h"
#include "GraphComponent.h"
#include <JuceHeader.h>
#include <memory>

namespace gui
{

class TorqueMapComponent : public GraphComponent<int>, juce::ValueTree::Listener
{
public:

    TorqueMapComponent(VCUConfiguration& config);

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

    void valueTreePropertyChanged(juce::ValueTree& changedTree, const juce::Identifier& property) override;

private:

    juce::Rectangle<int> getDeadzoneBounds() const;
    void paintDeadzoneOverlay(juce::Graphics& g) const;
    bool mouseEventInDeadzone(const juce::MouseEvent& event) const;
    bool shouldPreventPointEdit(const juce::MouseEvent& event) const;
    void showDeadzoneTooltip();
    void hideDeadzoneTooltip();

    juce::ValueTree torqueMap;
    int deadzonePosition;
    bool movingDeadzone = false;
    std::unique_ptr<juce::TooltipWindow> deadzoneTooltip;

    static const int inputResolution = 10;
    static const int outputResolution = 15;
    static const int inputMax = (1 << inputResolution) - 1;
    static const int outputMax = (1 << outputResolution) - 1;
    static const int defaultDeadzone = static_cast<int>(0.05f * inputMax);
};

} // namespace gui