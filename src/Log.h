// SILVA
#pragma once

#include "spdlog/spdlog.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"


// Simple log class to make my life HOPEFULLY easier (thanks the Cherno)
class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:

	static std::shared_ptr<spdlog::logger> s_Logger;

};




// Core macros 
// For debug
#define LOG_DEBUG(...) ::Log::GetLogger()->debug(__VA_ARGS__) // just some chekcs for memory managment etc
#define LOG_TRACE(...) ::Log::GetLogger()->trace(__VA_ARGS__) // This should where the values in methods should be logged
#define LOG_INFO(...)  ::Log::GetLogger()->info(__VA_ARGS__) // This is the timings etc ... to be written to logger txt

// For release
#define LOG_WARN(...)  ::Log::GetLogger()->warn(__VA_ARGS__) // Stange values etc that are a need to know if ther is an error
#define LOG_ERROR(...) ::Log::GetLogger()->error(__VA_ARGS__) // When the program breaks


