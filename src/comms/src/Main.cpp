#include "Communicator.hpp"
#include <iostream>

int main(int argc, char const* argv[])
{
    comms::Communicator* c = comms::Communicator::getInstance();

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
            std::string val;
            std::cin >> val;
            uint8_t tm[2048];
            uint8_t v = (uint8_t) std::stoi(val);
            for (int i = 0; i < 2048; i++)
            {
                tm[i] = v;
            }
            std::cout << "Inverter Mode (1 = Torque, ¬1 = Speed): ";
            std::cin >> val;
            CommsSchema::InverterMode im;
            switch (std::stoi(val))
            {
            case 1:
                im = CommsSchema::InverterMode_TORQUE;
                break;

            default:
                im = CommsSchema::InverterMode_SPEED;
                break;
            }
            std::cout << "Disable Torque Requests (Bool): ";
            bool dtr;
            std::cin >> std::boolalpha >> dtr;
            std::cout << "APPS 1 ADC Min (uint16): ";
            std::string a1min;
            std::cin >> a1min;
            std::cout << "APPS 1 ADC Max (uint16): ";
            std::string a1max;
            std::cin >> a1max;
            std::cout << "APPS 2 ADC Min (uint16): ";
            std::string a2min;
            std::cin >> a2min;
            std::cout << "APPS 2 ADC Max (uint16): ";
            std::string a2max;
            std::cin >> a2max;
            std::cout << "BPS ADC Min (uint16): ";
            std::string bmin;
            std::cin >> bmin;
            std::cout << "BPS ADC Max (uint16): ";
            std::string bmax;
            std::cin >> bmax;
            std::cout << "BPS Fully Pressed Threshold (uint32): ";
            std::string bpst;
            std::cin >> bpst;
            std::cout << "Enable Lapsim Testbench (Bool): ";
            bool elt;
            std::cin >> std::boolalpha >> elt;
            std::cout << "Lapsim Testbench Laps (uint8): ";
            std::string ltl;
            std::cin >> ltl;
            std::cout << std::to_string((uint8_t) std::stoi(ltl)) << std::endl;

            flatbuffers::FlatBufferBuilder builder(1024);

            CommsSchema::VariableVals vals(tm,
                                           im,
                                           dtr,
                                           (uint16_t) std::stoi(a1min),
                                           (uint16_t) std::stoi(a1max),
                                           (uint16_t) std::stoi(a2min),
                                           (uint16_t) std::stoi(a2max),
                                           (uint16_t) std::stoi(bmin),
                                           (uint16_t) std::stoi(bmax),
                                           (uint32_t) std::stoi(bpst),
                                           elt,
                                           (uint8_t) std::stoi(ltl));

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
