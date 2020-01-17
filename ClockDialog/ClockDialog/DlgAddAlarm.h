#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CDlgAddAlarm 대화 상자입니다.

static CListCtrl m_list_alrm;

class CDlgAddAlarm : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddAlarm)

public:
	CDlgAddAlarm(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAddAlarm();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADDALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox cmbox_apm;
	CComboBox cmbox_hour;
	CComboBox cmbox_minute;
	afx_msg void OnBnClickedOk();
	CButton m_chk1;
	CButton m_chk2;
	CButton m_chk3;
	CButton m_chk4;
	CButton m_chk5;
	CButton m_chk6;
	CButton m_chk7;
	
public:
	char* strSQL;
	char* pErr, *pDBFile = "MA_ALARM.db";
	int nResult;
	sqlite3* pDB;
	afx_msg void OnBnClickedButton1();
	CListCtrl m_list_ctl;
};

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	USES_CONVERSION;
	int i;
	int num = 0;
	CString seq;
	CString apm;
	CString time;
	CString day;
	for (i = 0; i<argc; i++)
	{
		//TRACE("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		seq.Format(L"%s", A2W(argv[i] ? argv[i] : "NULL"));
		i++;
		apm.Format(L"%s", A2W(argv[i] ? argv[i] : "NULL"));
		i++;
		time.Format(L"%s", A2W(argv[i] ? argv[i] : "NULL"));
		i++;
		day.Format(L"%s", A2W(argv[i] ? argv[i] : "NULL"));

		m_list_alrm.InsertItem(num, seq);// insert number
		m_list_alrm.SetItem(num, 1, LVIF_TEXT, apm, 0, 0, 0, NULL);
		m_list_alrm.SetItem(num, 2, LVIF_TEXT, time, 0, 0, 0, NULL);
		m_list_alrm.SetItem(num, 3, LVIF_TEXT, day, 0, 0, 0, NULL);
		num++;
	}
	printf("\n");
	return 0;
}