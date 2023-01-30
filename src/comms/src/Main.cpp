#include "Communicator.hpp"
#include <iostream>

int main(int argc, char const* argv[])
{
    Communicator* c = Communicator::getInstance();

    std::string cmd;
    while (true)
    {
        std::cout << "Command: ";
        std::cin >> cmd;
        if (cmd == "exit" || cmd == ":q")
        {
            break;
        }
        else if (cmd == "save")
        {
            std::string n;
            std::cout << "Config name: ";
            std::cin >> n;
            std::string v;
            std::cout << "Config version: ";
            std::cin >> v;
            std::cout << std::endl;
            c->save(n, v);
        }
        else if (cmd == "set")
        {
            std::cout << "Torque Map ([uint8:2048] (Enter a single value and "
                         "it will be repeated)): ";
            uint8_t val;
            std::cin >> val;
            uint8_t tm[2048];
            for (int i = 0; i < 2048; i++)
            {
                tm[i] = val;
            }
            std::cout << "Inverter Mode (1 = Torque, ¬1 = Speed): ";
            std::cin >> val;
            Comms::InverterMode im;
            switch (val)
            {
            case 1:
                im = Comms::InverterMode_TORQUE;
                break;

            default:
                im = Comms::InverterMode_SPEED;
                break;
            }
            std::cout << "Disable Torque Requests (Bool): ";
            bool dtr;
            std::cin >> dtr;
            std::cout << "APPS 1 ADC Min (uint16): ";
            uint16_t a1min;
            std::cin >> a1min;
            std::cout << "APPS 1 ADC Max (uint16): ";
            uint16_t a1max;
            std::cin >> a1max;
            std::cout << "APPS 2 ADC Min (uint16): ";
            uint16_t a2min;
            std::cin >> a2min;
            std::cout << "APPS 2 ADC Max (uint16): ";
            uint16_t a2max;
            std::cin >> a2max;
            std::cout << "BPS ADC Min (uint16): ";
            uint16_t bmin;
            std::cin >> bmin;
            std::cout << "BPS ADC Max (uint16): ";
            uint16_t bmax;
            std::cin >> bmax;
            std::cout << "BPS Fully Pressed Threshold (uint32): ";
            uint32_t bpst;
            std::cin >> bpst;
            std::cout << "Enable Lapsim Testbench (Bool): ";
            bool elt;
            std::cin >> elt;
            std::cout << "Lapsim Testbench Laps (uint8): ";
            uint8_t ltl;
            std::cin >> ltl;

            flatbuffers::FlatBufferBuilder builder(1024);

            Comms::VariableVals vals(tm,
                                     im,
                                     dtr,
                                     a1min,
                                     a1max,
                                     a2min,
                                     a2max,
                                     bmin,
                                     bmax,
                                     bpst,
                                     elt,
                                     ltl);

            c->set(vals);
        }
        else if (cmd == "get")
        {

            c->get();
        }
        else
        {
            std::cout << "Not a valid command" << std::endl;
        }
    }

    return 0;
}
