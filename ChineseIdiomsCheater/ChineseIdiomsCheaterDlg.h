
// ChineseIdiomsCheaterDlg.h : header file
//

#pragma once

#include "sqlite3.h"

// CChineseIdiomsCheaterDlg dialog
class CChineseIdiomsCheaterDlg : public CDialogEx
{
// Construction
public:
	CChineseIdiomsCheaterDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHINESEIDIOMSCHEATER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	sqlite3 * p_sqlite3_handle;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	CString TargetString;
	afx_msg void OnClickedButtonSearch();
	CListBox WordList;
	afx_msg void OnDblclkListDataShow();
	afx_msg void OnClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIdok();
};
