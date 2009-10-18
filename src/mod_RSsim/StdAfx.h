/////////////////////////////////////////////////////////////////////////////
//
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
// See _README.CPP
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDAFX_H__99C3C71E_6549_443C_B532_6557AB4C5095__INCLUDED_)
#define AFX_STDAFX_H__99C3C71E_6549_443C_B532_6557AB4C5095__INCLUDED_

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
	#define sscanf_s(a,b,c)			sscanf(a,b,c)

#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxmt.h>			// MFC Multithreading
#include <afxtempl.h>
#include <math.h>
#include <WinSock2.h>      // windows sockets APIs

#include "..\mylib\mylib.h"

#include "DDKSocket.h"     // Ethernet TCP/IP socket
#include "DDKSrvSocket.h"  // Server end of socket
#include "ServerSocketArray.h"
#include "util.h"          // globally used application functions & macros

// Vinay
#include "CsvReader.h"

#include "MemWriteLock.h"
#include "MemoryEditorList.h"
#include "ServerRS232Array.h"
#include "ServerSocketArray.h"
#include "splash.h"
#include "ActiveScriptHost.h"    // Active scripting class by Ernest Laurentin (Code-Project)
#include "MyHostProxy.h"

#include "mod_RSsim.h"
#include "htmloutput.h"
#include "mod_RSsimdlg.h"

#include "MemoryEditorList.h"
#include "RS232Port.h"
#include "simport.h"
#include "RS232Noise.h"
#include "MODEthCommsProcessor.h"
#include "MOD232CommsProcessor.h"
#include "AdvEmulationDlg.h"



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__99C3C71E_6549_443C_B532_6557AB4C5095__INCLUDED_)
