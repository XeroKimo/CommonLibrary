#include "CommonsLibrary/FileIO/SerialInputFile.h"

namespace CommonsLibrary
{
    SerialInputFile::SerialInputFile(std::string fileName)
    {
        m_inputStream.open(fileName + ".sso");
    }

    SerialInputFile::~SerialInputFile()
    {
        m_inputStream.close();
    }

    bool SerialInputFile::IsNameMatching(std::string name)
    {
        GetLine();
        bool objectFound = m_currentLine.find(name) != std::string::npos;
        m_inputStream.seekg(-static_cast<long long>(m_currentLine.size()), std::ios_base::cur);
        return objectFound;
    }

    int SerialInputFile::ReadInt()
    {
        GetLine();
        std::string findValue;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        findValue = m_currentLine.substr(startIndex, endIndex - startIndex);
        return Int::FromString(findValue);
    }

    long long SerialInputFile::ReadLongLong()
    {
        GetLine();
        std::string findValue;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        findValue = m_currentLine.substr(startIndex, endIndex - startIndex);
        return LongLong::FromString(findValue);
    }

    float SerialInputFile::ReadFloat()
    {
        GetLine();
        std::string findValue;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        findValue = m_currentLine.substr(startIndex, endIndex - startIndex);
        return Float::FromString(findValue);
    }

    double SerialInputFile::ReadDouble()
    {
        GetLine();
        std::string findValue;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        findValue = m_currentLine.substr(startIndex, endIndex - startIndex);
        return Double::FromString(findValue);
    }

    std::string SerialInputFile::ReadString()
    {
        GetLine();
        std::string findValue;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        findValue = m_currentLine.substr(startIndex, endIndex - startIndex);
        return findValue;
    }

    std::vector<int> SerialInputFile::ReadIntArray()
    {
        GetLine();
        std::vector<int> values;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        do
        {
            values.push_back(Int::FromString(m_currentLine.substr(startIndex, endIndex - startIndex)));

            startIndex = m_currentLine.find("\"", endIndex + 2) + 1;
            endIndex = m_currentLine.find("\"", startIndex);

        } while (endIndex + 1 != m_currentLine.size());

        values.push_back(Int::FromString(m_currentLine.substr(startIndex, m_currentLine.size() - startIndex)));

        return values;
    }

    std::vector<long long> SerialInputFile::ReadLongLongArray()
    {
        GetLine();
        std::vector<long long> values;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        do
        {
            values.push_back(LongLong::FromString(m_currentLine.substr(startIndex, endIndex - startIndex)));

            startIndex = m_currentLine.find("\"", endIndex + 2) + 1;
            endIndex = m_currentLine.find("\"", startIndex);

        } while (endIndex + 1 != m_currentLine.size());

        values.push_back(LongLong::FromString(m_currentLine.substr(startIndex, m_currentLine.size() - startIndex)));

        return values;
    }

    std::vector<float> SerialInputFile::ReadFloatArray()
    {
        GetLine();
        std::vector<float> values;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        do
        {
            values.push_back(Float::FromString(m_currentLine.substr(startIndex, endIndex - startIndex)));

            startIndex = m_currentLine.find("\"", endIndex + 2) + 1;
            endIndex = m_currentLine.find("\"", startIndex);

        } while (endIndex + 1 != m_currentLine.size());

        values.push_back(Float::FromString(m_currentLine.substr(startIndex, m_currentLine.size() - startIndex)));

        return values;
    }

    std::vector<double> SerialInputFile::ReadDoubleArray()
    {
        GetLine();
        std::vector<double> values;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        do
        {
            values.push_back(Double::FromString(m_currentLine.substr(startIndex, endIndex - startIndex)));

            startIndex = m_currentLine.find("\"", endIndex + 2) + 1;
            endIndex = m_currentLine.find("\"", startIndex);

        } while (endIndex + 1 != m_currentLine.size());

        values.push_back(Double::FromString(m_currentLine.substr(startIndex, m_currentLine.size() - startIndex)));

        return values;
    }

    std::vector<std::string> SerialInputFile::ReadStringArray()
    {
        GetLine();
        std::vector<std::string> values;
        size_t startIndex = m_currentLine.find("\"") + 1;
        size_t endIndex = m_currentLine.find("\"", startIndex);
        do
        {
            values.push_back(m_currentLine.substr(startIndex, endIndex - startIndex));

            startIndex = m_currentLine.find("\"", endIndex + 2) + 1;
            endIndex = m_currentLine.find("\"", startIndex);

        } while (endIndex + 1 != m_currentLine.size());

        values.push_back(m_currentLine.substr(startIndex, endIndex - startIndex));

        return values;
    }

    void SerialInputFile::GetLine()
    {
        std::getline(m_inputStream, m_currentLine);
    }
}