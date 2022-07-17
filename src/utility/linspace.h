/******************************************************************************
 * @file   linspace.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Function analogous to linspace() in MATLAB / numpy
 *****************************************************************************/

#pragma once

#include <vector>

/**
 * @brief       Creates a vector of linearly spaced values
 * 
 * @note        When used with integers, rounding may result in duplicate points or slightly non-linear spacing,
 *              depending on the number and range of values. Choose parameters carefully for sensible results.
 * 
 * @see         Heavily based on various gists on GitHub, see:
 *              - https://gist.github.com/lorenzoriano/5414671
 *              - https://gist.github.com/parequena/689dbfe2a4fca9c8203c76ab715bf75c
 * 
 * @param[in]   start   First value in the space
 * @param[in]   end     Last value in the space
 * @param[in]   num     Total number of values in the space
 */
template <typename T>
std::vector<T> linspace(T start, T end, int num)
{
    std::vector<T> linspaced;
    linspaced.reserve(static_cast<size_t>(num));

    if (num == 0)
    {
        return linspaced;
    }

    if (num == 1) 
    {
        linspaced.push_back(static_cast<T>(start));
    }
    else
    {
        double delta = static_cast<double>(end - start) / (num - 1);

        for (int i = 0; i < (num - 1); i++)
        {
            linspaced.push_back(static_cast<T>(start + delta * i));
        }

        linspaced.push_back(static_cast<T>(end));
    }

    return linspaced;
}