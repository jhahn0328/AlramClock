
// DlgAddAlarm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ClockDialog.h"
#include "DlgAddAlarm.h"
#include "afxdialogex.h"

// CDlgAddAlarm ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgAddAlarm, CDialogEx)

CDlgAddAlarm::CDlgAddAlarm(CWnd* pParent /*=NULL*/)
: CDialogEx(CDlgAddAlarm::IDD, pParent)
{

}

CDlgAddAlarm::~CDlgAddAlarm()
{

}

void CDlgAddAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_APM, cmbox_apm);
	DDX_Control(pDX, IDC_HOUR, cmbox_hour);
	DDX_Control(pDX, IDC_MINUTE, cmbox_minute);
	DDX_Control(pDX, IDC_CHECK1, m_chk1);
	DDX_Control(pDX, IDC_CHECK2, m_chk2);
	DDX_Control(pDX, IDC_CHECK3, m_chk3);
	DDX_Control(pDX, IDC_CHECK4, m_chk4);
	DDX_Control(pDX, IDC_CHECK5, m_chk5);
	DDX_Control(pDX, IDC_CHECK6, m_chk6);
	DDX_Control(pDX, IDC_CHECK7, m_chk7);
	DDX_Control(pDX, IDC_LIST2, m_list_alrm);
	//DDX_Control(pDX, IDC_LIST2, m_list_ctl);
}


