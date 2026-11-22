#include <Platform.hpp>

// Visual Studio 2015 to Visual Studio 2026
#if defined SYSTEM_WINDOWS && (_MSC_VER < 1900 || _MSC_VER > 1952)

#error The only supported compilation platforms for this project on Windows are Visual Studio 2015, 2017, 2019, 2022, and 2026 (for ABI compatibility reasons).

#endif
