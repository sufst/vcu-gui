/******************************************************************************
 * @file   PointComparator.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Element comparator for juce::Point<T>
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace gui
{

/**
 * @brief   Implements juce::ElementComparator for two juce::Point objects
 * 
 * @tparam  ValueType   Type of point data
 */
template<typename ValueType>
class PointComparator
{
public:

    /**
     * @brief       Compares two points based on their x-coordinate
     * 
     * @param[in]   p1      First point
     * @param[in]   p2      Second point
     */
    static int compareElements(const juce::Point<ValueType>& p1, const juce::Point<ValueType>& p2)
    {
        if (p1.getX() < p2.getX())
        {
            return -1;
        }
        else if (p1.getX() > p2.getX())
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
};

}