BEGIN_MESSAGE_MAP(CDlgAddAlarm, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAddAlarm::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgAddAlarm::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgAddAlarm �޽��� ó�����Դϴ�.
BOOL CDlgAddAlarm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//CComboBox *cmb_dev = (CComboBox*)GetDlgItem(IDC_COMBO1);
	USES_CONVERSION;
	m_list_alrm.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list_alrm.InsertColumn(0, TEXT("NO"), LVCFMT_LEFT, 50);
	m_list_alrm.InsertColumn(1, TEXT("APM"), LVCFMT_LEFT, 60);
	m_list_alrm.InsertColumn(2, TEXT("TIME"), LVCFMT_LEFT, 65);
	m_list_alrm.InsertColumn(3, TEXT("DAY"), LVCFMT_LEFT, 110);


	CComboBox *cmb_apm, *cmb_hour, *cmb_min;
	cmb_apm = (CComboBox *)GetDlgItem(IDC_APM);
	cmb_hour = (CComboBox *)GetDlgItem(IDC_HOUR);
	cmb_min = (CComboBox *)GetDlgItem(IDC_MINUTE);

	CTime cTime = CTime::GetCurrentTime();
	CString str_init_minu;
	CString str_init_hour;
	int temp = cTime.GetHour();
	if (temp < 13)
	{
		cmb_apm->SetWindowTextW(L"AM");
		str_init_hour.Format(L"%d", cTime.GetHour());
		cmb_hour->SetWindowTextW(str_init_hour);
	}
	else
	{
		cmb_apm->SetWindowTextW(L"PM");
		str_init_hour.Format(L"%d", cTime.GetHour() - 12);
		cmb_hour->SetWindowTextW(str_init_hour);
	}
	str_init_minu.Format(L"%d", cTime.GetMinute() + 1);
	cmb_min->SetWindowTextW(str_init_minu);

	cmb_apm->AddString(L"AM");
	cmb_apm->AddString(L"PM");
	CString str_temp_value;
	for (int i = 0; i < 13; i++)
	{
		str_temp_value.Format(L"%d", i);
		cmb_hour->AddString(str_temp_value);
	}

	for (int i = 0; i < 60; i++)
	{
		str_temp_value.Format(L"%d", i);
		cmb_min->AddString(str_temp_value);
	}

	nResult = sqlite3_open(pDBFile, &pDB);

	if (nResult)

	{
		AfxMessageBox(L"DB ������ ���� ���߽��ϴ�.");
		return 0;
	}

	m_list_alrm.DeleteAllItems();
	nResult = sqlite3_exec(pDB, "select * from ALARM_LIST", callback, 0, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlgAddAlarm::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//�˶��� �����ϴ� ����� �ʿ�� ��
	USES_CONVERSION;
	CString str_apm;
	CString str_hour;
	CString str_minute;
	int n_hour, n_minute;
	int n_num_item = 0;

	GetDlgItemTextW(IDC_APM, str_apm);
	GetDlgItemTextW(IDC_HOUR, str_hour);
	GetDlgItemTextW(IDC_MINUTE, str_minute);

	//�ð� ����ó�� �κ�
	n_hour = _ttoi(str_hour);
	//n_hour = atoi(W2A(str_hout)); //���� ó�� ����
	n_minute = _ttoi(str_minute);

	if (str_apm != "AM" && str_apm != "PM")
	{
		AfxMessageBox(L"�޺� �ڽ� ���� AM,PM�� �������ּ���");
		return;
	}
	if (!isdigit(n_hour) && n_hour < 1 || n_hour > 12)
	{
		AfxMessageBox(L"�޺� �ڽ� ���� 1~12�� �������ּ���");
		return;
	}
	if (!isdigit(n_minute) && n_minute < 0 || n_minute > 59)
	{
		AfxMessageBox(L"�޺� �ڽ� ���� 0~59�� �������ּ���");
		return;
	}


	//�߰������� üũ�ڽ� �����Ͽ� �����ؾ���
	bool chk1, chk2, chk3, chk4, chk5, chk6, chk7;
	char chk[8];
	int n_temp = 0;
	memset(chk, 0x00, sizeof(chk));

	chk1 = m_chk1.GetCheck();
	chk2 = m_chk2.GetCheck();
	chk3 = m_chk3.GetCheck();
	chk4 = m_chk4.GetCheck();
	chk5 = m_chk5.GetCheck();
	chk6 = m_chk6.GetCheck();
	chk7 = m_chk7.GetCheck();

	CString str_for_days;
	char c_days_temp[20];
	memset(c_days_temp, 0x00, sizeof(c_days_temp));
	if (chk1 == TRUE)
		strcat(c_days_temp, "��");
	if (chk2 == TRUE)
		strcat(c_days_temp, "ȭ");
	if (chk3 == TRUE)
		strcat(c_days_temp, "��");
	if (chk4 == TRUE)
		strcat(c_days_temp, "��");
	if (chk5 == TRUE)
		strcat(c_days_temp, "��");
	if (chk6 == TRUE)
		strcat(c_days_temp, "��");
	if (chk7 == TRUE)
		strcat(c_days_temp, "��");

	if (strcmp(c_days_temp, "") == 0)
	{
		AfxMessageBox(L"������ �������ּ���");
		return;
	}

	n_num_item = m_list_alrm.GetItemCount();

	CString str_m_apm;
	CString str_m_hour;
	CString str_m_minute;
	CString str_m_days;
	CString str_time;

	for (int i = 0; i < n_num_item; i++)
	{
		str_m_apm = m_list_alrm.GetItemText(i, 1);
		str_m_hour = m_list_alrm.GetItemText(i, 2);
		str_time.Format(L"%02s : %02s", str_hour, str_minute);
		str_m_days = m_list_alrm.GetItemText(i, 3);
		if (strcmp(W2A(str_apm), W2A(str_m_apm)) == 0 
			&& strcmp(W2A(str_m_hour), W2A(str_time)) == 0
			&& strcmp(c_days_temp, W2A(str_m_days)) == 0)
		{
			AfxMessageBox(L"�ð� / ���� �ߺ��Դϴ�.");
			return;
		}
	}


	//����ȭ
	str_for_days.Format(L"%s", A2W(c_days_temp));
	CString str_for_apm, str_for_time;

	str_for_apm.Format(_T("%s"), str_apm);
	str_for_time.Format(_T("%02s : %02s"), str_hour, str_minute);

	char strSQL1[100];
	memset(strSQL1, 0x00, sizeof(strSQL1));
	CString str_for_days2;
	str_for_days2.Format(L"INSERT INTO ALARM_LIST (APM, TIME,DAY) VALUES ( '%s', '%s', '%s')", str_for_apm, str_for_time, str_for_days);
	nResult = sqlite3_exec(pDB, W2A(str_for_days2), NULL, NULL, &pErr);
	if (nResult)
	{
		AfxMessageBox(L"�߰�����");
	}
	m_list_alrm.DeleteAllItems();
	nResult = sqlite3_exec(pDB, "select * from ALARM_LIST", callback, 0, NULL);
}


void CDlgAddAlarm::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	nResult = sqlite3_open(pDBFile, &pDB);

	if (nResult)
	{
		AfxMessageBox(L"DB ������ ���� ���߽��ϴ�.");
		return;
	}

	POSITION pos = NULL;
	int nItemIdx = 0;
	CString seq;

	pos = m_list_alrm.GetFirstSelectedItemPosition();

	if (NULL != pos)
	{
		nItemIdx = m_list_alrm.GetNextSelectedItem(pos);
	}
	seq = m_list_alrm.GetItemText(nItemIdx, 0);
	char strSQL1[1000];
	memset(strSQL1, 0x00, sizeof(strSQL1));
	sprintf(strSQL1, "DELETE FROM ALARM_LIST WHERE SEQ = %S; ", seq);
	nResult = sqlite3_exec(pDB, strSQL1, NULL, NULL, &pErr);
	if (nResult)
	{
		AfxMessageBox(L"���� ����!!");
	}
	m_list_alrm.DeleteAllItems();
	nResult = sqlite3_exec(pDB, "select * from ALARM_LIST", callback, 0, NULL);
}
