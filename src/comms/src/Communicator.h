/******************************************************************************
 * @file   Communicator.h
 * @author Toby Godfrey (@tg, tmag1g21@soton.ac.uk)
 * @brief  Header file for comms
 *****************************************************************************/

// Singleton pattern to ensure only one communicator is created
class Communicator
{
private:

    static Communicator* _instance;

    Communicator();
    ~Communicator();

public:

    static Communicator* getInstance();
};