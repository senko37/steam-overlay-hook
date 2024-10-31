#pragma once
#include <Windows.h>

namespace utils {
	UINT64 signatureScan(UINT64 base, const char* pattern, const char* mask);
}