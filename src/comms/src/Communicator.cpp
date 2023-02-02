/******************************************************************************
 * @file   Communicator.cpp
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Main file for VCU communication
 *****************************************************************************/

#include "Communicator.hpp"

#include "Candapter_MOCK.hpp"
#include "Utils.hpp"
#include <iostream>

using namespace comms;

Communicator* Communicator::_instance;

/**
 * @brief       Returns a pointer to the instance of the class
 *
 * @note        This is needed as we use the singleton pattern so there can only
 * be one instance of the class
 */
Communicator* Communicator::getInstance()
{
    if (Communicator::_instance == nullptr)
        Communicator::_instance = new Communicator();
    return Communicator::_instance;
}

/**
 * @brief       Constructor for the Communicator class
 *
 * @note        This opens the serial port for the CANdapter
 */
Communicator::Communicator()
{
}

/**
 * @brief       Destructor for the Communicator class
 *
 * @note        This closes the serial port for the CANdapter
 */
Communicator::~Communicator()
{
}

/**
 * @brief       Runs the GET command
 */
const CommsSchema::VariableVals* Communicator::get()
{
    CommsSchema::VariableVals vals;

    uint8_t* buf = Candapter_MOCK::getMsg("data.fb");
    auto cmd = flatbuffers::GetRoot<CommsSchema::Command>(buf);

    auto data = cmd->vals();

    utils::printVariables(data);

    return data;
}

/**
 * @brief       Runs the SET command
 *
 * @param[in]   vals    A struct containing the values for all the variables
 */
bool Communicator::set(CommsSchema::VariableVals vals)
{

    std::tuple<uint8_t*, int> pair
        = Communicator::createCommand(CommsSchema::CommandID_SET,
                                      &vals,
                                      nullptr,
                                      nullptr);

    Candapter_MOCK::sendMsg("data.fb", std::get<0>(pair), std::get<1>(pair));

    std::cout << std::to_string(std::get<1>(pair)) << " bytes written written"
              << std::endl;

    return true;
}

/**
 * @brief       Runs the SAVE command
 *
 * @note        This command moves the current config into EEPROM
 *
 * @param[in]   name    The name for the current configuration
 * @param[in]   version A struct containing the new version number
 */
bool Communicator::save(std::string name, std::string version)
{
    CommsSchema::Version v = utils::stringToVersion(version);
    const CommsSchema::Version* v_ptr = &v;

    std::tuple<uint8_t*, int> pair
        = Communicator::createCommand(CommsSchema::CommandID_SAVE,
                                      nullptr,
                                      &name,
                                      v_ptr);

    Candapter_MOCK::sendMsg("version.fb", std::get<0>(pair), std::get<1>(pair));

    /* FOR TESTING PURPOSES - DELETE LATER */
    uint8_t* data = Candapter_MOCK::getMsg("version.fb");
    auto received = flatbuffers::GetRoot<CommsSchema::Command>(data);

    auto newName = flatbuffers::GetString(received->config_name());
    auto newVersion = utils::versionToString(received->config_version());

    std::cout << newName << " - version " << newVersion << std::endl;

    return false;
}

/**
 * @brief       A command builder
 *
 * @note        This is a private function to build command flatbuffers and
 * check the relevant arguments are given
 *
 * @param[in]   cmdID   The enum value for the command type to build
 * @param[in]   vals    A pointer to a struct containing the values for all the
 * variables
 * @param[in]   name    A pointer to the name for the new saved configuration
 * (relevant for the SAVE command)
 * @param[in]   version A pointer to a struct containing the new version number
 * (relevant for the SAVE command)
 */
std::tuple<uint8_t*, int>
Communicator::createCommand(CommsSchema::CommandID cmdID,
                            const CommsSchema::VariableVals* vals,
                            std::string* name,
                            const CommsSchema::Version* version)
{

    // The flatbuffer builder object
    flatbuffers::FlatBufferBuilder builder(1024);

    if (cmdID == CommsSchema::CommandID_SAVE)
    {
        assert(vals == nullptr);
        assert(name != nullptr);
        assert(version != nullptr);

        auto fbName = builder.CreateString(*name);
        // auto fbVersion = builder.CreateStruct(version);

        CommsSchema::CommandBuilder commandBuilder(builder);

        commandBuilder.add_id(cmdID);
        commandBuilder.add_config_name(fbName);
        commandBuilder.add_config_version(version);

        auto command = commandBuilder.Finish();
        builder.Finish(command);
    }
    else if (cmdID == CommsSchema::CommandID_SET)
    {
        assert(vals != nullptr);
        assert(name == nullptr);
        assert(version == nullptr);

        CommsSchema::CommandBuilder commandBuilder(builder);

        commandBuilder.add_id(cmdID);
        commandBuilder.add_vals(vals);

        auto command = commandBuilder.Finish();
        builder.Finish(command);
    }
    else if (cmdID == CommsSchema::CommandID_GET)
    {
        assert(vals == nullptr);
        assert(name == nullptr);
        assert(version == nullptr);

        CommsSchema::CommandBuilder commandBuilder(builder);

        commandBuilder.add_id(cmdID);

        auto command = commandBuilder.Finish();
        builder.Finish(command);
    }
    else
    {
        throw std::runtime_error(
            "This function can only be used for SAVE, GET and SET commands!");
    }

    uint8_t* buf = builder.GetBufferPointer();
    int size = builder.GetSize();

    return std::tuple<uint8_t*, int>(buf, size);
}
