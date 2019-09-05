
// ChineseIdiomsCheaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChineseIdiomsCheater.h"
#include "ChineseIdiomsCheaterDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
#define BUFFER_SZIE 50
	CChineseIdiomsCheaterDlg* pDlg = (CChineseIdiomsCheaterDlg*)NotUsed;
	TCHAR p_buffer[BUFFER_SZIE];
	int i;
	int retsize;
	for (i = 0; i < argc; i++)
	{
		retsize=MultiByteToWideChar(CP_UTF8, 0, argv[i], strlen(argv[i]), p_buffer, BUFFER_SZIE);
		p_buffer[retsize] = _T('\0');
		//::MessageBoxW(NULL, p_buffer, NULL, 0);
		pDlg->WordList.AddString(p_buffer);
	}
	return 0;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChineseIdiomsCheaterDlg dialog



CChineseIdiomsCheaterDlg::CChineseIdiomsCheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHINESEIDIOMSCHEATER_DIALOG, pParent)
	, TargetString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	p_sqlite3_handle = NULL;
}

void CChineseIdiomsCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, TargetString);
	DDX_Control(pDX, IDC_LIST_DATA_SHOW, WordList);
}

BEGIN_MESSAGE_MAP(CChineseIdiomsCheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CChineseIdiomsCheaterDlg::OnClickedButtonSearch)
	ON_LBN_DBLCLK(IDC_LIST_DATA_SHOW, &CChineseIdiomsCheaterDlg::OnDblclkListDataShow)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CChineseIdiomsCheaterDlg::OnClickSyslink1)
	ON_COMMAND(IDOK, &CChineseIdiomsCheaterDlg::OnIdok)
END_MESSAGE_MAP()


// CChineseIdiomsCheaterDlg message handlers

BOOL CChineseIdiomsCheaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// sqlite3 db init
	if (PathFileExistsA(".\\Idioms.db"))
	{
		if (SQLITE_OK != sqlite3_open(".\\Idioms.db", &p_sqlite3_handle))
		{
			MessageBox(_T("加载成语数据库出错"));
		}
	}
	else
	{
		MessageBox(_T("成语数据库不存在"));
	}
	GotoDlgCtrl(GetDlgItem(IDC_EDIT_INPUT));
	return TRUE;
}

void CChineseIdiomsCheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChineseIdiomsCheaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChineseIdiomsCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CChineseIdiomsCheaterDlg::DestroyWindow()
{
	sqlite3_close(p_sqlite3_handle);
	return CDialogEx::DestroyWindow();
}


void CChineseIdiomsCheaterDlg::OnClickedButtonSearch()
{
	int sqlutf8size;
	char* sqlite_errmsg = NULL;
	char* p_buffer = NULL;
	CString SQL;
	CString LastChar;
	UpdateData(TRUE);
	TargetString.Trim();
	if (TargetString.IsEmpty())
	{
		return;
	}
	WordList.ResetContent();
	LastChar = TargetString[TargetString.GetLength() - 1];
	//MessageBox(LastChar);
	SQL += _T("SELECT Word FROM IdiomsData WHERE FirstChar='");
	SQL += LastChar;
	SQL+=_T("';");
	//MessageBox(SQL);
	p_buffer =(char*) malloc(SQL.GetLength() * 2);
	sqlutf8size=WideCharToMultiByte(CP_UTF8, 0, SQL, SQL.GetLength(), p_buffer, SQL.GetLength() * 2, NULL, NULL);
	p_buffer[sqlutf8size] = '\0';
	if (SQLITE_OK != sqlite3_exec(p_sqlite3_handle, p_buffer, callback, this, &sqlite_errmsg))
	{
		//::MessageBoxA(m_hWnd, sqlite_errmsg, NULL, 0);
		sqlite3_free(sqlite_errmsg);
	}
	free(p_buffer);
}


void CChineseIdiomsCheaterDlg::OnDblclkListDataShow()
{
	int cur_sel = WordList.GetCurSel();
	if (cur_sel == -1)
	{
		return;
	}
	else
	{
		WordList.GetText(cur_sel, TargetString);
	}
	UpdateData(FALSE);
}


void CChineseIdiomsCheaterDlg::OnClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	::ShellExecute(NULL, _T("open"), _T("https://github.com/qwerty258/ChineseIdiomsCheater"), NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}

void CChineseIdiomsCheaterDlg::OnIdok()
{
	// TODO: Add your command handler code here
}
