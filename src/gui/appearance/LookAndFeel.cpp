/******************************************************************************
 * @file   LookAndFeel.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Look and feel
 *****************************************************************************/

#include "LookAndFeel.h"

namespace sufst
{

/**
 * @brief Constructor
 */
LookAndFeel::LookAndFeel()
{
    auto karla = getKarlaTypeface();

    setDefaultSansSerifTypeface(karla);
}

/**
 * @brief Loads the Karla font from the binary data library
 */
const juce::Typeface::Ptr LookAndFeel::getKarlaTypeface()
{
    static auto typeface
        = juce::Typeface::createSystemTypefaceFor(BinaryData::KarlaLight_ttf, BinaryData::KarlaLight_ttfSize);
    return typeface;
}

/**
 * @brief   Resizes tab bar buttons to equally use up available space
 *
 * @note    If tab bar buttons are used somewhere other than preferences (which
 *          is the only place they are used right now) then another solution is
 *          possibly needed.
 *
 * @return  Equal width spacing for all tabs
 */
int LookAndFeel::getTabButtonBestWidth(juce::TabBarButton& button, int /*tabDepth*/)
{
    float totalWidth = button.getTabbedButtonBar().getWidth();
    float numTabs = button.getTabbedButtonBar().getNumTabs();
    return static_cast<int>(totalWidth / numTabs);
}

} // namespace sufst