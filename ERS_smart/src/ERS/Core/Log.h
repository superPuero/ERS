#pragma once
#include <iostream>

namespace ers {
	template<typename...L>
	inline void log_function(L...args) {
		((std::cout << args << ' '), ...);
		std::cout << '\n';

	}

#define ERS_LOG(...) log_function(__VA_ARGS__)


#ifdef ERS_ENABLE_INFO
#define ERS_LOG_INFO(...)		ERS_LOG("INFO:",	 __VA_ARGS__)
#else
#define ERS_LOG_INFO(...) 
#endif // ERS_INFO


#ifdef ERS_ENABLE_WARNINGS
#define ERS_LOG_WARNING(...)	ERS_LOG("WARNING:", __VA_ARGS__/*, "in", __FILE__, "at line", __LINE__, "in", __func__, "function"*/)
#else
#define ERS_LOG_WARNING(...)
#endif // ERS_INFO


#ifndef ERS_DISABLE_ERRORS
#define ERS_LOG_CRITICAL_ERROR(...)	ERS_LOG("CRITICAL ERROR:",	__VA_ARGS__/*, "in", __FILE__, "at line", __LINE__, "in", __func__, "function"*/), ERS_DEBUGBREAK()
#define ERS_LOG_ERROR(...)			ERS_LOG("ERROR:",	__VA_ARGS__/*, "in", __FILE__, "at line", __LINE__, "in", __func__, "function"*/)
#else
#define ERS_LOG_CRITICAL_ERROR(...)
#define ERS_LOG_ERROR(...)
#endif // ERS_INFO
}