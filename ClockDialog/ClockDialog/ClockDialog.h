
// ClockDialog.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CClockDialogApp:
// �� Ŭ������ ������ ���ؼ��� ClockDialog.cpp�� �����Ͻʽÿ�.
//

class CClockDialogApp : public CWinApp
{
public:
	CClockDialogApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CClockDialogApp theApp;