/******************************************************************************
 * @file   Utils.hpp
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Header file for comms utils
 *****************************************************************************/

#pragma once

#include "schema_generated.h"
#include <string>
#include <vector>

#define CHUNK_SIZE 6
#define CAN_ID     10

namespace comms
{
struct Frame
{
    uint8_t id;
    uint8_t payload[8];
};

namespace utils
{
std::vector<uint8_t*> chunkMsg(uint8_t*, int);
::comms::Frame makeFrame(uint8_t*, uint16_t);
std::vector<::comms::Frame> makeFrameSequence(std::vector<uint8_t*>);

CommsSchema::Version stringToVersion(std::string);
std::string versionToString(const CommsSchema::Version*);
void printVariables(const CommsSchema::VariableVals*);
}; // namespace utils

} // namespace comms