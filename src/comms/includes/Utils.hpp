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

namespace utils
{
// private:
//   Utils();
//   ~Utils();

// public:
static void chunkMsg();
static void makeFrame();
static void makeFrameSequence();

static CommsSchema::Version stringToVersion(std::string);
static std::string versionToString(const CommsSchema::Version*);
static void printVariables(const CommsSchema::VariableVals*);
}; // namespace utils

struct Frame
{
    uint8_t id;
    uint16_t frameNumber;
    uint16_t totalFrames;
    char payload[8];
};
} // namespace comms