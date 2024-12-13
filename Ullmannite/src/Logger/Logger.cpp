#include "Ullpch.h"
#include "Logger.h"
#include "Layer/Layer.h"
#include "UiElement/UiLayout.h"
#include "UiElement/UiRenderArea.h"
#include "UiElement/UiElement.h"

plog::util::nstring Ull::SimpleTxtFormatterImpl::format(const plog::Record& record)
{
    {
        tm t;
        plog::util::localtime_s(&t, &record.getTime().time);

        plog::util::nostringstream ss;
        ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int> (record.getTime().millitm) << PLOG_NSTR(" ");
        ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << severityToString(record.getSeverity()) << PLOG_NSTR(" ");
        ss << PLOG_NSTR("[") << record.getFunc() << PLOG_NSTR("@") << record.getLine() << PLOG_NSTR("] ");
        ss << record.getMessage() << PLOG_NSTR("\n");

        return ss.str();
    }
}

void Ull::InitLog()
{
    if (plog::get() != NULL)
    {
        ULOGW("Logger is already initialized");
        return;
    }

    static plog::ColorConsoleAppender<SimpleTxtFormatterImpl> consoleAppender;
    plog::init(plog::Severity::verbose, &consoleAppender);
}

void Ull::SetLogSeverity(plog::Severity severity)
{
    plog::get()->setMaxSeverity(severity);
}

void Ull::ClearLogs()
{
#ifdef PLATFORM_WINDOWS
    system("cls");
#elif PLATFORM_LINUX
    system("clear");
#endif
}

void Ull::LogLayerTree(const Ull::Layer& layer)
{
    std::stringstream ss;
    ss << "\nLayer " << layer.GetName() << "\n\t+ "
       << layer.GetLayout()->GetName() << "\n";

    LogElements(ss, layer.GetLayout()->GetChildren(), 2);
    

   ULOGD(ss.str().c_str());
}

void Ull::LogElements(std::stringstream& ss, const std::vector<std::shared_ptr<Ull::UiElement>> elements, unsigned int tabs)
{
    std::string depthStr;
    depthStr.resize(tabs);
    std::fill_n(depthStr.begin(), tabs, '\t');

    for (const auto element : elements)
    {
        ss << depthStr << "+ " << element->GetName() << "\n";
        LogElements(ss, element->GetChildren(), tabs + 1);
    }
}