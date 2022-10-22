/******************************************************************************
 * @file   VCUConfiguration.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Data model for VCU configuration
 *****************************************************************************/

#include "ConfigurationValueTree.h"

#include "../utility/Interpolator.h"

using utility::InterpolatorFactory;
using utility::LinearInterpolator;

//==============================================================================

/**
 * @brief Default constructor
 */
ConfigurationValueTree::ConfigurationValueTree()
    : tree(createEmptyConfiguration())
{
}

/**
 * @brief Create an empty configuration
 */
juce::ValueTree ConfigurationValueTree::createEmptyConfiguration()
{
    juce::ValueTree rootTree(Root);
    juce::ValueTree torqueMapTree(Children::TorqueMap);

    rootTree.addChild(torqueMapTree, 0, nullptr);
    rootTree.setProperty(Properties::ProfileName, "New Profile", nullptr);

    torqueMapTree.setProperty(
        Properties::InterpolationMethod,
        utility::SplineInterpolator<int>::identifier.toString(),
        nullptr);
    torqueMapTree.addChild(createTorqueMapPoint(0, 0), 0, nullptr);
    torqueMapTree.addChild(createTorqueMapPoint(1023, 32767), 1, nullptr);

    return rootTree;
}

//==============================================================================

/**
 * @brief   Adds a listener to the root juce::ValueTree
 *
 * @note    This should be used to register juce::ValueTree::Listener objects as
 *          it adds a listener to the root value tree owned by this object. If
 *          getRoot().addListener() or similar is used, when a new profile is
 *          loaded the valueTreeRedirected() callback will not be called!
 */
void ConfigurationValueTree::addListener(juce::ValueTree::Listener* newListener)
{
    tree.addListener(newListener);
}

//==============================================================================

/**
 * @brief Returns the root tree
 */
juce::ValueTree ConfigurationValueTree::getRoot() const
{
    return tree;
}

/**
 * @brief       Returns the first child tree with the specified name, if it
 * exists
 *
 * @param[in]   identifier  Identifier name of the child
 */
juce::ValueTree ConfigurationValueTree::getChildWithName(
    const juce::Identifier& identifier) const
{
    return tree.getChildWithName(identifier);
}

//==============================================================================

/**
 * @brief       Creates a new value tree representing a torque map point
 *
 * @param[in]   input   Input value
 * @param[in]   output  Output value (input -> torque map -> output)
 */
juce::ValueTree ConfigurationValueTree::createTorqueMapPoint(int input,
                                                             int output)
{
    juce::ValueTree point(Children::TorqueMapPoint);

    point.setProperty(Properties::InputValue, input, nullptr);
    point.setProperty(Properties::OutputValue, output, nullptr);

    return point;
}

//==============================================================================

/**
 * @brief   Exports the configuration to an XML document
 */
std::unique_ptr<juce::XmlDocument> ConfigurationValueTree::exportXml() const
{
    return std::make_unique<juce::XmlDocument>(tree.toXmlString());
}

/**
 * @brief       Load a configuration from a file
 *
 * @note        This will cause juce::ValueTree::Listener objects registered
 *              with addListener() to receive the valueTreeRedirected()
 *              callback which should handle loading of a new profile
 *
 * @param[in]   xml     XML document
 */
void ConfigurationValueTree::loadFromFile(const juce::File& file)
{
    juce::XmlDocument xml(file);
    tree = juce::ValueTree::fromXml(xml.getDocumentElement()->toString());
}