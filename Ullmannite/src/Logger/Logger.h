#pragma once
#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Formatters/TxtFormatter.h>

namespace Ull
{
	class SimpleTxtFormatterImpl
	{
	public:
		static plog::util::nstring header() { return plog::util::nstring(); }
		static plog::util::nstring format(const plog::Record& record);
	};

	void InitLog();
	void SetLogSeverity(plog::Severity serverity);
	void ClearLogs();
}

#define ULOGF(message) PLOGF << message

#ifdef DEBUG
	#define ULOGV(message) PLOGV << message
	#define ULOGD(message) PLOGD << message
	#define ULOGI(message) PLOGI << message
	#define ULOGW(message) PLOGW << message
	#define ULOGE(message) PLOGE << message
	#define ULOGN(message) PLOGN << message
	
	#ifdef PLATFORM_WINDOWS
		#define UASSERT(condition, message) { if(!condition) {PLOGF << "Assert! " << message; __debugbreak(); } }
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