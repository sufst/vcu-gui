#include "Communicator.hpp"
#include <iostream>

int main(int argc, char const* argv[])
{
    Communicator* c = Communicator::getInstance();

    std::string cmd;
    while (cmd != "exit")
    {
        std::cout << "Command: ";
        std::cin >> cmd;
        if (cmd == "save")
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
        else if (cmd == "exit")
        {
            break;
        }
        else
        {
            std::cout << "Not a valid command" << std::endl;
        }
    }

    return 0;
}
