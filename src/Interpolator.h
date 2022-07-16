/******************************************************************************
 * @file   Interpolator.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Interpolation algorithms
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

#include <cmath>

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
    const juce::Array<juce::Point<ValueType>>& getInterpolatedPoints() const 
    {
        return outputSamples;
    }

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

    /**
     * @brief       Resets the sample cache and ensures enough output samples are allocated for the next round of
     *              interpolation
     * 
     * @param[in]   numSamples  Number of output samples required
     */
    void resetSamples(int numSamples)
    {
        outputSamples.clearQuick();
        outputSamples.ensureStorageAllocated(numSamples);
    }

    /**
     * @brief Adds a new output sample to the cache
     */
    void addOutputSample(const juce::Point<ValueType>& sample)
    {
        outputSamples.add(sample);
    }

private:

    bool cacheValid = false;
    juce::Array<juce::Point<ValueType>> outputSamples;
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
            this->resetSamples(inputSamples.size() * speedRatio);

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

                    Interpolator<ValueType>::addOutputSample({x, y});
                }
            }

            this->addOutputSample(inputSamples.getLast());
            this->setCacheValid(true);
        }
    }
};

//----------------------------------------------------------------------- cosine
template <typename ValueType>
class CosineInterpolator : public Interpolator <ValueType>
{
public:

    /**
     * @brief Implements Interpolator::process()
     */
    void process(const juce::Array<juce::Point<ValueType>>& inputSamples, int speedRatio) override
    {
        this->resetSamples(inputSamples.size() * speedRatio);

        for (int i = 0; i < inputSamples.size() - 1; i++)
        {
            const auto& p1 = inputSamples[i];
            const auto& p2 = inputSamples[i + 1];

            const ValueType xDiff = p2.getX() - p1.getX();
            // const ValueType yDiff = p2.getY() - p1.getY();

            for (int j = 0; j < speedRatio; j++)
            {
                const float mu = static_cast<float>(j) / speedRatio;
                const float mu2 = (1 - std::cos(mu * juce::MathConstants<float>::pi)) / 2;

                auto x = static_cast<ValueType>(p1.getX() + mu * xDiff);
                auto y = static_cast<ValueType>(p1.getY() * (1 - mu2) + p2.getY() * mu2);

                this->addOutputSample({x, y});
            }
        }

        this->addOutputSample(inputSamples.getLast());
        this->setCacheValid(true);
    }
};

//---------------------------------------------------------------------- hermite
//--------------------------------------------------------------------------- c2

} // utility