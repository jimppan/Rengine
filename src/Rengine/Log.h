#ifndef RENGINE_LOG_H
#define RENGINE_LOG_H

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Rengine
{ 
	class Log
	{
	public:

		enum LogType
		{
			LOG_TRACE,
			LOG_INFO,
			LOG_WARN,
			LOG_ERROR
		};
		 static void Init();

		 inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		 inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		 static void LogMsg(spdlog::logger* logger, LogType logType, std::string msg);

		 inline static bool IsInitialized() { return m_bLogInitialized; }

	private:
		 static std::shared_ptr<spdlog::logger> s_CoreLogger;
		 static std::shared_ptr<spdlog::logger> s_ClientLogger;

		 static bool m_bLogInitialized;
	};
}

#if defined RENGINE_DEBUG || defined RENGINE_RELEASE

#define RENGINE_CORE_TRACE(...) if(::Rengine::Log::IsInitialized()) ::Rengine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RENGINE_CORE_INFO(...)  if(::Rengine::Log::IsInitialized()) ::Rengine::Log::GetCoreLogger()->info(__VA_ARGS__)   
#define RENGINE_CORE_WARN(...)  if(::Rengine::Log::IsInitialized()) ::Rengine::Log::GetCoreLogger()->warn(__VA_ARGS__)  
#define RENGINE_CORE_ERROR(...) if(::Rengine::Log::IsInitialized()) ::Rengine::Log::GetCoreLogger()->error(__VA_ARGS__)  

#define RENGINE_TRACE(...)      if(::Rengine::Log::IsInitialized()) ::Rengine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RENGINE_INFO(...)       if(::Rengine::Log::IsInitialized()) ::Rengine::Log::GetClientLogger()->info(__VA_ARGS__)
#define RENGINE_WARN(...)       if(::Rengine::Log::IsInitialized()) ::Rengine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RENGINE_ERROR(...)      if(::Rengine::Log::IsInitialized()) ::Rengine::Log::GetClientLogger()->error(__VA_ARGS__)

#define RENGINE_LOG_INIT() ::Rengine::Log::Init()
#else
#define RENGINE_CORE_TRACE
#define RENGINE_CORE_INFO
#define RENGINE_CORE_WARN
#define RENGINE_CORE_ERROR

#define RENGINE_TRACE
#define RENGINE_INFO
#define RENGINE_WARN
#define RENGINE_ERROR

#define RENGINE_LOG_INIT()
#endif


#endif // RENGINE_LOG_H
