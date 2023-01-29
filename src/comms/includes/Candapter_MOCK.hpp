#pragma once

#include <fstream>
#include <iostream>
#include <string>

class Candapter_MOCK
{
public:

    static void sendMsg(std::string name, uint8_t* data, int size)
    {
        auto file = std::fstream(name, std::ios::binary | std::ios::out);
        file.write((char*) data, size);
        file.close();
    }

    static uint8_t* getMsg(std::string name)
    {
        std::ifstream infile;
        infile.open(name, std::ios::binary | std::ios::in);
        infile.seekg(0, std::ios::end);
        int length = infile.tellg();
        infile.seekg(0, std::ios::beg);
        uint8_t* data = new uint8_t[length];
        infile.read((char*) data, length);
        infile.close();

        return data;
    }
};