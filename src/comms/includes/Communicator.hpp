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

namespace comms
{

// Singleton pattern to ensure only one communicator is created
class Communicator
{
private:

    static Communicator* _instance;

    Communicator();
    ~Communicator();
    std::tuple<uint8_t*, int> createCommand(CommsSchema::CommandID,
                                            const CommsSchema::VariableVals*,
                                            std::string*,
                                            const CommsSchema::Version*);

public:

    static Communicator* getInstance();

    bool set(CommsSchema::VariableVals);
    const CommsSchema::VariableVals* get();
    bool save(std::string, std::string);
};
} // namespace comms