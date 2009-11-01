// About.cpp : implementation file
//

#include "stdafx.h"
#include "mod_rssim.h"
#include "Creditstatic.h"
#include "About.h"
#include "StarWarsCtrl.h"
#include "EasterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog
char *pArrCreditPre = { "MOD_RSSIM Version %s\t||"
      "IDB_CREDITBMP1^|"
      "Simulator for MODBUS RTU, Ethernet,$|"
      "Allen-Bradley DF1 protocol (CRC and BCC),$|"
      "Slave and Master$|"
      "IDB_CREDITBMP3^|"
      "Hotkeys:\r|"
      "<CTRL>-S = Settings`|<CTRL>-N = Noise`|<CTRL>-E = Erase`|<CTRL>-L = Load`|<CTRL>-A = sAve`|"
      "<CTRL>-T = Transparency toggle`|<CTRL>-B = aBout`|<CTRL>-M = eMulations`|"
      "<CTRL>-C = Comms/values toggle`|<CTRL>-J = insert random RS232 error-char`||"
      "IDB_CREDITBMP4^|"
      "About\r|"
      "This cool protocol test program was written by|"
      "Conrad Braam.|"
      "For a PDF format document of the MODBUS protocol, $|"
      "open your browser and type www.modicon.com, go to $|"
      "the search page and type in\"PI-MBUS-300\" (no quotes)$|"
      ", and download the document.$||"
      "IDB_CREDITBMP2^|"
      "------------------------------------------------------------------|"
      "Allen Bradley is a trademark of Rockwell Corporation.|"
      "Modicon is a trademark of Gould Modicon USA.|"
      "JOY is a trademark of Joy Mining Machinery company UK.|"
      "IDB_CREDITBMP5^|"
      "|||||||"

};



CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
   m_topic.SetHelpTopic("More");
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
   ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnTTN_NeedText )
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_SPLASH, OnSplash)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
CString mailLink,versionStr;
CString reg;   //registration info
CString title;
	
   CDialog::OnInitDialog();

   
	// the WWW link is set up here
	m_hyperlink1.SubclassDlgItem(IDC_WWWLINK, this);
	m_hyperlink2.SubclassDlgItem(IDC_MODICONLINK, this);

   m_hyperlink1.SetHoverCursorHandle(AfxGetApp()->LoadCursor(IDC_MYHANDCURSOR));
   m_hyperlink2.SetHoverCursorHandle(AfxGetApp()->LoadCursor(IDC_MYHANDCURSOR));
   // set the URL that we will actually go to here
   m_hyperlink1.m_link.LoadString(IDS_ADROITWEBURL);// = "http://www.plcsimulator.org/";
   // set the URL that we will actually go to here
   m_hyperlink2.m_link = "http://www.modicon.com/";

	// the Email to link is set up here
   m_emaillink1.SubclassDlgItem(IDC_EMAILLINK, this);
   m_emaillink1.SetHoverCursorHandle(AfxGetApp()->LoadCursor(IDC_MYHANDCURSOR));
   m_emaillink1.SetWindowText(lpAuthor_email);

   // set the URL that we will actually go to here
   mailLink.Format("mailto:%s?subject=%s%s", lpAuthor_email, "mod_rssim", lpsMyAppVersion);  // append it
   m_emaillink1.m_link = mailLink;

   if (m_registeredOK)
      reg.Format("Registered user: %s   Key: %s", m_registeredUser, m_registeredKey);
   else
      reg.Format("Unregistered user");
   SetDlgItemText(IDC_REGISTRATIONINFO, reg);
   //TOOLTIPS START
   m_ToolTip.Create (this);
   m_ToolTip.Activate (TRUE);

   CWnd*    pWnd = GetWindow (GW_CHILD);
   while (pWnd)
   {
       int nID = pWnd->GetDlgCtrlID ();
       if (nID != -1)
       {
           m_ToolTip.AddTool (pWnd, pWnd->GetDlgCtrlID ());
       }
       pWnd = pWnd->GetWindow (GW_HWNDNEXT);
   }
   //TOOLTIPS END

   GetDlgItem(IDC_SINKSTAT2)->ModifyStyle(0, WS_CLIPSIBLINGS);

	m_static.SubclassDlgItem(IDC_CREDITSTATIC, this);
   CString pArrCredit;
   pArrCredit.Format(pArrCreditPre, lpsMyAppVersion);
	m_static.SetCredits(pArrCredit,'|');
	m_static.SetSpeed(DISPLAY_MEDIUM);
	m_static.SetColor(BACKGROUND_COLOR, GetSysColor(COLOR_3DFACE));//RGB(192,192,192));
	m_static.SetTransparent();
	m_static.StartScrolling();
   //window title-bar
   GetWindowText(title);
   title += " Version ";
   title += lpsMyAppVersion;
   SetWindowText(title);

   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION; OCX Property Pages should return FALSE
}

// ------------------------ OnTTN_NeedText ---------------------------------
// TTN_NEEDTEXT message handler for TOOLTIPS
//
BOOL CAboutDlg::OnTTN_NeedText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            pTTT->lpszText = MAKEINTRESOURCE(nID);
            pTTT->hinst = AfxGetResourceHandle();
            return(TRUE);
        }
    }
    return(FALSE);
} // OnTTN_NeedText

// -------------------------------- PreTranslateMessage -----------------------
BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg) 
{
    // TOOLTIPS START
    if (m_hWnd)
    {
        m_ToolTip.RelayEvent (pMsg);
        return CDialog::PreTranslateMessage(pMsg);
    }
    return (FALSE);
    // TOOLTIPS END
	
	return CDialog::PreTranslateMessage(pMsg);
}

// ------------------------------------- OnOK ---------------------------------
void CAboutDlg::OnOK() 
{
CEasterDlg dlg;
short state;

   if (m_registeredUser.GetLength())
   {
      dlg.m_userName = m_registeredUser;	
   }
   else
   {
      dlg.m_userName = "Unregistered user";
   }

   // show the easter egg if user holds down any Shift key while hitting this button
	state = GetKeyState(VK_LSHIFT) | GetKeyState(VK_RSHIFT);
   if (state <0)
      dlg.DoModal();
	
	CDialog::OnOK();
}

// ----------------------------------- OnSplash -------------------------------
// give the user time to view the splash screen
void CAboutDlg::OnSplash() 
{
	// splash kills itself after 7000 ms (7 seconds)
   CSplashWnd::EnableSplashScreen(TRUE);
   CSplashWnd::m_splashShowDelay = 7;
   CSplashWnd::ShowSplashScreen(this); 	
	
}
