
// ClockDialogDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

#pragma comment(lib, "winmm")
#include <mmsystem.h>

#define WM_TRAYICON_MSG WM_USER + 1
#define WM_TRAY_NOTIFYCATION WM_APP+10
#define TRAY_BALLON 0

static CString str_apm;
static CString str_time;
static CString str_day;
static volatile bool isAlarmChecked = FALSE;
static int callback2(void *NotUsed, int argc, char **argv, char **azColName);

// CClockDialogDlg ��ȭ ����
class CClockDialogDlg : public CDialogEx
{
// �����Դϴ�.
	DECLARE_EASYSIZE
public:
	CClockDialogDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CLOCKDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	bool m_bTrayStatus;
	LRESULT TrayIconMessage(WPARAM wParam, LPARAM lParam);
public:
	CString mDay;
	CString mTime;
	CWinThread *p2 = NULL;
	CWinThread *p1 = NULL;

	char* strSQL;
	char* pErr, *pDBFile = "MA_ALARM.db";
	int nResult;
	sqlite3* pDB;
	BOOL isAlarm;
public:
	void TraySetting();
	void Timecompare(){
		nResult = sqlite3_open(pDBFile, &pDB);

		if (nResult)
		{
			AfxMessageBox(L"DB ������ ���� ���߽��ϴ�.");
			return;
		}

		nResult = sqlite3_exec(pDB, "select * from ALARM_LIST", callback2, 0, NULL);
		if (isAlarmChecked == TRUE){
			AlarmStart();
		}
	}
	void AlarmStart(){
		HWND h_awnd = ::GetForegroundWindow();
		DWORD thr_id = GetWindowThreadProcessId(h_awnd, NULL);
		DWORD thr_curid = GetCurrentThreadId();
		if (thr_curid != thr_id){
			if (AttachThreadInput(thr_curid, thr_id, TRUE)){
				BringWindowToTop();
				AttachThreadInput(thr_curid, thr_id, FALSE);
			}
		}
		p2 = AfxBeginThread(SoundThread, this);
		if (p2 == NULL)
		{
			p2->SuspendThread();
			AfxMessageBox(L"Error");
		}
	}
	void AlarmStop(){
		if (p2 != NULL){
			PlaySound(NULL, 0, 0);
			isAlarmChecked = FALSE;
			p2->SuspendThread();
			//delete p2;
			p2->Delete();
			p2 = NULL;
		}
		else {
			return;
		}
	}

