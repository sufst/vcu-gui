/******************************************************************************
 * @file   Communicator.cpp
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Main file for VCU communication
 *****************************************************************************/

#include "Communicator.hpp"

#include "schema_generated.h"
//#include "candapter.h"

#include <iostream>

Communicator* Communicator::_instance;
flatbuffers::FlatBufferBuilder* Communicator::_fbBuilder;

Communicator* Communicator::getInstance()
{
    if (Communicator::_instance == nullptr)
        Communicator::_instance = new Communicator();
    return Communicator::_instance;
}

flatbuffers::FlatBufferBuilder* Communicator::getBuilder()
{
    if (Communicator::_fbBuilder == nullptr)
    {
        flatbuffers::FlatBufferBuilder builder(1024);
        Communicator::_fbBuilder = &builder;
    }
    return Communicator::_fbBuilder;
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

bool Communicator::setConfigDetails(std::string name, std::string version)
{
    int v[3]; // Parsed version number a.b.c
    try
    {
        int pos = 0;
        std::string token;
        std::string delimiter = ".";
        for (uint8_t i = 0; i < 3; i++)
        {
            pos = version.find(delimiter);
            v[i] = std::stoi(version.substr(0, pos));
            version.erase(0, pos + delimiter.length());
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout << name << " - " << v[0] << v[1] << v[2] << std::endl;
}

std::string Communicator::get()
{
    return ":(";
}

bool Communicator::save()
{
    return true;
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