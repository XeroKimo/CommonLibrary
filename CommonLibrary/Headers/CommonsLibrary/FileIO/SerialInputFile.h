#pragma once
#include "CommonsLibrary/StdHelpers/StringHelpers.h"
#include <fstream>
#include <vector>

namespace CommonsLibrary
{
    class SerialInputFile
    {
        std::ifstream m_inputStream;
        std::string m_currentLine;
    public:
        SerialInputFile(std::string fileName);
        ~SerialInputFile();

    public:
        bool IsNameMatching(std::string name);

        int ReadInt();
        long long ReadLongLong();
        float ReadFloat();
        double ReadDouble();
        std::string ReadString();

    public:
        std::vector<int> ReadIntArray();
        std::vector<long long> ReadLongLongArray();
        std::vector<float> ReadFloatArray();
        std::vector<double> ReadDoubleArray();
        std::vector<std::string> ReadStringArray();
    private:
        void GetLine();
    };
}