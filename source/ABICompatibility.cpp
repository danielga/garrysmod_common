#include <Platform.hpp>

#if defined SYSTEM_WINDOWS && ( _MSC_VER < 1910 || _MSC_VER > 1999 )

#error The only supported compilation platform for this project on Windows is Visual Studio 2017 (for ABI compatibility reasons).

#endif
