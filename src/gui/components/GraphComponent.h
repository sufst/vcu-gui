/******************************************************************************
 * @file   GraphComponent.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @date   2022-07-11
 * @brief  Component for drawing a graph
 *****************************************************************************/

#include <JuceHeader.h>

#include <memory>

namespace gui
{

/**
 * @brief   Graph drawing component
 */
class GraphComponent : public juce::Component
{
public:

    GraphComponent();
    GraphComponent(std::shared_ptr<juce::Path> path);

    void setPath(std::shared_ptr<juce::Path> newPath);
    void setRangeX(float min, float max);
    void setRangeY(float min, float max);

    void paint(juce::Graphics& g) override;

private:

    void paintBackground(juce::Graphics& g) const;
    void paintTicks(juce::Graphics& g) const;
    void paintBorder(juce::Graphics& g) const;
    void paintCurve(juce::Graphics& g);

    juce::Rectangle<float> valueBounds;
    std::shared_ptr<juce::Path> sharedPath;
};

} // namespace gui