/******************************************************************************
 * @file   clip.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Value clipping function
 *****************************************************************************/

#pragma once

#include <algorithm>

namespace utility
{

/**
 * @brief Clips a value to the range [min, max]
 * 
 * @tparam T    Value type
 * 
 * @param[in]   value   Value to clip
 * @param[in]   min     Lower bound on value
 * @param[in]   max     Upper bound on value
 */
template <typename T>
T clip(T value, T min, T max)
{
    return std::max(min, std::min(value, max));
}

} // namespace utility