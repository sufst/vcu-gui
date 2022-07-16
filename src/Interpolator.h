/******************************************************************************
 * @file   Interpolator.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Interpolation algorithms
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

#pragma GCC diagnostic push // disable warnings from external lib
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wextra-semi"
#pragma GCC diagnostic ignored "-Wc++98-compat-extra-semi"
#define NDEBUG // prevent assert() in lib
#include <spline.h>
#undef NDEBUG
#pragma GCC diagnostic pop

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

namespace utility
{

//----------------------------------------------------------------------------------------------------------------- base

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
    virtual ~Interpolator() = default;

    /**
     * @brief       Processes the interpolation for a set of samples
     *
     * @param[in]   inputSamples    Input samples
     * @param[in]   speedRatio      Increase in sample rate
     */
    virtual void process(const juce::Array<juce::Point<ValueType>>& inputSamples, int speedRatio) = 0;

    /**
     * @brief   Returns the interpolated points
     *
     * @note    Call process() first
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
     * @brief Cache of output samples
     */
    juce::Array<juce::Point<ValueType>> outputSamples;

private:

    bool cacheValid = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Interpolator)
};

//--------------------------------------------------------------------------------------------------------------- linear

/**
 * @brief   Simple linear interpolator
 *
 * @details Interpolates by creating straight lines between each pair of points
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

                    this->outputSamples.add({x, y});
                }
            }

            this->outputSamples.add(inputSamples.getLast());
            this->setCacheValid(true);
        }
    }

    static const juce::Identifier identifier;
};

template <typename ValueType>
const juce::Identifier LinearInterpolator<ValueType>::identifier = "Linear";

//--------------------------------------------------------------------------------------------------------------- cosine

/**
 * @brief   Cosine interpolator
 *
 * @details Interpolates by drawing a half-cosine wave between each pair of points
 */
template <typename ValueType>
class CosineInterpolator : public Interpolator<ValueType>
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

                this->outputSamples.add({x, y});
            }
        }

        this->outputSamples.add(inputSamples.getLast());
        this->setCacheValid(true);
    }

    static const juce::Identifier identifier;
};

template <typename ValueType>
const juce::Identifier CosineInterpolator<ValueType>::identifier = "Cosine";

//--------------------------------------------------------------------------------------------------------------- spline

/**
 * @brief   Spline interpolator
 *
 * @details Interpolates using a C2 spline
 */
template <typename ValueType>
class SplineInterpolator : public Interpolator<ValueType>
{
public:

    /**
     * @brief Implements Interpolator::process()
     */
    void process(const juce::Array<juce::Point<ValueType>>& inputSamples, int speedRatio) override
    {
        const int numInputSamples = inputSamples.size();
        const int numOutputSamples = numInputSamples * speedRatio;

        this->resetSamples(numOutputSamples);

        std::vector<double> xInputs; // spline lib requires double
        std::vector<double> yInputs;
        xInputs.reserve(static_cast<size_t>(numInputSamples));
        yInputs.reserve(static_cast<size_t>(numInputSamples));

        std::for_each(inputSamples.begin(),
                      inputSamples.end(),
                      [&xInputs, &yInputs](const juce::Point<ValueType>& point)
                      {
                          // hack to enforce strict monotonicity
                          if (xInputs.back() == point.getX())
                          {
                              xInputs.push_back(point.getX() + 1);
                          }
                          else
                          {
                              xInputs.push_back(point.getX());
                          }

                          yInputs.push_back(point.getY());
                      });

        tk::spline spline(xInputs, yInputs, getRequiredSplineType(numInputSamples));

        for (int i = 0; i < numInputSamples - 1; i++)
        {
            const auto& p1 = inputSamples[i];
            const auto& p2 = inputSamples[i + 1];

            const ValueType xDiff = p2.getX() - p1.getX();

            for (int j = 0; j < speedRatio; j++)
            {
                const float mu = static_cast<float>(j) / speedRatio;

                auto x = static_cast<ValueType>(p1.getX() + mu * xDiff);
                auto y = static_cast<ValueType>(spline(x));

                this->outputSamples.add({x, y});
            }
        }
    }

    static const juce::Identifier identifier;

private:

    /**
     * @brief       Determine the required type of spline given the number of input samples
     *
     * @details     Two points is not enough for a C2 spline, so in this case defaulting to linear is a sensible
     *              alternative
     *
     * @param[in]   numInputSamples     Number of input samples
     */
    tk::spline::spline_type getRequiredSplineType(int numInputSamples) const
    {
        using spline_type = tk::spline::spline_type;
        return (numInputSamples > 2) ? spline_type::cspline : spline_type::linear;
    }
};

template <typename ValueType>
const juce::Identifier SplineInterpolator<ValueType>::identifier = "Spline";

//-------------------------------------------------------------------------------------------------------------- factory

/**
 * @brief Factory for creating interpolators from their identifiers
 */
template <typename ValueType>
class InterpolatorFactory
{
public:

    /**
     * @brief       Creates an interpolator
     *
     * @param[in]   identifier  Identifier for the interpolator
     */
    static std::unique_ptr<Interpolator<ValueType>> makeInterpolator(const juce::Identifier& identifier)
    {
        // TODO: this is probably fine for just 3 interpolator types, but could be made more efficient / compact

        if (identifier == LinearInterpolator<ValueType>::identifier)
        {
            return std::make_unique<LinearInterpolator<ValueType>>();
        }

        if (identifier == CosineInterpolator<ValueType>::identifier)
        {
            return std::make_unique<CosineInterpolator<ValueType>>();
        }

        if (identifier == SplineInterpolator<ValueType>::identifier)
        {
            return std::make_unique<SplineInterpolator<ValueType>>();
        }

        return nullptr;
    }

    /**
     * @brief Returns all valid identifiers for interpolator classes
     */
    static std::array<juce::Identifier, 3> getAllIdentifiers()
    {
        return {LinearInterpolator<ValueType>::identifier,
                CosineInterpolator<ValueType>::identifier,
                SplineInterpolator<ValueType>::identifier};
    }

private:

    InterpolatorFactory() = delete;
};

} // namespace utility