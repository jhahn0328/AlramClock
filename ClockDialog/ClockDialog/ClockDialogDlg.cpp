// ClockDialogDlg.cpp : ���� ����
//
#include <crtdbg.h>
#include "stdafx.h"
#include "ClockDialog.h"
#include "ClockDialogDlg.h"
#include "afxdialogex.h"
#include "DlgAddAlarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClockDialogDlg ��ȭ ����



CClockDialogDlg::CClockDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClockDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClockDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DaySTATIC, st_date);
	DDX_Control(pDX, IDC_WEEKSTATIC, st_day);
	DDX_Control(pDX, IDC_TimeSTATIC, st_time);
	DDX_Control(pDX, IDC_STOP, bt_stop);
}

BEGIN_MESSAGE_MAP(CClockDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_TRAYICON_MSG, TrayIconMessage)
	ON_COMMAND(ID_ADDALARM, &CClockDialogDlg::OnAddalarm)
	ON_WM_SIZE()
	//ON_WM_GETMINMAXINFO()
	//ON_BN_CLICKED(IDC_SOUND, &CClockDialogDlg::OnBnClickedSound)
	ON_BN_CLICKED(IDC_STOP, &CClockDialogDlg::OnBnClickedStop)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZING()
	ON_WM_DESTROY()
//	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CClockDialogDlg)
	EASYSIZE(IDC_DaySTATIC, ES_BORDER, ES_KEEPSIZE, IDC_WEEKSTATIC, ES_BORDER, 0)
	EASYSIZE(IDC_WEEKSTATIC, ES_BORDER, ES_KEEPSIZE, ES_BORDER, ES_BORDER, 0)
	EASYSIZE(IDC_TimeSTATIC, ES_BORDER, ES_KEEPSIZE, ES_BORDER, ES_BORDER, 0)
END_EASYSIZE_MAP

// CClockDialogDlg �޽��� ó����

BOOL CClockDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	INIT_EASYSIZE;

	m_bTrayStatus = FALSE;

	SetBackgroundColor(RGB(255, 255, 255));

	CFont fnt;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 80;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, (LPCWSTR)"Tahoma");
	fnt.CreateFontIndirect(&lf);
	GetDlgItem(IDC_TimeSTATIC)->SetFont(&fnt);
	fnt.Detach();

	CFont fnt1;
	LOGFONT lf1;
	::ZeroMemory(&lf1, sizeof(lf1));
	lf1.lfHeight = 30;
	lf1.lfWeight = FW_BOLD;
	::lstrcpy(lf1.lfFaceName, (LPCWSTR)"Tahoma");
	fnt1.CreateFontIndirect(&lf1);
	GetDlgItem(IDC_DaySTATIC)->SetFont(&fnt1);
	GetDlgItem(IDC_WEEKSTATIC)->SetFont(&fnt1);
	fnt1.Detach();
	
	p1 = AfxBeginThread(TimeThread, this);
	if (p1 == NULL)
	{
		AfxMessageBox(L"Error");
	}


	nResult = sqlite3_open(pDBFile, &pDB);

	if (nResult)

	{

		AfxMessageBox(L"DB ������ ���� ���߽��ϴ�.");

		return 0;

	}

	strSQL = "CREATE TABLE ALARM_LIST (\SEQ         INTEGER      PRIMARY KEY AUTOINCREMENT,\APM    VARCHAR(20),\TIME    VARCHAR(20),\DAY    VARCHAR(50)\); "; // MySQL �ڷ������� ������, SQLite �ڷ��� Ȯ���ϱ�.



	nResult = sqlite3_exec(pDB, strSQL, NULL, NULL, &pErr);

	if (nResult)

	{
		//AfxMessageBox(L"���̺��� �̹� �ֽ��ϴ�");
	}
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CClockDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
		TraySetting();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CClockDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CClockDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//�޴� ���ý� �˶� �߰� â �˾�
