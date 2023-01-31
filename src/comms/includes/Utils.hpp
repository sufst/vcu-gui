/******************************************************************************
 * @file   Utils.hpp
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Header file for comms utils
 *****************************************************************************/

#pragma once

#include "flatbuffers/flatbuffer_builder.h"
#include "schema_generated.h"
#include <string>
#include <vector>

namespace comms
{

namespace utils
{
void chunkMsg();
void makeFrame();
void makeFrameSequence();

CommsSchema::Version stringToVersion(std::string);
std::string versionToString(const CommsSchema::Version*);
void printVariables(const CommsSchema::VariableVals*);
}; // namespace utils

struct Frame
{
    uint8_t id;
    uint16_t frameNumber;
    uint16_t totalFrames;
    char payload[8];
};
} // namespace comms