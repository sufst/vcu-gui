/******************************************************************************
 * @file   Interpolator.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Interpolation algorithm
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace utility
{

//------------------------------------------------------------------------- base

/**
 * @brief   Base class for interpolation algorithms
 * 
 * @tparam  ValueType   Type for data points to interpolate
 */
template <typename ValueType>
class Interpolator
{
public:

    Interpolator() = default;

    /**
     * @brief       Processes the interpolation for a set of samples
     * 
     * @param[in]   inputSamples    Input samples
     * @param[in]   speedRatio      Increase in sample rate
     */
    virtual void process(const juce::Array<juce::Point<ValueType>>& inputSamples, int speedRatio) = 0;

    /**
     * @brief Returns the interpolated points
     */
    virtual const juce::Array<juce::Point<ValueType>>& getInterpolatedPoints() const = 0;

    /**
     * @brief Invalidates the cache to cause values to be recomputed on next call to process
     */
    void invalidateCache()
    {
        setCacheValid(false);
    }

protected:

    /**
     * @brief       Validates or invalidates the cache
     * 
     * @param[in]   isValid     Whether or not the cache is valid
     */
    void setCacheValid(bool isValid = true)
    {
        cacheValid = isValid;
    }

    /**
     * @brief Returns true if the cache is valid, false otherwise
     */
    bool getCacheValid() const   
    {
        return cacheValid;
    }

private:

    bool cacheValid = false;
};

//----------------------------------------------------------------------- linear

/**
 * @brief   A simple linear interpolator
 */
template <typename ValueType>
class LinearInterpolator : public Interpolator<ValueType>
{
public:

    /**
     * @brief Implements Interpolator::process()
     */
    void process(const juce::Array<juce::Point<ValueType>>& inputSamples, int speedRatio) override
    {
        if (!this->getCacheValid())
        {
            // pre-alloc for efficiency
            const auto numOutputSamples = inputSamples.size() * speedRatio;
            outputSamples.clearQuick();
            outputSamples.ensureStorageAllocated(numOutputSamples);

            // linear interp
            for (int i = 0; i < inputSamples.size() - 1; i++)
            {
                const auto& p1 = inputSamples[i];
                const auto& p2 = inputSamples[i + 1];

                const ValueType xDiff = p2.getX() - p1.getX();
                const ValueType yDiff = p2.getY() - p1.getY();

                for (int j = 0; j < speedRatio; j++)
                {
                    const float mu = static_cast<float>(j) / speedRatio;
                    auto x = static_cast<ValueType>(p1.getX() + mu * xDiff);
                    auto y = static_cast<ValueType>(p1.getY() + mu * yDiff);

                    outputSamples.add({x, y});
                }
            }

            outputSamples.add(inputSamples.getLast());

            this->setCacheValid(true);
        }
    }

    /**
     * @brief Implements Interpolator::getInterpolatedPoints()
     */
    const juce::Array<juce::Point<ValueType>>& getInterpolatedPoints() const override 
    {
        return outputSamples;
    }

private:
    juce::Array<juce::Point<ValueType>> outputSamples;
};

//----------------------------------------------------------------------- cosine
//---------------------------------------------------------------------- hermite
//--------------------------------------------------------------------------- c2

} // utility