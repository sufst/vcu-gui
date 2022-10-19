/******************************************************************************
 * @file   Interpolator.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Interpolation algorithms
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion",
                                    "-Wextra-semi",
                                    "-Wmissing-prototypes",
                                    "-Wc++98-compat-extra-semi")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4267 4068)

#ifndef NDEBUG
    #define NDEBUG // prevent assert()
    #include <spline.h>
    #undef NDEBUG
#else
    #include <spline.h>
#endif

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC

#include "linspace.h"
#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

namespace utility
{

//==============================================================================

/**
 * @brief   Base class for interpolation algorithms
 *
 * @tparam  ValueType   Type for data points to interpolate
 */
template <typename ValueType>
class Interpolator
{
public:

    //==========================================================================
    Interpolator() = default;
    virtual ~Interpolator() = default;

    //==========================================================================
    /**
     * @brief       Processes the interpolation for a set of samples
     *
     * @param[in]   inputSamples        Input samples
     * @param[in]   numOutputSamples    The number of output samples to generate
     */
    virtual void
    process(const juce::Array<juce::Point<ValueType>>& inputSamples,
            int numOutputSamples)
    {
        jassert(inputSamples.size() >= 2);

        if (!this->getCacheValid())
        {
            prepare(inputSamples);
            this->resetSamples(numOutputSamples);

            auto outputX = linspace<ValueType>(inputSamples.getFirst().getX(),
                                               inputSamples.getLast().getX(),
                                               numOutputSamples);

            int leftIndex = 0;
            int rightIndex = 1;

            for (const auto& x : outputX)
            {
                while (x >= inputSamples[rightIndex].getX()
                       && rightIndex != inputSamples.size())
                {
                    leftIndex++;
                    rightIndex++;
                }

                const auto& p1 = inputSamples[leftIndex];
                const auto& p2 = inputSamples[rightIndex];

                const auto y = interpolate(x, p1, p2);

                this->outputSamples.add({x, y});
            }

            this->outputSamples.add(inputSamples.getLast());
            this->setCacheValid(true);
        }
    }

    //==========================================================================
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
     * @brief Invalidates the cache to cause values to be recomputed on next
     * call to process
     */
    void invalidateCache()
    {
        setCacheValid(false);
    }

protected:

    //==========================================================================
    /**
     * @brief       Internal function implemented by derived classes to compute
     * an interpolated value between two points
     *
     * @param[in]   input       Input to interpolate
     * @param[in]   leftPoint   Nearest point with x-coordinate to left of input
     * @param[in]   rightPoint  Nearest point with x-coordinate to right of
     * input
     */
    virtual ValueType interpolate(ValueType input,
                                  juce::Point<ValueType> leftPoint,
                                  juce::Point<ValueType> rightPoint)
        = 0;

    /**
     * @brief       Internal function implemented by derived classes to prepare
     * for calls to interpolate()
     *
     * @param[in]   inputSamples    The input samples for interpolation
     */
    virtual void
    prepare(const juce::Array<juce::Point<ValueType>>& inputSamples)
        = 0;

    /**
     * @brief       Resets the sample cache and ensures enough output samples
     * are allocated for the next round of interpolation
     *
     * @param[in]   numSamples  Number of output samples required
     */
    void resetSamples(int numSamples)
    {
        outputSamples.clearQuick();
        outputSamples.ensureStorageAllocated(numSamples);
    }

    //==========================================================================

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

    //==========================================================================

    /**
     * @brief Cache of output samples
     */
    juce::Array<juce::Point<ValueType>> outputSamples;

private:

    //==========================================================================
    bool cacheValid = false;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Interpolator)
};

//==============================================================================

/**
 * @brief   Simple linear interpolator
 *
 * @details Interpolates by creating straight lines between each pair of points
 */
template <typename ValueType>
class LinearInterpolator : public Interpolator<ValueType>
{
public:

    //==========================================================================
    /**
     * @brief Implements Interpolator::prepare()
     */
    void prepare(
        const juce::Array<juce::Point<ValueType>>& /*inputSamples*/) override
    {
        // nothing to do
    }

    /**
     * @brief Implements Interpolator::interpolate()
     */
    ValueType interpolate(ValueType input,
                          juce::Point<ValueType> leftPoint,
                          juce::Point<ValueType> rightPoint) override
    {
        const auto xDiff
            = static_cast<float>(rightPoint.getX() - leftPoint.getX());
        const auto yDiff
            = static_cast<float>(rightPoint.getY() - leftPoint.getY());
        const auto inDiff = static_cast<float>(input - leftPoint.getX());

        const double mu = inDiff / xDiff;
        return static_cast<ValueType>(leftPoint.getY() + mu * yDiff);
    }

