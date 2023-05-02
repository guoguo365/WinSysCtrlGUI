#pragma warning(disable: 4996)

#include <tchar.h>
#include <Windows.h>

#include "resource.h"

#define _CRT_SECURE_NO_WARNINGS

/*
提权
*/
BOOL EnableShutDownPriv()
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp = { 0 };
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return FALSE;
	}
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}
/*
dwFlags:	操作类型
bForce:		是否强制
*/
BOOL ReSetWindows(DWORD dwFlags, BOOL bForce) {
	if (dwFlags != EWX_LOGOFF && dwFlags != EWX_REBOOT && dwFlags != EWX_SHUTDOWN) {
		return FALSE;
	}

	// 获取操作系统版本号
	OSVERSIONINFO osvi = { 0 };
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&osvi)) {
		return FALSE;
	}

	// 如果是版本号是NB， 则需要提权操作
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) {

		// 提权
		EnableShutDownPriv();
	}
	dwFlags |= (bForce != FALSE) ? EWX_FORCE : EWX_FORCEIFHUNG;
	return ExitWindowsEx(dwFlags, 0);
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
		// 初始化对话框
	case WM_INITDIALOG:
	{
		// 获取下拉框句柄
		HWND hCombo = GetDlgItem(hwndDlg, IDC_COMBO1);
		SendMessage(hCombo, CB_INSERTSTRING, 0, (LPARAM) L"注销");
		SendMessage(hCombo, CB_INSERTSTRING, 1, (LPARAM) L"重启");
		SendMessage(hCombo, CB_INSERTSTRING, 2, (LPARAM) L"关机");

		// 获取桌面尺寸
		int deskCx = GetSystemMetrics(SM_CXFULLSCREEN);
		int deskCy = GetSystemMetrics(SM_CYFULLSCREEN);

		// 获取对话框尺寸
		RECT dialogRect{};
		GetWindowRect(hwndDlg, &dialogRect);
		long width = dialogRect.right - dialogRect.left;
		long height = dialogRect.bottom - dialogRect.top;

		// 对话框剧中显示
		MoveWindow(hwndDlg, deskCx / 2 - width / 2, deskCy / 2 - height / 2, width, height, TRUE);
		
		break;
	}
	case WM_COMMAND: 
		if (wParam == IDOK) {
			TCHAR szText[20] = { 0 };
			GetDlgItemText(hwndDlg, IDC_COMBO1, szText, 20);

			if (_tcscmp(szText, _T("重启")) == 0) {
				ReSetWindows(EWX_REBOOT, FALSE);
			}
			else if (_tcscmp(szText, _T("关机")) == 0) {
				ReSetWindows(EWX_SHUTDOWN, FALSE);
			}
			else if (_tcscmp(szText, _T("注销")) == 0) {
				ReSetWindows(EWX_LOGOFF, FALSE);
			}
			//MessageBox(hwndDlg, L"OK按钮按下", L"提示", MB_OK);
		}
		else if (wParam == IDCANCEL) {
			int iRet = MessageBox(hwndDlg, L"是否真的关闭窗口？", L"提示", MB_OKCANCEL);
			if (iRet == IDOK) {
				EndDialog(hwndDlg, IDCANCEL);
			}
			
		}
		break;
	default:
		break;
	}
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	if (_tcscmp(lpCmdLine, _T("-r")) == 0) {
		ReSetWindows(EWX_REBOOT, FALSE);
	}
	else if (_tcscmp(lpCmdLine, _T("-s")) == 0) {
		ReSetWindows(EWX_SHUTDOWN, FALSE);
	}
	else if (_tcscmp(lpCmdLine, _T("-l")) == 0) {
		ReSetWindows(EWX_LOGOFF, FALSE);
	}
	else {
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), NULL, DialogProc);
	}

	return 0;
}