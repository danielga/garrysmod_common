#include <Platform.hpp>

// Visual Studio 2015 to Visual Studio 2019
#if defined SYSTEM_WINDOWS && ( _MSC_VER < 1900 || _MSC_VER > 1929 )

#error The only supported compilation platforms for this project on Windows are Visual Studio 2015, 2017 and 2019 (for ABI compatibility reasons).

#endif
