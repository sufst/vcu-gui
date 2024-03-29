/******************************************************************************
 * @file    TabbedComponent.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "TabbedComponent.h"

namespace gui
{

//==============================================================================

/**
 * @brief   Default constructor
 */
TabbedComponent::TabbedComponent()
    : juce::TabbedComponent(juce::TabbedButtonBar::Orientation::TabsAtTop)
{
}

} // namespace gui