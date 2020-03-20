#include "CommonsLibrary/DebugTools/Logger.h"
#include <chrono>
#include <time.h>

namespace CommonsLibrary
{
    Logger* Singleton<Logger>::m_instance = nullptr;

    Logger::Logger() : Singleton()
    {
        m_outputFile.open(std::string(fileName) + ".txt");
    }

    Logger::~Logger()
    {
        m_outputFile.close();
    }

    void Logger::Log(const std::string& message)
    {
        std::unique_ptr<char[]> timeBuffer = std::make_unique<char[]>(26);
        time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        ctime_s(&timeBuffer[0], 26, &time);
        std::string output = timeBuffer.get() + message + "\n\n";
        m_outputFile << output;
    }
}