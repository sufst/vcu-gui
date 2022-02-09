/******************************************************************************
 * @file   ThrottleCurve.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Model of a throttle curve
 *****************************************************************************/

#include "ThrottleCurve.h"

//==================================================== Constructor / destructor

/**
 * @brief Constructor
 */
ThrottleCurve::ThrottleCurve()
{
    // nothing to do
}

/**
 * @brief Destructor
 */
ThrottleCurve::~ThrottleCurve()
{
    // nothing to do
}


//============================================================== Static utility

/**
 * @brief Returns a list of allowed interpolation methods
 */
const juce::Array<ThrottleCurve::InterpolationMethod>& ThrottleCurve::getAllInterpolationMethods()
{
    return listOfInterpolationMethods;
}

/**
 * @brief Returns the name of a particular interpolation method as a string
 */
const juce::String& ThrottleCurve::getInterpolationMethodName(ThrottleCurve::InterpolationMethod method)
{
    return namesOfInterpolationMethods.getReference(static_cast<int>(method));
}

//================================================================= Static data

const juce::Array<ThrottleCurve::InterpolationMethod> ThrottleCurve::listOfInterpolationMethods = {
    ThrottleCurve::InterpolationMethod::Linear, ThrottleCurve::InterpolationMethod::Cosine, ThrottleCurve::InterpolationMethod::Bezier
};

const juce::Array<juce::String> ThrottleCurve::namesOfInterpolationMethods = {
    "Linear", "Cosine", "Bezier"
};
