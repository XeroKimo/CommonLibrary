#pragma once
#include "../Singleton.h"
#include <fstream>

namespace CommonsLibrary
{
    constexpr char fileName[] = "Log";
    class Logger : public Singleton<Logger>
    {
        friend class Singleton<Logger>;
    private:

    private:
        std::ofstream m_outputFile;

    private:
        Logger();
    public:
        ~Logger();

    public:
        void Log(const std::string& message);
    };
}