    //==========================================================================
    /**
     * @brief Identifier / name for algorithm
     */
    inline static const juce::Identifier identifier = "Linear";
};

//==============================================================================

/**
 * @brief   Cosine interpolator
 *
 * @details Interpolates by drawing a half-cosine wave between each pair of
 * points
 */
template <typename ValueType>
class CosineInterpolator : public Interpolator<ValueType>
{
public:

    //==========================================================================
    /**
     * @brief Implements Interpolator::prepare()
     */
    void prepare(
        const juce::Array<juce::Point<ValueType>>& /*inputSamples*/) override
    {
        // nothing to do
    }

    /**
     * @brief Implements Interpolator::interpolate()
     */
    ValueType interpolate(ValueType input,
                          juce::Point<ValueType> leftPoint,
                          juce::Point<ValueType> rightPoint) override
    {
        const ValueType xDiff = rightPoint.getX() - leftPoint.getX();

        const double mu = static_cast<double>(input - leftPoint.getX()) / xDiff;
        const double mu2
            = (1 - std::cos(mu * juce::MathConstants<double>::pi)) / 2;

        return static_cast<ValueType>(leftPoint.getY() * (1 - mu2)
                                      + rightPoint.getY() * mu2);
    }

    //==========================================================================
    /**
     * @brief Identifier / name for algorithm
     */
    inline static const juce::Identifier identifier = "Cosine";
};

//==============================================================================

/**
 * @brief   Spline interpolator
 *
 * @details Interpolates using a C2 spline
 */
template <typename ValueType>
class SplineInterpolator : public Interpolator<ValueType>
{
public:

    //==========================================================================
    /**
     * @brief Implements Interpolator::prepare()
     */
    void
    prepare(const juce::Array<juce::Point<ValueType>>& inputSamples) override
    {
        const size_t numInputSamples = static_cast<size_t>(inputSamples.size());

        xInputs.resize(numInputSamples);
        yInputs.resize(numInputSamples);

        for (unsigned i = 0; i < numInputSamples; i++)
        {
            const auto& inputPoint = inputSamples[static_cast<int>(i)];

            xInputs[i] = inputPoint.getX();
            yInputs[i] = inputPoint.getY();

            // hack to enforce strict monotonicity in x
            if (i != 0 && xInputs[i - 1] == xInputs[i])
            {
                xInputs[i] += 1;
            }
        }

        spline = tk::spline(xInputs,
                            yInputs,
                            getRequiredSplineType(numInputSamples));
    }

    /**
     * @brief Implements Interpolator::interpolate()
     */
    ValueType interpolate(ValueType input,
                          juce::Point<ValueType> /*leftPoint*/,
                          juce::Point<ValueType> /*rightPoint*/) override
    {
        return static_cast<ValueType>(spline(input));
    }

    //==========================================================================
    /**
     * @brief Identifier / name for algorithm
     */
    inline static const juce::Identifier identifier = "Spline";

private:

    //==========================================================================
    /**
     * @brief       Determine the required type of spline given the number of
     * input samples
     *
     * @details     Two points is not enough for a C2 spline, so in this case
     * defaulting to linear is a sensible alternative
     *
     * @param[in]   numInputSamples     Number of input samples
     */
    tk::spline::spline_type getRequiredSplineType(size_t numInputSamples) const
    {
        using spline_type = tk::spline::spline_type;
        return (numInputSamples > 2) ? spline_type::cspline
                                     : spline_type::linear;
    }

    //==========================================================================
    std::vector<double> xInputs;
    std::vector<double> yInputs;
    tk::spline spline;
};

//==============================================================================

/**
 * @brief Factory for creating interpolators from their identifiers
 */
template <typename ValueType>
class InterpolatorFactory
{
public:

    //==========================================================================
    /**
     * @brief       Creates an interpolator
     *
     * @param[in]   identifier  Identifier for the interpolator
     */
    static std::unique_ptr<Interpolator<ValueType>>
    makeInterpolator(const juce::Identifier& identifier)
    {
        // TODO: this is probably fine for just 3 interpolator types, but could
        // be made more efficient / compact

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

    //==========================================================================
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

    //==========================================================================
    InterpolatorFactory() = delete;
};

} // namespace utility