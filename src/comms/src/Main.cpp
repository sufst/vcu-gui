#include "Communicator.hpp"
#include <iostream>

int main(int argc, char const* argv[])
{
    Communicator* c = Communicator::getInstance();
    std::string n;
    std::cout << "Config name: ";
    std::cin >> n;
    std::cout << std::endl;
    std::string v;
    std::cout << "Config version: ";
    std::cin >> v;
    std::cout << std::endl;
    c->save(n, v);
    return 0;
}
