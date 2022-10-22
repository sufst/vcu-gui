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
    // TODO: create model!
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
