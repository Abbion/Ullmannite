#include "Logger.h"

void Ull::InitLog()
{
    if (plog::get() != NULL)
    {
        ULOGW("Logger is already initialized");
        return;
    }

    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::Severity::verbose, &consoleAppender);
}

void Ull::SetLogSeverity(plog::Severity severity)
{
    plog::get()->setMaxSeverity(severity);
}

void Ull::ClearLogs()
{
#ifdef _WIN32
    system("cls");
#elif __linux__
    system("clear");
#endif
}