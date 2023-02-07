/******************************************************************************
 * @file   Utils.cpp
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  A file with helper methods for VCU communication
 *****************************************************************************/

#include "Utils.hpp"

#include "Candapter_MOCK.hpp"
#include <iostream>
#include <math.h>

// Divide the buffer into blocks of 6 bytes
std::vector<uint8_t[CHUNK_SIZE]> comms::utils::chunkMsg(uint8_t* buf,
                                                        int length)
{
    std::vector<uint8_t[CHUNK_SIZE]> chunks;
    uint8_t single[CHUNK_SIZE]; // Zero initialised

    for (uint16_t i = 0; i < (length / CHUNK_SIZE); i++)
    {

        if ((i + CHUNK_SIZE) <= length)
        {
            for (uint8_t j = 0; j < CHUNK_SIZE; j++)
            {
                single[j] = buf[i];
            }
        }
        else
        {
            throw std::runtime_error("aohdiosdipsadios");
        }

        chunks.push_back(single);
    }

    if (chunks.size() != std::ceil(length / CHUNK_SIZE))
    {
        throw std::runtime_error("Incorrect number of chunks");
    }

    return chunks;
}

comms::Frame comms::utils::makeFrame()
{
}

std::vector<comms::Frame> comms::utils::makeFrameSequence()
{
}

/**
 * @brief       A helper function to convert a version struct into a string
 *
 * @param[in]   v    A pointer to a struct containing the version
 */
std::string comms::utils::versionToString(const CommsSchema::Version* v)
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
CommsSchema::Version comms::utils::stringToVersion(std::string s)
{
    CommsSchema::Version version;
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
        version = CommsSchema::Version{v[0], v[1], v[2]};
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

/**
 * @brief       A helper function to print the values stored in a variable
 * values struct
 *
 * @param[in]   data    A pointer to a struct containing the values
 */
void comms::utils::printVariables(const CommsSchema::VariableVals* data)
{
    std::cout << "Variable:                       Val" << std::endl;
    std::cout << "----------------------------------------------------"
              << std::endl;
    std::cout << "Config name: ";
    auto name = data->config_name()->data();
    std::string name_str;
    for (uint8_t i = 0; i < 64; i++)
    {
        unsigned char c = (unsigned char) name[i];
        if (c == '\0')
            break;
        name_str += c;
    }

    std::cout << "Torque Map:                    [";
    auto tm = data->torque_map_val()->data();
    for (int i = 0; i < 2047; i++)
    {
        std::cout << std::to_string(tm[i]) << ", ";
    }
    std::cout << std::to_string(tm[2047]) << "]" << std::endl;
    auto im = data->inverter_mode_val();
    if (im == CommsSchema::InverterMode_TORQUE)
    {
        std::cout << "Inverter Mode:                 TORQUE" << std::endl;
    }
    else if (im == CommsSchema::InverterMode_SPEED)
    {
        std::cout << "Inverter Mode:                 SPEED" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }
    std::cout << "Disable Torque Requests:       ";
    auto dtr = data->disable_torque_requests_val();
    if (dtr)
    {
        std::cout << "TRUE" << std::endl;
    }
    else
    {
        std::cout << "FALSE" << std::endl;
    }
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
    std::cout << "Enable Lapsim Testbench:       ";
    auto elt = data->enable_lapsim_testbench_val();
    if (elt)
    {
        std::cout << "TRUE" << std::endl;
    }
    else
    {
        std::cout << "FALSE" << std::endl;
    }
    std::cout << "Lapsim Testbench Laps:         "
              << std::to_string(data->lapsim_testbench_laps_val()) << std::endl;
}
