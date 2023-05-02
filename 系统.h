#pragma once
#include "afxdialogex.h"


// 系统 对话框

class 系统 : public CDialogEx
{
	DECLARE_DYNAMIC(系统)

public:
	系统(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~系统();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
