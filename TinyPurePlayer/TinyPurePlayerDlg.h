
// TinyPurePlayerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"




// CTinyPurePlayerDlg 对话框
class CTinyPurePlayerDlg : public CDialogEx
{
// 构造
public:
	CTinyPurePlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TINYPUREPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFomerBut();
	afx_msg void OnBnClickedNextBut();
	afx_msg void OnBnClickedRunBut();
	afx_msg void OnBnClickedStopBut();
	afx_msg void OnBnClickedChangeBut();
public:	
	afx_msg void OnLvnItemchangedMusicList(NMHDR *pNMHDR, LRESULT *pResult);
	void PlayASong(CString songName,CString act);
public:
	CListCtrl musicList;
	CArray<CString,CString&>* musicNameList;
	UINT  currentPlayId;
	int currentPlayIndex;

	afx_msg void OnItemdblclickMusicList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkMusicList(NMHDR *pNMHDR, LRESULT *pResult);
};
