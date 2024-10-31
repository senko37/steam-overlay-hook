#include "utils.hpp"

UINT64 utils::signatureScan(UINT64 base, const char* pattern, const char* mask) {
    PIMAGE_DOS_HEADER dos = reinterpret_cast<PIMAGE_DOS_HEADER>(base);
    PIMAGE_NT_HEADERS64 nt = reinterpret_cast<PIMAGE_NT_HEADERS64>(base + dos->e_lfanew);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || nt->Signature != IMAGE_NT_SIGNATURE)
        return 0;

    int maskLen = strlen(mask);
    char* bytes = reinterpret_cast<char*>(base);
    for (int i = 0; i < nt->OptionalHeader.SizeOfImage - maskLen; i++) {
        for (int j = 0; j < maskLen; j++) {
            if (bytes[i + j] != pattern[j] && mask[j] != '?')
                break;
            else if (j == maskLen - 1)
                return base + i;
        }
    }
    return 0;
}