void CClockDialogDlg::OnAddalarm()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OutputDebugString(L"OnAddalarm");
	CDlgAddAlarm dlg;
	dlg.DoModal();
}

//������ ����ũ�� ����
void CClockDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	return;
}
void CClockDialogDlg::TraySetting() {
	OutputDebugString(L"TraySetting");
	NOTIFYICONDATA  nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd; // ���� ������ �ڵ�
	nid.uID = IDR_MAINFRAME;  // ������ ���ҽ� ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // �÷��� ����
	nid.uCallbackMessage = WM_TRAYICON_MSG; // �ݹ�޽��� ����
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // ������ �ε�   

	lstrcpy(nid.szTip, _T("Test Application"));
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);
	m_bTrayStatus = true;
}
LRESULT CClockDialogDlg::TrayIconMessage(WPARAM wParam, LPARAM lParam) {		// Ʈ���� �����ܿ��� ���콺 �̺�Ʈ �߻��� ����� �ƹ�ư �׷�����
	OutputDebugString(L"TrayIconMessage");
	if (lParam == WM_LBUTTONDBLCLK) {
		NOTIFYICONDATA  nid;
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hWnd; // ���� ������ �ڵ�
		nid.uID = IDR_MAINFRAME;
		// �۾� ǥ����(TaskBar)�� ���� ������ �������� �����Ѵ�.
		Shell_NotifyIcon(NIM_DELETE, &nid);
		KillTimer(TRAY_BALLON);
		ShowWindow(SW_SHOW);
	}
	return 0L;
}
void CClockDialogDlg::OnBnClickedStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//����ó��
	OutputDebugString(L"OnBnClickedStop");
	if (p2 != NULL){
		PlaySound(NULL, 0, 0);
		isAlarmChecked = FALSE;
		p2->SuspendThread();
		//delete p2;
		p2->Delete();
		p2 = NULL;
	}
	return;
}

void CClockDialogDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CFont fnt;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));

	HWND Handle = this->m_hWnd;
	HWND hwndBox = ::GetDlgItem(this->m_hWnd, IDD_CLOCKDIALOG_DIALOG);
	RECT rc2;
	::GetClientRect(Handle, &rc2);

	int bottom = 0;
	int right = 0;
	bottom = rc2.bottom;
	right = rc2.right;

	lf.lfHeight = bottom / 2;
	lf.lfWidth = right/15;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, (LPCWSTR)"Tahoma");
	fnt.CreateFontIndirect(&lf);
	GetDlgItem(IDC_TimeSTATIC)->SetFont(&fnt);
	fnt.Detach();

	CFont fnt1;
	LOGFONT lf1;
	::ZeroMemory(&lf1, sizeof(lf1));
	lf1.lfHeight = bottom / 6;
	lf1.lfWidth = right / 30;
	lf1.lfWeight = FW_BOLD;
	::lstrcpy(lf1.lfFaceName, (LPCWSTR)"Tahoma");
	fnt1.CreateFontIndirect(&lf1);
	GetDlgItem(IDC_DaySTATIC)->SetFont(&fnt1);
	GetDlgItem(IDC_WEEKSTATIC)->SetFont(&fnt1);
	fnt1.Detach();

	//��Ʈ�� ũ�� ���
	st_date.MoveWindow(right / 10, bottom / 10, right / 3, bottom / 6);//��¥
	st_day.MoveWindow(right / 2, bottom / 10, right / 3, bottom / 6);//����
	st_time.MoveWindow(0, bottom / 3, right, bottom / 2);//�ð�
	bt_stop.MoveWindow(0, (bottom / 1.2), right, bottom / 6);//��ư
}

void CClockDialogDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	//������ ������ �ּ�ũ�� ����
	EASYSIZE_MINSIZE(448, 322, fwSide, pRect);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CClockDialogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	OutputDebugString(L"OnDestroy");
	p1->SuspendThread();
	p1->Delete();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
}
