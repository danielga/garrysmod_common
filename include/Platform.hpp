/*************************************************************************
* Platform macros
* A C++ header that defines macros depending on the compilation
* environment and targets.
*------------------------------------------------------------------------
* Copyright (c) 2017-2019, Daniel Almeida
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/

#pragma once

#if defined __clang__

#define COMPILER_CLANG 1

#elif defined __GNUC__

#define COMPILER_GNUC 1

#elif defined _MSC_VER

#define COMPILER_VC 1

#else

#error "Unknown compiler. Probably not supported by Garry's Mod."

#endif

#ifdef COMPILER_VC

#define DEPRECATED \
	__declspec( deprecated( "deprecated, avoid using this" ) )

#define DEPRECATED_WITH_SUBSTITUTE( substitute ) \
	__declspec( deprecated( "deprecated, use " #substitute " instead" ) )

#else

#define DEPRECATED \
	__attribute__( ( deprecated( "avoid using this" ) ) )

#define DEPRECATED_WITH_SUBSTITUTE( substitute ) \
	__attribute__( ( deprecated( "use " #substitute " instead" ) ) )

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

#error "Unknown platform. Probably not supported by Garry's Mod."

#endif

#if defined _M_IX86 || defined ___i386__ || defined __i386 || defined __X86__ || defined _X86_ || defined __I86__

#define ARCHITECTURE_X86 1

#elif defined _M_X64 || defined __amd64__ || defined __amd64 || defined __x86_64__ || defined __x86_64

#define ARCHITECTURE_X86_64 1

#else

#error "Unknown architecture. Probably not supported by Garry's Mod."

#endif
