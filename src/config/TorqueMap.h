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

    //==========================================================================
    TorqueMapPoint(const juce::ValueTree& v);

    //==========================================================================
    using ConstrainerType = RangeConstrainer<RawValueType, 0, 1023>;
    using ValueType = ConstrainerWrapper<RawValueType, ConstrainerType>;

    juce::ValueTree state;
    juce::CachedValue<ValueType> input;
    juce::CachedValue<ValueType> output;
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