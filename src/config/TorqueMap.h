/******************************************************************************
 * @file    TorqueMap.h
 * @author  Tim Brewis (@t-bre)
 * @brief   Data model for torque map
 *****************************************************************************/

#pragma once

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
    using RawValueType = int;

    static constexpr const int MinInput = 0;
    static constexpr const int MaxInput = (1 << 10) - 1;
    static constexpr const int MinOutput = 0;
    static constexpr const int MaxOutput = (1 << 15) - 1;

    //==========================================================================
    TorqueMapPoint(const juce::ValueTree& v);

    //==========================================================================
    using InputConstrainerType
        = RangeConstrainer<RawValueType, MinInput, MaxInput>;
    using OutputConstrainerType
        = RangeConstrainer<RawValueType, MinInput, MaxOutput>;
    using InputValueType
        = ConstrainerWrapper<RawValueType, InputConstrainerType>;
    using OutputValueType
        = ConstrainerWrapper<RawValueType, OutputConstrainerType>;

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
    bool isSuitableType(const ValueTree& v) const override;
    TorqueMapPoint* createNewObject(const juce::ValueTree& v) override;
    void deleteObject(TorqueMapPoint* p) override;
    void newObjectAdded(TorqueMapPoint*) override;
    void objectRemoved(TorqueMapPoint*) override;
    void objectOrderChanged() override;
};

} // namespace config