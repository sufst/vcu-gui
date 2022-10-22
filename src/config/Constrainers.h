/******************************************************************************
 * @file    Constrainers.h
 * @author  Tim Brewis (@t-bre)
 * @author  David Rowland (@drowaudio) - see @author and @copyright on classes
 * @brief   Constrainers for parameters
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace config
{

//==============================================================================

/**
 * @brief       Wrapper around constrainers
 *
 * @tparam      ValueType       Type of value for constrainer
 * @tparam      Constrainer     Constrainer
 *
 * @author      David Rowland (@drowaudio)
 *
 * @copyright   See below (TODO: this needs to be distributed w/ next release)
 *
 * MIT License
 *
 * Copyright (c) 2016 drowaudio
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
template <typename ValueType, typename Constrainer>
struct ConstrainerWrapper
{
    //==========================================================================

    ConstrainerWrapper() = default;

    template <typename OtherType>
    ConstrainerWrapper(const OtherType& other)
    {
        value = Constrainer::constrain(other);
    }

    ConstrainerWrapper(const ConstrainerWrapper& other)
    {
        value = other.value;
    }

    //==========================================================================

    ConstrainerWrapper& operator=(const ConstrainerWrapper& other) noexcept
    {
        value = Constrainer::constrain(other.value);
        return *this;
    }

    bool operator==(const ConstrainerWrapper& other) const noexcept
    {
        return value == other.value;
    }
    bool operator!=(const ConstrainerWrapper& other) const noexcept
    {
        return value != other.value;
    }

    //==========================================================================

    operator var() const noexcept
    {
        return Constrainer::constrain(value);
    }

    operator Type() const noexcept
    {
        return Constrainer::constrain(value);
    }

    //==========================================================================
    ValueType value = ValueType();
};

//==============================================================================

/**
 * @brief       Constrains a value to a range
 *
 * @tparam      ValueType   Type of value to constrain
 * @tparam      MinValue    Minimum of range
 * @tparam      MaxValue    Maximum of range
 *
 * @author      David Rowland (@drowaudio)
 *
 * @copyright   See below (TODO: this needs to be distributed w/ next release)
 *
 * MIT License
 *
 * Copyright (c) 2016 drowaudio
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
template <typename ValueType, int MinValue, int MaxValue>
struct RangeConstrainer
{
    static ValueType constrain(const Type& value)
    {
        const ValueType min = static_cast<ValueType>(MinValue);
        const ValueType max = static_cast < ValueType(MaxValue);

        return juce::Range<ValueType>(min, max).clipValue(value);
    }
};

} // namespace config