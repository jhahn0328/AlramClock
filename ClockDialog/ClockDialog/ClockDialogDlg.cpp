// ClockDialogDlg.cpp : 구현 파일
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


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
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


// CClockDialogDlg 대화 상자



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

// CClockDialogDlg 메시지 처리기

BOOL CClockDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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

		AfxMessageBox(L"DB 파일을 열지 못했습니다.");

		return 0;

	}

	strSQL = "CREATE TABLE ALARM_LIST (\SEQ         INTEGER      PRIMARY KEY AUTOINCREMENT,\APM    VARCHAR(20),\TIME    VARCHAR(20),\DAY    VARCHAR(50)\); "; // MySQL 자료형으로 지정함, SQLite 자료형 확인하기.



	nResult = sqlite3_exec(pDB, strSQL, NULL, NULL, &pErr);

	if (nResult)

	{
		//AfxMessageBox(L"테이블이 이미 있습니다");
	}
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CClockDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CClockDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//메뉴 선택시 알람 추가 창 팝업
void CClockDialogDlg::OnAddalarm()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OutputDebugString(L"OnAddalarm");
	CDlgAddAlarm dlg;
	dlg.DoModal();
}

//윈도우 가변크기 설정
void CClockDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	return;
}
void CClockDialogDlg::TraySetting() {
	OutputDebugString(L"TraySetting");
	NOTIFYICONDATA  nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd; // 메인 윈도우 핸들
	nid.uID = IDR_MAINFRAME;  // 아이콘 리소스 ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // 플래그 설정
	nid.uCallbackMessage = WM_TRAYICON_MSG; // 콜백메시지 설정
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 아이콘 로드   

	lstrcpy(nid.szTip, _T("Test Application"));
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);
	m_bTrayStatus = true;
}
LRESULT CClockDialogDlg::TrayIconMessage(WPARAM wParam, LPARAM lParam) {		// 트레이 아이콘에서 마우스 이벤트 발생시 생기는 아무튼 그런거임
	OutputDebugString(L"TrayIconMessage");
	if (lParam == WM_LBUTTONDBLCLK) {
		NOTIFYICONDATA  nid;
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hWnd; // 메인 윈도우 핸들
		nid.uID = IDR_MAINFRAME;
		// 작업 표시줄(TaskBar)의 상태 영역에 아이콘을 삭제한다.
		Shell_NotifyIcon(NIM_DELETE, &nid);
		KillTimer(TRAY_BALLON);
		ShowWindow(SW_SHOW);
	}
	return 0L;
}
void CClockDialogDlg::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//예외처리
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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

	//컨트롤 크기 계산
	st_date.MoveWindow(right / 10, bottom / 10, right / 3, bottom / 6);//날짜
	st_day.MoveWindow(right / 2, bottom / 10, right / 3, bottom / 6);//요일
	st_time.MoveWindow(0, bottom / 3, right, bottom / 2);//시간
	bt_stop.MoveWindow(0, (bottom / 1.2), right, bottom / 6);//버튼
}

void CClockDialogDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	//윈도우 사이즈 최소크기 지정
	EASYSIZE_MINSIZE(448, 322, fwSide, pRect);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CClockDialogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	OutputDebugString(L"OnDestroy");
	p1->SuspendThread();
	p1->Delete();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}
