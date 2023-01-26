/******************************************************************************
 * @file   Communicator.cpp
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Main file for VCU communication
 *****************************************************************************/

#include "Communicator.h"

#include "schema_generated.h"
//#include "candapter.h"

Communicator* Communicator::getInstance()
{
    if (Communicator::_instance == nullptr)
    {
        Communicator::_instance = new Communicator();
    }
    return Communicator::_instance;
}

// Constructor for the Communicator object
// Opens the serial port for the candapter
Communicator::Communicator(/* args */)
{
}

// Descructor for the Communicator object
// Closes the serial port
Communicator::~Communicator()
{
}
