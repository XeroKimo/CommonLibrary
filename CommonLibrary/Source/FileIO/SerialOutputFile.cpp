
#include "CommonsLibrary/FileIO/SerialOutputFile.h" 

namespace CommonsLibrary
{
    SerialOutputFile::SerialOutputFile(std::string fileName)
    {
        m_outputStream.open(fileName + ".sso");
    }

    SerialOutputFile::~SerialOutputFile()
    {
        m_outputStream.close();
    }

    void SerialOutputFile::Write(std::string name, std::string value)
    {
        m_outputStream << name + ": \"" + value + "\"\n";
    }

    void SerialOutputFile::Write(std::string name, int value)
    {
        m_outputStream << name + ": \"" + Int::ToString(value) + "\"\n";
    }

    void SerialOutputFile::Write(std::string name, long long value)
    {
        m_outputStream << name + ": \"" + LongLong::ToString(value) + "\"\n";
    }

    void SerialOutputFile::Write(std::string name, float value)
    {
        m_outputStream << name + ": \"" + Float::ToString(value) + "\"\n";
    }

    void SerialOutputFile::Write(std::string name, double value)
    {
        m_outputStream << name + ": \"" + Double::ToString(value) + "\"\n";
    }

    void SerialOutputFile::Write(std::string name, const std::vector<std::string>& values)
    {
        m_outputStream << name + ": ";
        for (size_t i = 0; i < values.size(); i++)
        {
            m_outputStream << "\"" + values[i] + "\"";
            if (i < values.size() - 1)
                m_outputStream << ", ";
            else
                m_outputStream << "\n";
        }
    }

    void SerialOutputFile::Write(std::string name, const std::vector<int>& values)
    {
        m_outputStream << name + ": \"";
        for (size_t i = 0; i < values.size(); i++)
        {
            m_outputStream << Int::ToString(values[i]);
            if (i < values.size() - 1)
                m_outputStream << "\", \"";
            else
                m_outputStream << "\"\n";
        }
    }

    void SerialOutputFile::Write(std::string name, const std::vector<long long>& values)
    {
        m_outputStream << name + ": \"";
        for (size_t i = 0; i < values.size(); i++)
        {
            m_outputStream << LongLong::ToString(values[i]);
            if (i < values.size() - 1)
                m_outputStream << "\", \"";
            else
                m_outputStream << "\"\n";
        }
    }

    void SerialOutputFile::Write(std::string name, const std::vector<float>& values)
    {
        m_outputStream << name + ": \"";
        for (size_t i = 0; i < values.size(); i++)
        {
            m_outputStream << Float::ToString(values[i]);
            if (i < values.size() - 1)
                m_outputStream << "\", \"";
            else
                m_outputStream << "\"\n";
        }
    }

    void SerialOutputFile::Write(std::string name, const std::vector<double>& values)
    {
        m_outputStream << name + ": \"";
        for (size_t i = 0; i < values.size(); i++)
        {
            m_outputStream << Double::ToString(values[i]);
            if (i < values.size() - 1)
                m_outputStream << "\", \"";
            else
                m_outputStream << "\"\n";
        }
    }
}