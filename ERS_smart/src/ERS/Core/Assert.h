#pragma once
#ifndef ERS_DISABLE_ASSERTS
	#ifdef ERS_ENABLE_ASSERTS_LOG
		#define ERS_ASSERT(expression) expression ? \
			ERS_LOG(ECHO_COLOR_BLACK, ECHO_COLOR_GREEN, ECHO_STYLE_DEFAULT, "Assert:", "Success."					, /*"file:", __FILE__,*/ "line", __LINE__, "in", __func__) :\
			(ERS_LOG(ECHO_COLOR_BLACK , ECHO_COLOR_RED, ECHO_STYLE_DEFAULT , "Assert:",	"Failed."					, /*"file:", __FILE__,*/ "line", __LINE__, "in", __func__), ERS_DEBUGBREAK())

	#else
		#define ERS_ASSERT(expression) if(!expression) ERS_DEBUGBREAK()

	#endif // ERS_ENABLE_ASSERTS_LOG

	#define ERS_ASSERT_MSG(expression, ...) expression ? \
				ERS_LOG(ECHO_COLOR_BLACK, ECHO_COLOR_GREEN, ECHO_STYLE_DEFAULT, "Assert:", "Success."			    /*, "file:", __FILE__,*/ "line", __LINE__, "in", __func__) :\
				(ERS_LOG(ECHO_COLOR_BLACK, ECHO_COLOR_RED, ECHO_STYLE_DEFAULT, "Assert:",	"Failed."				/*, "file:", __FILE__,*/ "line", __LINE__, "in", __func__), ERS_DEBUGBREAK())
#else	
	#define ERS_ASSERT(expression)
	#define ERS_ASSERT_MSG(expression, ...)
#endif // !ERS_DISABLE_ASSERTS

