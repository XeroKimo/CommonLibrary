#pragma once
#include "CommonsLibrary/StdHelpers/StringHelpers.h"
#include <fstream>
#include <vector>

namespace CommonsLibrary
{
    class SerialOutputFile
    {
        std::ofstream m_outputStream;

    public:
        SerialOutputFile(std::string fileName);
        ~SerialOutputFile();

    public:
        void Write(std::string name, std::string value);
        void Write(std::string name, int value);
        void Write(std::string name, long long value);
        void Write(std::string name, float value);
        void Write(std::string name, double value);

    public:
        void Write(std::string name, const std::vector<std::string>& values);
        void Write(std::string name, const std::vector<int>& values);
        void Write(std::string name, const std::vector<long long>& values);
        void Write(std::string name, const std::vector<float>& values);
        void Write(std::string name, const std::vector<double>& values);
    };
}