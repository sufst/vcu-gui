/******************************************************************************
 * @file   Communicator.h
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Header file for comms
 *****************************************************************************/

#pragma once

#include <string>
#include <vector>

// Singleton pattern to ensure only one communicator is created
class Communicator
{
private:

    static Communicator* _instance;

    Communicator();
    ~Communicator();
    bool chunkMsg();
    struct Frame makeFrame();
    std::vector<struct Frame> makeFrameSequence();

public:

    static Communicator* getInstance();

    bool set();
    std::string get();
    bool save();
};

struct Frame;