#if defined _WIN32 && ( _MSC_VER < 1910 || _MSC_VER > 1999 )

#error The only supported compilation platform for this project on Windows is Visual Studio 2017 (for ABI compatibility reasons).

#elif defined __linux && ( __GNUC__ != 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 4 ) )

#error The only supported compilation platforms for this project on Linux are GCC 4.4 to 4.9 (for ABI compatibility reasons).

#elif defined __APPLE__

#include <AvailabilityMacros.h>

#if MAC_OS_X_VERSION_MIN_REQUIRED != 1050

#error The only supported compilation platform for this project on Mac OS X is GCC with Mac OS X 10.5 SDK (for ABI compatibility reasons).

#endif

#endif
