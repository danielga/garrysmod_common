#define IS_SERVERSIDE

#include "Symbols.hpp"
#include "Symbol.hpp"
#include "Platform.hpp"

namespace Symbols
{
	const std::vector<Symbol> HandleClientLuaError = {

#if defined SYSTEM_WINDOWS

#if defined ARCHITECTURE_X86_64

		Symbol::FromName( "?HandleClientLuaError@@YAXPEAVCBasePlayer@@PEBD@Z" )

#elif defined ARCHITECTURE_X86

		Symbol::FromName( "?HandleClientLuaError@@YAXPAVCBasePlayer@@PBD@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x83\xEC\x08\x8B\x0D\x2A\x2A\x2A\x2A\x57\x8B\x7D\x08" )

#endif

#elif defined SYSTEM_POSIX

		Symbol::FromName( "@_Z20HandleClientLuaErrorP11CBasePlayerPKc" ),

#if defined SYSTEM_LINUX && ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x4C\x65\xA1\x2A\x2A\x2A\x2A\x89\x45\xE4" )

#endif

#endif

	};

#if defined SYSTEM_WINDOWS

	const std::vector<Symbol> FileSystemFactory = {

#if defined ARCHITECTURE_X86_64

		Symbol::FromName( "?FileSystemFactory@@YAPEAXPEBDPEAH@Z" )

#elif defined ARCHITECTURE_X86

		Symbol::FromName( "?FileSystemFactory@@YAPAXPBDPAH@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x68\x2A\x2A\x2A\x2A\xFF\x75\x08\xE8" )

#endif

	};

#if defined ARCHITECTURE_X86_64

	const Symbol g_pFullFileSystem = Symbol::FromName( "?g_pFullFileSystem@@3PEAVIFileSystem@@EA" );

#elif defined ARCHITECTURE_X86

	const Symbol g_pFullFileSystem = Symbol::FromName( "?g_pFullFileSystem@@3PAVIFileSystem@@A" );

#endif

#elif defined SYSTEM_POSIX

	const std::vector<Symbol> FileSystemFactory = { Symbol::FromName( "_Z17FileSystemFactoryPKcPi" ) };

	const Symbol g_pFullFileSystem = Symbol::FromName( "g_pFullFileSystem" );

#endif

}
