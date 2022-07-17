/******************************************************************************
 * @file   VCUConfiguration.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Data model for VCU configuration
 *****************************************************************************/

#include "VCUConfiguration.h"

#include "Interpolator.h"

/**
 * @brief Default constructor
 */
VCUConfiguration::VCUConfiguration() : tree(createEmptyConfiguration())
{
}

/**
 * @brief Create an empty configuration
 */
juce::ValueTree VCUConfiguration::createEmptyConfiguration()
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
juce::ValueTree VCUConfiguration::getTorqueMap() const
{
    return tree.getChildWithName(TorqueMap);
}

/**
 * @brief       Creates a new value tree representing a torque map point
 *
 * @param[in]   input   Input value
 * @param[in]   output  Output value (input -> torque map -> output)
 */
juce::ValueTree VCUConfiguration::createTorqueMapPoint(int input, int output)
{
    juce::ValueTree point(TorqueMapPoint);

    point.setProperty(TorqueMapInputValue, input, nullptr);
    point.setProperty(TorqueMapOutputValue, output, nullptr);

    return point;
}

/**
 * @brief   Exports the configuration to an XML document
 */
std::unique_ptr<juce::XmlDocument> VCUConfiguration::exportXml() const
{
    return std::make_unique<juce::XmlDocument>(tree.toXmlString());
}

/**
 * @brief       Load a configuration from an XML document
 *
 * @param[in]   xml     XML document
 */
void VCUConfiguration::loadFromXml(juce::XmlDocument& xml)
{
    tree = juce::ValueTree::fromXml(xml.getDocumentElement()->toString());
    DBG(tree.toXmlString());
    sendSynchronousChangeMessage();
}

/*
 * property identifiers
 */
const juce::Identifier VCUConfiguration::Root = "VCUConfiguration";
const juce::Identifier VCUConfiguration::TorqueMap = "TorqueMap";
const juce::Identifier VCUConfiguration::TorqueMapPoint = "TorqueMapPoint";
const juce::Identifier VCUConfiguration::TorqueMapInputValue = "TorqueMapInput";
const juce::Identifier VCUConfiguration::TorqueMapOutputValue = "TorqueMapOutput";
const juce::Identifier VCUConfiguration::ProfileName = "ProfileName";
const juce::Identifier VCUConfiguration::InterpolationMethod = "InterpolationMethod";