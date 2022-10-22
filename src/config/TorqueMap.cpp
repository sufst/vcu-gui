/******************************************************************************
 * @file    TorqueMap.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "TorqueMap.h"

namespace config
{

//==============================================================================

/**
 * @brief       Creates a torque map point
 *
 * @param[in]   v   Value tree holding data
 *
 * @note        Points default to (0, 0) if their value was never set
 */
TorqueMapPoint::TorqueMapPoint(const juce::ValueTree& v) : state(v)
{
    jassert(v.hasType(IDs::TorqueMapPoint));

    input.setDefault(0);
    output.setDefault(0);
    input.referTo(state, IDs::Input, nullptr);
    output.referTo(state, IDs::Output, nullptr);
}

//==============================================================================

/**
 * @brief       Creates a torque map
 *
 * @param[in]   v   Value tree holding data
 */
TorqueMap::TorqueMap(const juce::ValueTree& v)
    : ValueTreeObjectList(v), state(v)
{
    rebuildObjects();
    interpolationMethod.referTo(state, IDs::InterpolationMethod, nullptr);
}

/**
 * @brief   Destroys the torque map
 */
TorqueMap::~TorqueMap()
{
    freeObjects();
}

/**
 * @brief       Adds a point to the torque map
 *
 * @param[in]   input   Input value of point
 * @param[in]   output  Output value of point
 */
void TorqueMap::addPoint(TorqueMapPoint::ValueType input,
                         TorqueMapPoint::ValueType output)
{
    juce::ValueTree vt(IDs::TorqueMapPoint);
    TorqueMapPoint point(vt);

    point.input.setValue(input, nullptr);
    point.output.setValue(output, nullptr);

    state.addChild(vt, -1, nullptr);
}

/**
 * @brief   Returns the list of torque map points
 */
juce::Array<TorqueMapPoint*> TorqueMap::getPoints()
{
    return objects;
}

/**
 * @brief   Removes the given point from the torque map
 */
void TorqueMap::removePoint(TorqueMapPoint& point)
{
    state.removeChild(point.state, nullptr);
}

/**
 * @brief   Implements ValueTreeObjectList::isSuitableType()
 */
bool TorqueMap::isSuitableType(const ValueTree& v) const
{
    return v.hasType(IDs::TorqueMapPoint);
}

/**
 * @brief   Implements ValueTreeObjectList::createNewObject()
 */
TorqueMapPoint* TorqueMap::createNewObject(const juce::ValueTree& v)
{
    return new TorqueMapPoint(v);
}

/**
 * @brief   Implements ValueTreeObjectList::deleteObject()
 */
void TorqueMap::deleteObject(TorqueMapPoint* p)
{
    delete p;
}

/**
 * @brief   Implements ValueTreeObjectList::newObjectAdded()
 */
void TorqueMap::newObjectAdded(TorqueMapPoint*)
{
}

/**
 * @brief   Implements ValueTreeObjectList::objectRemoved()
 */
void TorqueMap::objectRemoved(TorqueMapPoint*)
{
}

/**
 * @brief   Implements ValueTreeObjectList::objectOrderChanged()
 */
void TorqueMap::objectOrderChanged()
{
}

} // namespace config