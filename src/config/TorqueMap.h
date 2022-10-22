/******************************************************************************
 * @file    TorqueMap.h
 * @author  Tim Brewis (@t-bre)
 * @brief   Data model for torque map
 *****************************************************************************/

#pragma once

#include "../utility/Interpolator.h"
#include "Constrainers.h"
#include "Identifiers.h"
#include "ValueTreeObjectList.h"
#include <JuceHeader.h>

namespace config
{

//==============================================================================

/**
 * @brief   A point on the torque map
 */
struct TorqueMapPoint : public ReferenceCountedObject
{
    using ValueType = int;

    static constexpr const int InputResolution = 10;
    static constexpr const int OutputResolution = 15;
    static constexpr const int MinInput = 0;
    static constexpr const int MaxInput = (1 << InputResolution) - 1;
    static constexpr const int MinOutput = 0;
    static constexpr const int MaxOutput = (1 << OutputResolution) - 1;

    //==========================================================================
    TorqueMapPoint(const juce::ValueTree& v);

    //==========================================================================
    using InputConstrainerType
        = RangeConstrainer<ValueType, MinInput, MaxInput>;
    using OutputConstrainerType
        = RangeConstrainer<ValueType, MinInput, MaxOutput>;
    using InputValueType = ConstrainerWrapper<ValueType, InputConstrainerType>;
    using OutputValueType
        = ConstrainerWrapper<ValueType, OutputConstrainerType>;

    juce::ValueTree state;
    juce::CachedValue<InputValueType> input;
    juce::CachedValue<OutputValueType> output;
};

//==============================================================================

/**
 * @brief   Torque map
 */
struct TorqueMap : public ValueTreeObjectList<TorqueMapPoint>
{
    //==========================================================================
    TorqueMap(const juce::ValueTree& v);
    ~TorqueMap() override;

    //==========================================================================
    void addPoint(TorqueMapPoint::ValueType input,
                  TorqueMapPoint::ValueType output);
    juce::Array<TorqueMapPoint*> getPoints();
    void removePoint(TorqueMapPoint& point);

    //==========================================================================
    bool isSuitableType(const ValueTree& v) const override;
    TorqueMapPoint* createNewObject(const juce::ValueTree& v) override;
    void deleteObject(TorqueMapPoint* p) override;
    void newObjectAdded(TorqueMapPoint*) override;
    void objectRemoved(TorqueMapPoint*) override;
    void objectOrderChanged() override;

    //==========================================================================
    using InterpolatorConstrainerType
        = ConstrainerWrapper<juce::String,
                             InterpolatorNameConstrainer<
                                 TorqueMapPoint::ValueType>>; // formatting bad?

    juce::CachedValue<InterpolatorConstrainerType> interpolationMethod;
    juce::ValueTree state;
};

} // namespace config