#ifndef __PRAGMA_SET_H__
#define __PRAGMA_SET_H__

#pragma message("Pragma warnings and messages included")

// Disabling
// The given formal parameter was never referenced in the body of the function
// for which it was declared
#pragma warning  (disable : 4100)
// nonstandard extension used : class rvalue used as lvalue
#pragma warning (disable : 4238)
// signed/unsigned mismatch
#pragma warning (error : 4018)
// disables warning: identifier was truncated to '255' characters in the browser information
#pragma warning (disable : 4786)

//local variable may be used without having been initialized
#pragma warning (disable : 4701)

// warning C++ language change
#pragma warning (disable : 4663)

// cast truncates constant value
#pragma warning (disable : 4310)

// nonstandard extension used: 'argument' : conversion
#pragma warning (disable : 4239)

// nonstandard extension used : nameless struct/union
#pragma warning (disable : 4201)

// warning C4018: '<' : signed/unsigned mismatch
#pragma warning (disable : 4018)

//'function' : function not inlined
#pragma warning(default: 4710)

//'function' : function marked as __forceinline not inlined
#pragma warning(default: 4714)

// assignment operator could not be generated
#pragma warning (disable : 4512)

#ifdef _DEBUG
	//identifier : local variable is initialized but not referenced"
	#pragma warning (error : 4189)
	// local variable 'name' used without having been initialized
	#pragma warning(error: 4700)
	//'identifier' : unreferenced local variable
	#pragma warning(error: 4101)
#else
	//identifier : local variable is initialized but not referenced"
	#pragma warning(default: 4189)
	// local variable 'name' used without having been initialized
	#pragma warning(disable: 4700)
	//'identifier' : unreferenced local variable
	#pragma warning(default: 4101)

	#pragma warning( disable : 4552 )// '!=': operator has no effect

	#pragma warning( disable : 4553 ) // non dll-interface class
#endif

#pragma warning( disable : 4251 ) // DLL interface needed
#pragma warning( disable : 4275 ) // non dll-interface class

// End Trace functionality

#ifndef _DEBUG
	#define _WIN32_WINDOWS 0x0500
#endif

#ifdef _DEBUG
    #ifndef UNUSED
		#define UNUSED(x)
    #endif
#else
	#ifndef UNUSED
		#define UNUSED(x) x
	#endif
#endif

#ifndef UNUSED_ALWAYS
	#define UNUSED_ALWAYS(x) x
#endif

//#ifndef __DEBUG_UTILS_H__
//	#include "DebugUtils.h"
//#endif

#endif
