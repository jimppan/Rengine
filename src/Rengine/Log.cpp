#include "PCH.h"

#include "Log.h"

namespace Rengine
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	bool Log::m_bLogInitialized = false;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("RENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APPLICATION");
		s_ClientLogger->set_level(spdlog::level::trace);

		m_bLogInitialized = true;
	}

	void Log::LogMsg(spdlog::logger* logger, LogType logType, std::string msg)
	{
		switch (logType)
		{
		case LogType::LOG_TRACE:
			logger->trace(msg);
			break;
		case LogType::LOG_INFO:
			logger->info(msg);
			break;
		case LogType::LOG_WARN:
			logger->warn(msg);
			break;
		case LogType::LOG_ERROR:
			logger->error(msg);
			break;
		}
	}
}
