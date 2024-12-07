#pragma once

#define ERS_DEBUGBREAK() __debugbreak();

#ifdef ERS_ENABLE_DEBUG
	#define ERS_ENABLE_ASSERTS
	#define ERS_ENABLE_WARNINGS

#endif //ERS_ENABLE_DEBUG
