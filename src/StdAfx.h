// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5A8C4A14_B713_44A8_A1FB_0ECDC81D5D31__INCLUDED_)
#define AFX_STDAFX_H__5A8C4A14_B713_44A8_A1FB_0ECDC81D5D31__INCLUDED_
#pragma once


#include "Tools/PragmaSet.h"

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// STLPort specific
#include <string> // concatination optimization if STLPort used
#include <new>
#include <vector>
#include <algorithm>
#include <hash_map>


#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT
//
//#include <iostream>

// TODO: reference additional headers your program requires here


inline CString FormatStr(LPCSTR pszFormat, ...)
{
	va_list arglist;
	va_start(arglist, pszFormat);

	CString str;
	str.FormatV(pszFormat, arglist);

	va_end(arglist);

	return str;
}



#endif // !defined(AFX_STDAFX_H__5A8C4A14_B713_44A8_A1FB_0ECDC81D5D31__INCLUDED_)
