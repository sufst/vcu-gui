/******************************************************************************
 * @file    Colours.h
 * @author  Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief   Custom colours
 * @details These colours are taken from the theme.jsx file for the SUFST
 *          website
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace sufst
{
    namespace Colours // colours as ARGB hex digits
    {
        const juce::Colour primaryblue {0xFF0066CC};
        const juce::Colour darkblue {0xFF003366};
        const juce::Colour lightblue {0xFFF0F8FF};
        const juce::Colour white {0xFFFFFFFF};
        const juce::Colour black {0xFF000000};
        const juce::Colour lightgrey {0xFFE8E8E8};
        const juce::Colour midgrey {0xFF9B9B9B};
        const juce::Colour darkgrey {0xFF636363};
        const juce::Colour sfsyellow {0xFFFCC431};
    }

} // namespace sufst