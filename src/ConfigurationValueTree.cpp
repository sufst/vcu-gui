/******************************************************************************
 * @file   VCUConfiguration.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Data model for VCU configuration
 *****************************************************************************/

#include "ConfigurationValueTree.h"

#include "Interpolator.h"

/**
 * @brief Default constructor
 */
ConfigurationValueTree::ConfigurationValueTree() : tree(createEmptyConfiguration())
{
}

/**
 * @brief Create an empty configuration
 */
juce::ValueTree ConfigurationValueTree::createEmptyConfiguration()
{
    juce::ValueTree root(Root);
    juce::ValueTree torqueMapTree(TorqueMap);

    root.addChild(torqueMapTree, 0, nullptr);
    root.setProperty(ProfileName, "New Profile", nullptr);

    torqueMapTree.setProperty(InterpolationMethod, utility::SplineInterpolator<int>::identifier.toString(), nullptr);
    torqueMapTree.addChild(createTorqueMapPoint(0, 0), 0, nullptr);
    torqueMapTree.addChild(createTorqueMapPoint(1023, 32767), 1, nullptr);

    return root;
}

/**
 * @brief Returns a reference to the juce::ValueTree containing the torque map
 */
juce::ValueTree ConfigurationValueTree::getTorqueMap() const
{
    return tree.getChildWithName(TorqueMap);
}

/**
 * @brief       Creates a new value tree representing a torque map point
 *
 * @param[in]   input   Input value
 * @param[in]   output  Output value (input -> torque map -> output)
 */
juce::ValueTree ConfigurationValueTree::createTorqueMapPoint(int input, int output)
{
    juce::ValueTree point(TorqueMapPoint);

    point.setProperty(TorqueMapInputValue, input, nullptr);
    point.setProperty(TorqueMapOutputValue, output, nullptr);

    return point;
}

/**
 * @brief   Exports the configuration to an XML document
 */
std::unique_ptr<juce::XmlDocument> ConfigurationValueTree::exportXml() const
{
    return std::make_unique<juce::XmlDocument>(tree.toXmlString());
}

/**
 * @brief       Load a configuration from an XML document
 *
 * @param[in]   xml     XML document
 */
void ConfigurationValueTree::loadFromXml(juce::XmlDocument& xml)
{
    tree.copyPropertiesAndChildrenFrom(juce::ValueTree::fromXml(xml.getDocumentElement()->toString()), nullptr);
    DBG(tree.toXmlString());
    sendSynchronousChangeMessage();
}

/*
 * property identifiers
 */
const juce::Identifier ConfigurationValueTree::Root = "VCUConfiguration";
const juce::Identifier ConfigurationValueTree::TorqueMap = "TorqueMap";
const juce::Identifier ConfigurationValueTree::TorqueMapPoint = "TorqueMapPoint";
const juce::Identifier ConfigurationValueTree::TorqueMapInputValue = "TorqueMapInput";
const juce::Identifier ConfigurationValueTree::TorqueMapOutputValue = "TorqueMapOutput";
const juce::Identifier ConfigurationValueTree::ProfileName = "ProfileName";
const juce::Identifier ConfigurationValueTree::InterpolationMethod = "InterpolationMethod";