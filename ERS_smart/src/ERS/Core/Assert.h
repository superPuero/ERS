#pragma once

#ifdef ERS_ENABLE_ASSERTS
	#define ERS_ASSERT(expected) expected ? \
		ERS_LOG("ASSERT:", "SUCSSESS"	/*, "in", __FILE__*/, "at line", __LINE__, "in", __func__, "function") :\
		ERS_LOG("ASSERT:",	"FAIL."		/*, "in", __FILE__*/, "at line", __LINE__, "in", __func__, "function"), ERS_DEBUGBREAK()
#else
	#define ERS_ASSERT(expected, reality) 
#endif // ERS_ENABLE_ASSERTS
