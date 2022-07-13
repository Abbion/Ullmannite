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

#define ULOGF(x) PLOGF << x

#ifdef _DEBUG
	#define ULOGV(x) PLOGV << x
	#define ULOGD(x) PLOGD << x
	#define ULOGI(x) PLOGI << x
	#define ULOGW(x) PLOGW << x
	#define ULOGE(x) PLOGE << x
	#define ULOGN(x) PLOGN << x
#elif !_DEBUG
	#define ULOGV(x)
	#define ULOGD(x)
	#define ULOGI(x)
	#define ULOGW(x)
	#define ULOGE(x)
	#define ULOGN(x)
#endif