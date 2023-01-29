/******************************************************************************
 * @file   Communicator.cpp
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Main file for VCU communication
 *****************************************************************************/

#include "Communicator.hpp"

#include "Candapter_MOCK.hpp"
#include <iostream>

Communicator* Communicator::_instance;

Communicator* Communicator::getInstance()
{
    if (Communicator::_instance == nullptr)
        Communicator::_instance = new Communicator();
    return Communicator::_instance;
}

// Constructor for the Communicator object
// Opens the serial port for the candapter
Communicator::Communicator()
{
}

// Descructor for the Communicator object
// Closes the serial port
Communicator::~Communicator()
{
}

bool Communicator::set()
{
    return true;
}

bool Communicator::save(std::string name, std::string version)
{
    Comms::Version v = stringToVersion(version);
    const Comms::Version* v_ptr = &v;

    // Oh no, now time for serialisation :(
    flatbuffers::FlatBufferBuilder builder(1024);
    auto config_name = builder.CreateString(name);
    auto config_version = builder.CreateStruct(v);

    Comms::CommandBuilder commandBuilder(builder);
    commandBuilder.add_id(Comms::CommandID_SAVE);
    commandBuilder.add_config_name(config_name);
    commandBuilder.add_config_version(v_ptr);

    auto command = commandBuilder.Finish();
    builder.Finish(command);

    uint8_t* buf = builder.GetBufferPointer();
    int size = builder.GetSize();

    Candapter_MOCK::sendMsg("schema.fb", buf, size);

    return false;
}

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

std::string Communicator::get()
{
    return ":(";
}

bool chunkMsg()
{
    return true;
}

Frame makeFrame()
{
    Frame newFrame = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
    return newFrame;
}

std::vector<Frame> makeFrameSequence()
{
    return std::vector<Frame>{};
}