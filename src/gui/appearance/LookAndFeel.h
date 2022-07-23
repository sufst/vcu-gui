/******************************************************************************
 * @file   LookAndFeel.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Look and feel
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace sufst
{

/**
 * @brief Look and feel derived from the JUCE default
 */
class LookAndFeel : public juce::LookAndFeel_V4
{
public:

    LookAndFeel();

private:

    static const juce::Typeface::Ptr getKarlaTypeface();

    JUCE_DECLARE_NON_COPYABLE(LookAndFeel)
};

} // namespace sufst