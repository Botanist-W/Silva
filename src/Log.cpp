// SILVA
#include "Log.h"

std::shared_ptr<spdlog::logger> Log::s_Logger;


void Log::Init() {

    spdlog::set_pattern("%^[%T] %n: %v%$");

    // Use thread-safe sinks
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
   // auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/logfile.txt", true);
    spdlog::sinks_init_list sink_list = { console_sink };

    s_Logger = std::make_shared<spdlog::logger>("SILVA", sink_list);

#ifdef NDEBUG
    s_Logger->set_level(spdlog::level::info);
#else
    s_Logger->set_level(spdlog::level::trace); 
#endif

    spdlog::set_default_logger(s_Logger);

}