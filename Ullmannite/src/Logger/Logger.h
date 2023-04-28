#pragma once
#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Formatters/TxtFormatter.h>

namespace Ull
{
	class Layer;
	class UiElement;

	class SimpleTxtFormatterImpl
	{
	public:
		static plog::util::nstring header() { return plog::util::nstring(); }
		static plog::util::nstring format(const plog::Record& record);
	};

	void InitLog();
	void SetLogSeverity(plog::Severity serverity);
	void ClearLogs();

	void LogLayerTree(const Layer& layer);
	void LogElements(std::stringstream& ss, const std::vector<std::shared_ptr<Ull::UiElement>> elements, unsigned int tabs = 0);
}

#define ULOGF(message) PLOGF << message

#ifdef DEBUG
	#define ULOGV(message) PLOGV << message
	#define ULOGD(message) PLOGD << message
	#define ULOGI(message) PLOGI << message
	#define ULOGW(message) PLOGW << message
	#define ULOGE(message) PLOGE << message
	#define ULOGN(message) PLOGN << message
	
	//Assert stops when condition is false
	#ifdef PLATFORM_WINDOWS
		#define UASSERT(condition, message) { if(!(condition)) {PLOGF << "Assert! " << message; __debugbreak(); } }
	#endif
	#ifdef PLATFORM_LINUX
		#define UASSERT(condition, message) { if(!condition) {PLOGF << "Assert! " << message; } }
	#endif

#elif RELEASE
	#define ULOGV(message)
	#define ULOGD(message)
	#define ULOGI(message)
	#define ULOGW(message)
	#define ULOGE(message)
	#define ULOGN(message)

	#define UASSERT(status, message)
#endif