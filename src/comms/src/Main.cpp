#include "Communicator.hpp"
#include <iostream>

int main(int argc, char const* argv[])
{
    Communicator* c = Communicator::getInstance();
    std::string v;
    std::cin >> v;
    c->setConfigDetails("a", v);
    return 0;
}
