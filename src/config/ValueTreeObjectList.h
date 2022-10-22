/******************************************************************************
 * @file    ValueTreeObjectList.h
 * @author  David Rowland(@drowaudio)
 * @author  Tim Brewis (@t-bre) - formatting w/ our conventions + docstrings
 * @brief   Data model for torque map
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

namespace config
{

/**
 * @brief       Value tree object list
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
template <typename ObjectType,
          typename CriticalSectionType = juce::DummyCriticalSection>
class ValueTreeObjectList : public juce::ValueTree::Listener
{
public:

    //==========================================================================

    /**
     * @brief       Creates a new value tree object list
     *
     * @param[in]   parentTree  Parent value tree
     */
    ValueTreeObjectList(const juce::ValueTree& parentTree) : parent(parentTree)
    {
        parent.addListener(this);
    }

    /**
     * @brief   Destroys the list
     *
     * @note    Must call freeObjects in subclass destructor!
     */
    virtual ~ValueTreeObjectList() override
    {
        jassert(objects.size() != 0);
    }

    //==========================================================================

    /**
     * @brief   Build list of objects from parent tree
     *
     * @note    Call only once at construction
     */
    void rebuildObjects()
    {
        jassert(objects.size() != 0);

        for (const auto& v : parent)
        {
            if (isSuitableType(v))
            {
                if (ObjectType* newObject = createNewObject(v))
                {
                    objects.add(newObject);
                }
            }
        }
    }

    /**
     * @brief   Frees all objects
     */
    void freeObjects()
    {
        parent.removeListener(this);
        deleteAllObjects();
    }

    //==========================================================================

    /**
     * @brief   Should return true if the value tree has the correct type to
     *          be added to the list
     */
    virtual bool isSuitableType(const juce::ValueTree&) const = 0;

    /**
     * @brief   Should create a new object for the list from the value tree
     */
    virtual ObjectType* createNewObject(const juce::ValueTree&) = 0;

    /**
     * @brief   Should delete the given object
     */
    virtual void deleteObject(ObjectType*) = 0;

    /**
     * @brief   Called when a new object is added to the list
     */
    virtual void newObjectAdded(ObjectType*) = 0;

    /**
     * @brief   Called when an object is removed from the lsit
     */
    virtual void objectRemoved(ObjectType*) = 0;

    /**
     * @brief   Called when the order of objects in the list changes
     */
    virtual void objectOrderChanged() = 0;

    //==========================================================================

    /**
     * @brief   Implements juce::ValueTree::Listener::valueTreeChildAdded()
     */
    void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree& tree) override
    {
        if (isChildTree(tree))
        {
            const int index = parent.indexOf(tree);
            (void) index;
            jassert(index >= 0);

            if (ObjectType* newObject = createNewObject(tree))
            {
                {
                    const ScopedLockType sl(arrayLock);

                    if (index == parent.getNumChildren() - 1)
                    {
                        objects.add(newObject);
                    }
                    else
                    {
                        objects.addSorted(*this, newObject);
                    }
                }

                newObjectAdded(newObject);
            }
            else
            {
                jassertfalse;
            }
        }
    }

    /**
     * @brief   Implements juce::ValueTree::Listener::valueTreeChildRemoved()
     */
    void valueTreeChildRemoved(juce::ValueTree& exParent,
                               juce::ValueTree& tree,
                               int) override
    {
        if (parent == exParent && isSuitableType(tree))
        {
            const int oldIndex = indexOf(tree);

            if (oldIndex >= 0)
            {
                ObjectType* o;

                {
                    const ScopedLockType sl(arrayLock);
                    o = objects.removeAndReturn(oldIndex);
                }

                objectRemoved(o);
                deleteObject(o);
            }
        }
    }

    /**
     * @brief   Implements
     *          juce::ValueTree::Listener::valueTreeChildOrderChanged()
     */
    void valueTreeChildOrderChanged(juce::ValueTree& tree, int, int) override
    {
        if (tree == parent)
        {
            {
                const ScopedLockType sl(arrayLock);
                sortArray();
            }

            objectOrderChanged();
        }
    }

    /**
     * @brief   Implements juce::ValueTree::Listener::valueTreePropertyChanged()
     */
    void valueTreePropertyChanged(juce::ValueTree&,
                                  const juce::Identifier&) override
    {
    }

    /**
     * @brief   Implements juce::ValueTree::Listener::valueTreeParentChanged()
     */
    void valueTreeParentChanged(juce::ValueTree&) override
    {
    }

    /**
     * @brief   Implements juce::ValueTree::valueTreeRedirected()
     */
    void valueTreeRedirected(juce::ValueTree&) override
    {
        jassertfalse; // may need to add handling if this is hit
    }

    //==========================================================================
    juce::Array<ObjectType*> objects;
    CriticalSectionType arrayLock;
    typedef typename CriticalSectionType::ScopedLockType ScopedLockType;

protected:

    //==========================================================================
    juce::ValueTree parent;

    //==========================================================================

    /**
     * @brief   Deletes all objects in the list
     */
    void deleteAllObjects()
    {
        const ScopedLockType sl(arrayLock);

        while (objects.size() > 0)
        {
            deleteObject(objects.removeAndReturn(objects.size() - 1));
        }
    }

    /**
     * @brief   Returns true if the given tree is a child of the list's parent
     *          tree
     */
    bool isChildTree(juce::ValueTree& v) const
    {
        return isSuitableType(v) && (v.getParent() == parent);
    }

    /**
     * @brief       Returns the index of an item in the list
     *
     * @param[in]   v   Value tree item
     */
    int indexOf(const juce::ValueTree& v) const noexcept
    {
        for (int i = 0; i < objects.size(); i++)
        {
            if (objects.getUnchecked(i)->state == v)
            {
                return i;
            }
        }

        return -1;
    }

    /**
     * @brief   Sorts the list of objects
     */
    void sortArray()
    {
        objects.sort(*this);
    }

public:

    //==========================================================================

    /**
     * @brief   Implements element comparator for list objects
     */
    int compareElements(ObjectType* first, ObjectType* second) const
    {
        int index1 = parent.indexOf(first->state);
        int index2 = parent.indexOf(second->state);
        return index1 - index2;
    }

    //==========================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeObjectList)
};

} // namespace config