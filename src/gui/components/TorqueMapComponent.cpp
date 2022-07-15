/******************************************************************************
 * @file   TorqueMapComponent.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Component for drawing the torque map
 *****************************************************************************/

#include "TorqueMapComponent.h"

namespace gui
{

/**
 * @brief Constructor
 */
TorqueMapComponent::TorqueMapComponent(VCUConfiguration& config)
{
    torqueMapData = config.getTorqueMap();

    setRangeX(0, 1000);
    setRangeY(0, 1000);
}

} // namespace gui
