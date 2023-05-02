// 系统.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "系统.h"


// 系统 对话框

IMPLEMENT_DYNAMIC(系统, CDialogEx)

系统::系统(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{

}

系统::~系统()
{
}

void 系统::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(系统, CDialogEx)
END_MESSAGE_MAP()


// 系统 消息处理程序
