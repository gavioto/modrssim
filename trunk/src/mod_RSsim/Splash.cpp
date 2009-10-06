// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  // e. g. stdafx.h
#include "resource.h"  // e.g. resource.h

#include "Splash.h"  // e.g. splash.h

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;

int   CSplashWnd::m_splashShowCount;
int   CSplashWnd::m_splashShowDelay=1;

// ------------------------------- CSplashWnd --------------------------------
// constructor
CSplashWnd::CSplashWnd()
{
   //m_splashShowDelay = 1;   //default value
   m_pFont = NULL;
   m_pLargeFont = NULL;
}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
   if (NULL != m_pFont)
      delete m_pFont;
   if (NULL !=m_pLargeFont)
      delete m_pLargeFont;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// --------------------------- EnableSplashScreen ------------------------------
void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

// -------------------------- ShowSplashScreen ----------------------------------
void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

// ------------------------------ PreTranslateAppMessage ------------------------
BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
      // at least 1 seconds delay
      if (abs(m_splashShowDelay - m_splashShowCount) >=1)
      {
		   c_pSplashWnd->HideSplashScreen();
		   return TRUE;	// message handled here
      }
	}

	return FALSE;	// message not handled
}

// ---------------------------------- Create -------------------------------
BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
   
   if (!m_bitmap.LoadBitmap(IDB_SPLASH))
		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

// -------------------------------- HideSplashScreen -----------------------
void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
}

// --------------------------------- PostNcDestroy ----------------------
void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

// --------------------------------- OnCreate ---------------------------
int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
	SetTimer(1, 1000, NULL);
	m_splashShowCount = m_splashShowDelay;

	return 0;
}

#define LEFTMARGIN 45
// --------------------------------- OnPaint ------------------------------
void CSplashWnd::OnPaint()
{
CFont *pOldFont;
CRect clientRect;
CString tempText;

   CPaintDC dc(this);

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);

   ///////////////////////////////////////////////////////////
   // FIDDLE WITH TEXT ON TOP OF THE SPLASH
   if (NULL == m_pFont)
   {
   //CFont * pFont = GetFont(); // based on current font
   LOGFONT logFont;

      memset(&logFont,0,sizeof(logFont));
      logFont.lfHeight=18;
      logFont.lfWeight = 700;
      strcpy(logFont.lfFaceName, "Arial");
      m_pFont = new CFont;
      m_pFont->CreateFontIndirect(&logFont);
      
      logFont.lfHeight=26;
      logFont.lfWeight = 400; //normal
      m_pLargeFont = new CFont;
      m_pLargeFont->CreateFontIndirect(&logFont);
   }

   ASSERT(NULL != m_pFont);
   pOldFont = (CFont*)dc.SelectObject(m_pLargeFont);
   GetClientRect(&clientRect);

   dc.SetBkMode(TRANSPARENT);
   dc.SetTextColor(RGB(5,5,255));
   tempText = "MODBUS Serial-RTU, TCP/IP and";
   dc.TextOut(LEFTMARGIN,170, tempText);
   tempText = "Allen Bradley DF1 Protocols with \"Simulation\"";
   dc.TextOut(LEFTMARGIN,200, tempText);
   tempText.Format("Version %s", lpsMyAppVersion);
   dc.TextOut(LEFTMARGIN, 230, tempText);

   //smaller white text
   dc.SetTextColor(RGB(255,255,255));
   dc.SelectObject(m_pFont);
   tempText = "MOD_SIM is an unsupported utility, created";
   dc.TextOut(LEFTMARGIN,305, tempText);
   tempText = "for testing communications drivers for Adroit(tm).";
   dc.TextOut(LEFTMARGIN,325, tempText);
   tempText = "This application may be freely distributed without re-";
   dc.TextOut(LEFTMARGIN,345, tempText);
   tempText = "compilation as long as this notice is not removed.";
   dc.TextOut(LEFTMARGIN,365, tempText);
   tempText = "Copyright subsists with Adroit Technologies(r). www.adroit.co.za";
   dc.TextOut(LEFTMARGIN,385, tempText);
   tempText.Format("Comments, mail the author : %s", lpAuthor_email); //conradb@adroit.co.za";
   dc.TextOut(LEFTMARGIN,425, tempText);
   
   //registration code
   dc.SetTextColor(RGB(175,5,5));
   if (pGlobalDialog->m_registration.IsRegistered())
      tempText.Format("Registered user: %s   Key: %s", pGlobalDialog->m_registeredUser, pGlobalDialog->m_registeredKey);
   else
      tempText.Format("Unregistered user");
   dc.TextOut(LEFTMARGIN,275, tempText);


   //////////////////////////////////////////////////////////
   dc.SelectObject(pOldFont);
	dcImage.SelectObject(pOldBitmap);
}

// ------------------------------- OnTimer -----------------------------
// count down timer messages untill we want to hide the splash screen
void CSplashWnd::OnTimer(UINT nIDEvent)
{
   if (0 == m_splashShowCount)
   {
   	// Destroy the splash screen window.
	   HideSplashScreen();
   }
   else
      m_splashShowCount--;
}
