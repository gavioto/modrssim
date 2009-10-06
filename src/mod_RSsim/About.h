#if !defined(AFX_ABOUT_H__E2C72AC3_9FBF_4BB0_A2C7_0A07706C57A8__INCLUDED_)
#define AFX_ABOUT_H__E2C72AC3_9FBF_4BB0_A2C7_0A07706C57A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// About.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
{
// Construction
public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

   CStaticLink m_hyperlink1;  // adroit
   CStaticLink m_hyperlink2;  // modicon
   CStaticLink m_emaillink1;

	CCreditStatic m_static;

   CString m_registeredUser, m_registeredKey;
   BOOL    m_registeredOK;

   SetCurrentTopic  m_topic;  //help system

// Implementation
protected:

   // TOOLTIPS
   CToolTipCtrl    m_ToolTip;
   BOOL     OnTTN_NeedText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSplash();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUT_H__E2C72AC3_9FBF_4BB0_A2C7_0A07706C57A8__INCLUDED_)
