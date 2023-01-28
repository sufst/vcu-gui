/******************************************************************************
 * @file   Communicator.h
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Header file for comms
 *****************************************************************************/

#pragma once

#include "flatbuffers/flatbuffer_builder.h"
#include "schema_generated.h"
#include <string>
#include <vector>

// Singleton pattern to ensure only one communicator is created
class Communicator
{
private:

    static Communicator* _instance;
    static flatbuffers::FlatBufferBuilder* _fbBuilder;

    Communicator();
    ~Communicator();
    bool chunkMsg();
    struct Frame makeFrame();
    std::vector<struct Frame> makeFrameSequence();
    Comms::Version stringToVersion(std::string);

public:

    static Communicator* getInstance();
    static flatbuffers::FlatBufferBuilder* getBuilder();

    bool set();
    std::string get();
    bool save(std::string name, std::string version);
};

struct Frame
{
    uint8_t id;
    uint16_t frameNumber;
    uint16_t totalFrames;
    char payload[8];
};
