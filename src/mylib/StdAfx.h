// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__090A43A7_F5DE_450E_B572_6EB2EBB7E8E4__INCLUDED_)
#define AFX_STDAFX_H__090A43A7_F5DE_450E_B572_6EB2EBB7E8E4__INCLUDED_

#define _WIN32_WINNT 0x502   // Target XP SP2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if     _MSC_VER > 1200	//VC 9
	#define _WIN32_WINNT 0x502   // Target XP SP2
#else
	
	// "unsafe" CRT functions which I've changed to use the safe version in subsequent CRT.
	// macros defined here temporally bypass the non-existent safe versions.
	#define strncpy_s(a,b,c,d)		strncpy(a,c,d)
	#define strcpy_s(a,b,c)			strcpy(a,c)
	#define sprintf_s(a,b,c)		sprintf(a,c)
	#define sprintf_s(a,b,c,d)		sprintf(a,c,d)
	#define strtok_s(a,b,c)			strtok(a,b)

	#define _itoa_s(a,b,c,d)				itoa(a,b,d)
	#define _ltoa_s(a,b,c,d)				ltoa(a,b,d)
	#define _ultoa_s(a,b,c,d)				ultoa(a,b,d)
	#define _gcvt_s(a,b,c,d)				gcvt(c,d,a)

#endif


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>

// TODO: reference additional headers your program requires here
#include "mylib.h"
#include "ado.h"
#include "statlink.h"
#include "colorstatic.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__090A43A7_F5DE_450E_B572_6EB2EBB7E8E4__INCLUDED_)
