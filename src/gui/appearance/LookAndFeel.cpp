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
    static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::KarlaLight_ttf, 
                                                                   BinaryData::KarlaLight_ttfSize);
    return typeface;
}


} // namespace sufst