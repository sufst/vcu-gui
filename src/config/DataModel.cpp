/******************************************************************************
 * @file    DataModel.cpp
 * @author  Tim Brewis (@t-bre)
 *****************************************************************************/

#include "DataModel.h"

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
    // torque map
    juce::ValueTree torqueMapTree(IDs::TORQUE_MAP);
    TorqueMap torqueMap(torqueMapTree);
    tree.addChild(torqueMapTree, -1, nullptr);

    std::initializer_list<std::pair<int, int>> defaultPoints{
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
