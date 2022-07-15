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

class TorqueMapComponent : public GraphComponent<int>
{
public:

    TorqueMapComponent(VCUConfiguration& config);

private:

    juce::Point<int> transformPointToGraph(const juce::Point<int>& point) const;

    juce::ValueTree torqueMapData;
};

} // namespace gui