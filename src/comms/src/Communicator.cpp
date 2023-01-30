/******************************************************************************
 * @file   Communicator.cpp
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Main file for VCU communication
 *****************************************************************************/

#include "Communicator.hpp"

#include "Candapter_MOCK.hpp"
#include <iostream>

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
const Comms::VariableVals* Communicator::get()
{
    Comms::VariableVals vals;

    uint8_t* buf = Candapter_MOCK::getMsg("data.fb");
    auto cmd = flatbuffers::GetRoot<Comms::Command>(buf);

    auto data = cmd->vals();

    Communicator::printVariables(data);

    return data;
}

/**
 * @brief       Runs the SET command
 *
 * @param[in]   vals    A struct containing the values for all the variables
 */
bool Communicator::set(Comms::VariableVals vals)
{

    std::tuple<uint8_t*, int> pair
        = Communicator::createCommand(Comms::CommandID_SET,
                                      &vals,
                                      nullptr,
                                      nullptr);

    Candapter_MOCK::sendMsg("data.fb", std::get<0>(pair), std::get<1>(pair));

    std::cout << "Data written" << std::endl;

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
    Comms::Version v = stringToVersion(version);
    const Comms::Version* v_ptr = &v;

    std::tuple<uint8_t*, int> pair
        = Communicator::createCommand(Comms::CommandID_SAVE,
                                      nullptr,
                                      &name,
                                      v_ptr);

    Candapter_MOCK::sendMsg("version.fb", std::get<0>(pair), std::get<1>(pair));

    /* FOR TESTING PURPOSES - DELETE LATER */
    uint8_t* data = Candapter_MOCK::getMsg("version.fb");
    auto received = flatbuffers::GetRoot<Comms::Command>(data);

    auto newName = flatbuffers::GetString(received->config_name());
    auto newVersion = versionToString(received->config_version());

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
Communicator::createCommand(Comms::CommandID cmdID,
                            const Comms::VariableVals* vals,
                            std::string* name,
                            const Comms::Version* version)
{

    // The flatbuffer builder object
    flatbuffers::FlatBufferBuilder builder(1024);

    if (cmdID == Comms::CommandID_SAVE)
    {
        assert(vals == nullptr);
        assert(name != nullptr);
        assert(version != nullptr);

        auto fbName = builder.CreateString(*name);
        // auto fbVersion = builder.CreateStruct(version);

        Comms::CommandBuilder commandBuilder(builder);

        commandBuilder.add_id(cmdID);
        commandBuilder.add_config_name(fbName);
        commandBuilder.add_config_version(version);

        auto command = commandBuilder.Finish();
        builder.Finish(command);
    }
    else if (cmdID == Comms::CommandID_SET)
    {
        assert(vals != nullptr);
        assert(name == nullptr);
        assert(version == nullptr);

        Comms::CommandBuilder commandBuilder(builder);

        commandBuilder.add_id(cmdID);
        commandBuilder.add_vals(vals);

        auto command = commandBuilder.Finish();
        builder.Finish(command);
    }
    else if (cmdID == Comms::CommandID_GET)
    {
        assert(vals == nullptr);
        assert(name == nullptr);
        assert(version == nullptr);

        Comms::CommandBuilder commandBuilder(builder);

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

/**
 * @brief       A helper function to convert a version struct into a string
 *
 * @param[in]   v    A pointer to a struct containing the version
 */
std::string Communicator::versionToString(const Comms::Version* v)
{
    return (std::to_string(v->a()) + "." + std::to_string(v->b()) + "."
            + std::to_string(v->c()));
}

/**
 * @brief       A helper function to convert a version string into a struct
 *
 * @param[in]   s    A version string in the form X.X.X
 *
 * @throw       std::overflow_error This is thrown if the values are larger than
 * 8 bits
 */
Comms::Version Communicator::stringToVersion(std::string s)
{
    Comms::Version version;
    uint8_t v[3]; // Parsed version number a.b.c
    try
    {
        int pos = 0;
        std::string token;
        std::string delimiter = ".";
        for (uint8_t i = 0; i < 3; i++)
        {
            pos = s.find(delimiter);
            int n = std::stoi(s.substr(0, pos));
            // if((n >= (int)'A' && n <= (int)'Z') || (n >= (int)'a' && n <=
            // (int)'z')){
            //   std::cout << "AGH" << std::endl;
            //   throw std::runtime_error("The version must be of the format
            //   X.X.X where the Xs are integers");
            // }
            if (n > 255)
                throw std::overflow_error("Value too large");
            s.erase(0, pos + delimiter.length());
            v[i] = n;
        }
        version = Comms::Version{v[0], v[1], v[2]};
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "The version number can only contain integers and must be "
                     "of the form X.X.X"
                  << std::endl;
        ;
    }
    catch (const std::overflow_error& e)
    {
        std::cerr << "The version number can only contain 8-bit integers"
                  << std::endl;
        ;
    }
    return version;
}

// Chunk the serialised data into small enough chunks to be sent in the payload
// of a CAN frame
bool chunkMsg()
{
    return true;
}

// Make a CAN frame
Frame makeFrame()
{
    Frame newFrame = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
    return newFrame;
}

// Convert a command into a series of CAN frames
std::vector<Frame> makeFrameSequence()
{
    return std::vector<Frame>{};
}

/**
 * @brief       A helper function to print the values stored in a variable
 * values struct
 *
 * @param[in]   data    A pointer to a struct containing the values
 */
void Communicator::printVariables(const Comms::VariableVals* data)
{
    std::cout << "Variable:                       Val" << std::endl;
    std::cout << "----------------------------------------------------"
              << std::endl;
    std::cout << "Torque Map:                    [";
    for (int i = 0; i < 2047; i++)
    {
        std::cout << std::to_string(data->torque_map_val()->data()[i]) << ", ";
    }
    std::cout << std::to_string(data->torque_map_val()->data()[2047]) << "]"
              << std::endl;
    if (data->inverter_mode_val() == Comms::InverterMode_TORQUE)
    {
        std::cout << "Inverter Mode:                 TORQUE" << std::endl;
    }
    else if (data->inverter_mode_val() == Comms::InverterMode_SPEED)
    {
        std::cout << "Inverter Mode:                 SPEED" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }
    std::cout << "Disable Torque Requests:       "
              << std::to_string(data->disable_torque_requests_val())
              << std::endl;
    std::cout << "APPS 1 ADC Min:                "
              << std::to_string(data->apps_1_adc_min_val()) << std::endl;
    std::cout << "APPS 1 ADC Max:                "
              << std::to_string(data->apps_1_adc_max_val()) << std::endl;
    std::cout << "APPS 2 ADC Min:                "
              << std::to_string(data->apps_2_adc_min_val()) << std::endl;
    std::cout << "APPS 2 ADC Max:                "
              << std::to_string(data->apps_2_adc_max_val()) << std::endl;
    std::cout << "BPS ADC Min:                   "
              << std::to_string(data->bps_adc_min_val()) << std::endl;
    std::cout << "BPS ADC Max:                   "
              << std::to_string(data->bps_adc_max_val()) << std::endl;
    std::cout << "BPS Fully Pressed Threshold:   "
              << std::to_string(data->bps_fully_pressed_threshold_val())
              << std::endl;
    std::cout << "Enable Lapsim Testbench:       "
              << std::to_string(data->enable_lapsim_testbench_val())
              << std::endl;
    std::cout << "Lapsim Testbench Laps:         "
              << std::to_string(data->lapsim_testbench_laps_val()) << std::endl;
}