	void Delay(DWORD dwSec){
		MSG msg;
		DWORD dwStart = GetTickCount();

		while (GetTickCount() - dwStart < dwSec){
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	static UINT TimeThread(LPVOID _mothod)
	{
		CClockDialogDlg *fir = (CClockDialogDlg*)_mothod;
		
		while (1)//
		{
			CTime cTime = CTime::GetCurrentTime();
			int nWeek;
			int hour = 0;
			if (cTime.GetHour() > 12)
			{
				hour = cTime.GetHour() - 12;
				str_apm.Format(L"PM");
			}
			else
			{
				hour = cTime.GetHour();
				str_apm.Format(L"AM");
			}
			
			fir->mDay.Format(L"%04d.%02d.%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());
			fir->mTime.Format(L"%s %02d:%02d:%02d", str_apm, hour, cTime.GetMinute(), cTime.GetSecond());
			str_time.Format(L"%02d : %02d", hour, cTime.GetMinute());
			nWeek=cTime.GetDayOfWeek();
			switch (nWeek){
				case 1:
					fir->SetDlgItemText(IDC_WEEKSTATIC, L"�Ͽ���");
					str_day.Format(L"��");
					break;
				case 2:
					fir->SetDlgItemText(IDC_WEEKSTATIC, L"������");
					str_day.Format(L"��");
					break;
				case 3:
					fir->SetDlgItemText(IDC_WEEKSTATIC, L"ȭ����");
					str_day.Format(L"ȭ");
					break;
				case 4:
					fir->SetDlgItemText(IDC_WEEKSTATIC, L"������");
					str_day.Format(L"��");
					break;
				case 5:
					fir->SetDlgItemText(IDC_WEEKSTATIC, L"�����");
					str_day.Format(L"��");
					break;
				case 6:
					fir->SetDlgItemText(IDC_WEEKSTATIC, L"�ݿ���");
					str_day.Format(L"��");
					break;
				case 7:
					fir->SetDlgItemText(IDC_WEEKSTATIC, L"�����");
					str_day.Format(L"��");
					break;
			
			}
			fir->SetDlgItemText(IDC_DaySTATIC, fir->mDay);
			fir->SetDlgItemText(IDC_TimeSTATIC, fir->mTime);
			if (cTime.GetSecond()==0){
				fir->Timecompare();
				Sleep(1000);
			}
			fir->Delay(250);
		}
		
		return 0;
	}


	static UINT SoundThread(LPVOID sndvoid)
	{
		CClockDialogDlg *sThread = (CClockDialogDlg*)sndvoid;
		clock_t wait;
		int counter = 0;

		//�߰��� �˶� ������ �ȴ��ȴٸ� �� ���� �ݺ��ϰԲ�
		if (sThread->isAlarm = TRUE){
			for (int i = 0; i < 3; i++){
				PlaySound(L"c:\sound1.wav", NULL, SND_ASYNC | SND_LOOP);

				wait = clock() + (1 * CLOCKS_PER_SEC);
				while (clock() < wait){ ; }
				sThread->Delay(100);

				if (i == 2){
					PlaySound(NULL, 0, 0);

					sThread->Delay(1000);
					counter++;
					i = 0;
				}
				if (counter == 5){
					PlaySound(NULL, 0, 0);
					sThread->isAlarm = FALSE;
					break;
				}
			}
		}
		sThread->AlarmStop();
		return 0;
	}
	afx_msg void OnAddalarm();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedSound();
	afx_msg void OnBnClickedStop();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic st_date;
	CStatic st_day;
	CStatic st_time;
	afx_msg void OnDestroy();
	CButton bt_stop;
};

static int callback2(void *NotUsed, int argc, char **argv, char **azColName)
{
	USES_CONVERSION;
	int i=0;
	CString seq;
	CString apm;
	CString time;
	CString day;
	int result = 0;
	//for (i = 0; i < argc; i++)
	//{
		seq.Format(L"%s", A2W(argv[i] ? argv[i] : "NULL"));
		i++;
		apm.Format(L"%s", A2W(argv[i] ? argv[i] : "NULL"));
		i++;
		time.Format(L"%s", A2W(argv[i] ? argv[i] : "NULL"));
		i++;
		day.Format(L"%s", A2W(argv[i] ? argv[i] : "NULL"));

		char c_day[50];
		char c_day2[10];
		memset(c_day, 0x00, sizeof(c_day));
		memset(c_day2, 0x00, sizeof(c_day2));
		memcpy(c_day, day, sizeof(c_day));
		memcpy(c_day2, str_day, sizeof(c_day2));


		if (apm.Compare(str_apm) == 0 && time.Compare(str_time) == 0 && strstr(c_day, c_day2) != NULL){
			//Ʈ���� �˸� + �Ҹ� �˸�
			isAlarmChecked = TRUE;
			NOTIFYICONDATA nid;
			ZeroMemory(&nid, sizeof(nid));
			nid.uID = 0;
			nid.cbSize = sizeof(nid);

			nid.uFlags = NIF_MESSAGE | NIF_INFO | NIF_ICON;
			nid.hWnd = AfxGetApp()->m_pMainWnd->m_hWnd; // ���� ������ �ڵ�
			nid.uTimeout = 3000;
			nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // ������ �ε�

			nid.uCallbackMessage = WM_TRAY_NOTIFYCATION;
			nid.dwInfoFlags = NIIF_USER | NIIF_LARGE_ICON;        // ǳ�� �˸������� ����

			lstrcpy(nid.szInfo, _T("������ �˶� �ð��� �Ǿ����ϴ�."));
			lstrcpy(nid.szInfoTitle, _T("�˶��ð�"));
			Shell_NotifyIcon(NIM_ADD, &nid);
			Shell_NotifyIcon(NIM_DELETE, &nid);

		}
		return 0;
	//}
}