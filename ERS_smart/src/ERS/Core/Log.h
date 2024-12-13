#pragma once
#include "echolog.h"

#define ERS_LOG(text_color, background_color, style, ...) echolog::log_decorated(text_color, background_color, style, __VA_ARGS__)

#ifdef ERS_ENABLE_INFO
#define ERS_LOG_INFO(...)		ERS_LOG(ECHO_COLOR_GREEN, ECHO_COLOR_DEFAULT, ECHO_STYLE_DEFAULT, "INFO:",	 __VA_ARGS__)
#else
#define ERS_LOG_INFO(...) 
#endif // ERS_INFO


#ifdef ERS_ENABLE_WARNINGS
#define ERS_LOG_WARNING(...)	ERS_LOG(ECHO_COLOR_YELLOW, ECHO_COLOR_DEFAULT, ECHO_STYLE_DEFAULT, "WARNING:", __VA_ARGS__/*, "in", __FILE__, "at line", __LINE__, "in", __func__, "function"*/)
#else
#define ERS_LOG_WARNING(...)
#endif // ERS_INFO


#ifndef ERS_DISABLE_ERRORS
#define ERS_LOG_CRITICAL_ERROR(...)	ERS_LOG(ECHO_COLOR_BLACK, ECHO_COLOR_RED, ECHO_STYLE_BLINKING,"CRITICAL ERROR:",	__VA_ARGS__/*, "in", __FILE__, "at line", __LINE__, "in", __func__, "function"*/), ERS_DEBUGBREAK()
#define ERS_LOG_ERROR(...)			ERS_LOG(ECHO_COLOR_RED, ECHO_COLOR_DEFAULT, ECHO_STYLE_DEFAULT, "ERROR:",	__VA_ARGS__/*, "in", __FILE__, "at line", __LINE__, "in", __func__, "function"*/)
#else
#define ERS_LOG_CRITICAL_ERROR(...)
#define ERS_LOG_ERROR(...)
#endif // ERS_INFO
