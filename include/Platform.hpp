#pragma once

#if defined __clang__

#define COMPILER_CLANG 1

#elif defined __GNUC__

#define COMPILER_GNUC 1

#elif defined _MSC_VER

#define COMPILER_VC 1

#else

#error Unknown compiler. Probably not supported by Garry's Mod.

#endif

#if defined _WIN32

#define SYSTEM_WINDOWS 1

#elif defined __linux

#define SYSTEM_LINUX 1
#define SYSTEM_POSIX 1

#elif defined __APPLE__

#define SYSTEM_MACOSX 1
#define SYSTEM_POSIX 1

#include <AvailabilityMacros.h>

#if MAC_OS_X_VERSION_MIN_REQUIRED < 1090

#define SYSTEM_MACOSX_BAD 1

#endif

#else

#error Unknown platform. Probably not supported by Garry's Mod.

#endif

#if defined _M_IX86 || defined ___i386__ || defined __i386 || defined __X86__ || defined _X86_ || defined __I86__

#define ARCHITECTURE_X86 1

#elif defined _M_X64 || defined __amd64__ || defined __amd64 || defined __x86_64__ || defined __x86_64

#define ARCHITECTURE_X86_64 1

#else

#error Unknown architecture. Probably not supported by Garry's Mod.

#endif
