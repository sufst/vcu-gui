/******************************************************************************
 * @file    DataModel.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "DataModel.h"

#include "Metadata.h"
#include "TorqueMap.h"

namespace config
{

//==============================================================================

/**
 * @brief   Default constructor
 */
DataModel::DataModel() : tree(IDs::CONFIGURATION)
{
    createDefaultModel();
}

/**
 * @brief   Creates the default data model
 */
void DataModel::createDefaultModel()
{
    // metadata
    juce::ValueTree metadataTree(IDs::METADATA);
    Metadata metadata(metadataTree);
    tree.addChild(metadataTree, -1, nullptr);

    // torque map
    juce::ValueTree torqueMapTree(IDs::TORQUE_MAP);
    TorqueMap torqueMap(torqueMapTree);
    tree.addChild(torqueMapTree, -1, nullptr);
    tree.setProperty(
        IDs::InterpolationMethod,
        utility::SplineInterpolator<TorqueMapPoint::ValueType>::identifier
            .toString(),
        nullptr);

    std::initializer_list<
        std::pair<TorqueMapPoint::ValueType, TorqueMapPoint::ValueType>>
        defaultPoints{
            {TorqueMapPoint::MinInput, TorqueMapPoint::MinOutput},
            {TorqueMapPoint::MaxInput, TorqueMapPoint::MaxOutput},
        };

    for (const auto& [input, output] : defaultPoints)
    {
        juce::ValueTree pointTree = juce::ValueTree(IDs::TorqueMapPoint);
        torqueMapTree.addChild(pointTree, -1, nullptr);

        TorqueMapPoint point(pointTree);
        point.input = input;
        point.output = output;
    }

    DBG("Created default configuration:\n");
    DBG(tree.createXml()->toString());
}

/**
 * @brief       Saves the model to a file
 *
 * @param[in]   file    File to save to
 *
 * @return      true    Success
 * @return      false   Error
 */
bool DataModel::saveToFile(const juce::File& file)
{
    const juce::TemporaryFile tmpFile(file);

    {
        juce::FileOutputStream os(tmpFile.getFile());

        if (!os.getStatus().wasOk())
        {
            return false;
        }

        if (auto xml = std::unique_ptr<juce::XmlElement>(tree.createXml()))
        {
            xml->writeTo(os);
        }
    }

    if (tmpFile.getFile().existsAsFile())
    {
        return tmpFile.overwriteTargetFileWithTemporary();
    }

    return false;
}

/**
 * @brief   Attempts to load the model from a file
 *
 * @note    Taken from the Projucer code
 */
void DataModel::loadFromFile(const juce::File& file)
{
    if (auto xml
        = std::unique_ptr<juce::XmlElement>(juce::XmlDocument::parse(file)))
    {
        const auto newModel = juce::ValueTree::fromXml(*xml);

        // manually copy across the new properties to the existing tree so
        // that the GUI is updated without reloading it
        //
        // TODO: this was taken from the Projucer code
        //       does this work for any depth of tree?
        tree.copyPropertiesFrom(newModel, nullptr);

        for (int i = tree.getNumChildren(); --i >= 0;) // what the
        {
            ValueTree c(tree.getChild(i));

            if (c.getType() == IDs::TORQUE_MAP)
            {
                // TODO: this is a hack for importing the torque map since
                //       the only array. surely there is a better way?
                TorqueMap actualMap(c);
                TorqueMap newMap(newModel.getChildWithName(IDs::TORQUE_MAP));

                for (const auto& point : actualMap.getPoints())
                {
                    actualMap.removePoint(*point);
                }

                for (const auto* point : newMap.getPoints())
                {
                    actualMap.addPoint(point->input.get(), point->output.get());
                }
                actualMap.state.sendPropertyChangeMessage(IDs::TORQUE_MAP);

                // c.copyPropertiesAndChildrenFrom(
                //     newModel.getChildWithName(IDs::TORQUE_MAP),
                //     nullptr);
            }
            else
            {
                const ValueTree newValue(
                    newModel.getChildWithName(c.getType()));

                if (newValue.isValid())
                {
                    c.copyPropertiesFrom(newValue, nullptr);
                }
            }
        }

        // tree.copyPropertiesAndChildrenFrom(newModel, nullptr);
        DBG("Imported configuration:\n");
        DBG(tree.toXmlString());
    }
}

//==============================================================================

#if UNIT_TEST

/**
 * @brief   Unit test for data model
 */
class DataModelTest : public juce::UnitTest
{
public:

    DataModelTest();
    ~DataModelTest() = default;

    void runTest();
};

DataModelTest::DataModelTest() : juce::UnitTest("config::DataModel")
{
}

void DataModelTest::runTest()
{
    beginTest("Model creation");
    DataModel model;
}

/**
 * @brief   Test instance
 */
static DataModelTest testInstance;

#endif

} // namespace config
