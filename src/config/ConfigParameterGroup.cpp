/******************************************************************************
 * @file    ConfigParameterGroup.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "ConfigParameterGroup.h"

//==============================================================================

/**
 * @brief       Default constructor
 *
 * @param[in]   groupID     Identifier for group
 * @param[in]   groupName   Name of group
 */
ConfigParameterGroup::ConfigParameterGroup(const juce::String& groupID,
                                           const juce::String& groupName)
    : identifier(groupID), name(groupName)
{
}

//==============================================================================

/**
 * @brief       Creates a new node from a parameter
 *
 * @param[in]   parameter       Parameter associated with node
 * @param[in]   parentGroup     Parent group of node
 */
ConfigParameterGroup::ConfigParameterNode::ConfigParameterNode(
    std::unique_ptr<ConfigParameter> parameter,
    ConfigParameterGroup* parentGroup)
    : param(std::move(parameter)), parent(parentGroup)
{
}

/**
 * @brief       Creates a new node from a parameter group
 *
 * @param[in]   grp             Group
 * @param[in]   parentGroup     Parent group of group
 */
ConfigParameterGroup::ConfigParameterNode::ConfigParameterNode(
    std::unique_ptr<ConfigParameterGroup> grp,
    ConfigParameterGroup* parentGroup)
    : group(std::move(grp)), parent(parentGroup)
{
}

/**
 * @brief   Returns the parent group or nullptr if top level group
 */
ConfigParameterGroup*
ConfigParameterGroup::ConfigParameterNode::getParent() const
{
    return parent;
}

/**
 * @brief   Returns a pointer to the parameter if this node is a group,
 *          and nullptr otherwise
 */
ConfigParameterGroup*
ConfigParameterGroup::ConfigParameterNode::getGroup() const
{
    return group.get();
}

/**
 * @brief   Returns a pointer to the parameter if this node is a parameter,
 *          and nullptr otherwise
 */
ConfigParameter* ConfigParameterGroup::ConfigParameterNode::getParameter() const
{
    return param.get();
}

//==============================================================================

/**
 * @brief       Adds a subgroup to this group
 *
 * @param[in]   group   Subgroup to add
 */
void ConfigParameterGroup::append(std::unique_ptr<ConfigParameterGroup> group)
{
    children.add(new ConfigParameterNode(std::move(group), this));
}

/**
 * @brief       Adds a parameter to this group
 *
 * @param[in]   parameter   parameter to add
 */
void ConfigParameterGroup::append(std::unique_ptr<ConfigParameter> parameter)
{
    children.add(new ConfigParameterNode(std::move(parameter), this));
}