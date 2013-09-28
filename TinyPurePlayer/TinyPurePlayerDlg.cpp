
// TinyPurePlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TinyPurePlayer.h"
#include "TinyPurePlayerDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <AFXCOM_.H>

#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTinyPurePlayerDlg 对话框



CTinyPurePlayerDlg::CTinyPurePlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTinyPurePlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	currentPlayId = -1;
}

void CTinyPurePlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_MUSIC_LIST, musicList);

}

BEGIN_MESSAGE_MAP(CTinyPurePlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_BN_CLICKED(IDC_FOMER_BUT, &CTinyPurePlayerDlg::OnBnClickedFomerBut)
	ON_BN_CLICKED(IDC_NEXT_BUT, &CTinyPurePlayerDlg::OnBnClickedNextBut)
	ON_BN_CLICKED(IDC_RUN_BUT, &CTinyPurePlayerDlg::OnBnClickedRunBut)
	ON_BN_CLICKED(IDC_STOP_BUT, &CTinyPurePlayerDlg::OnBnClickedStopBut)
	ON_BN_CLICKED(IDC_CHANGE_BUT, &CTinyPurePlayerDlg::OnBnClickedChangeBut)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MUSIC_LIST, &CTinyPurePlayerDlg::OnLvnItemchangedMusicList)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CTinyPurePlayerDlg::OnItemdblclickMusicList)
	ON_NOTIFY(NM_DBLCLK, IDC_MUSIC_LIST, &CTinyPurePlayerDlg::OnDblclkMusicList)
END_MESSAGE_MAP()


// CTinyPurePlayerDlg 消息处理程序

BOOL CTinyPurePlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	LONG lStyle = GetWindowLong(musicList.m_hWnd, GWL_STYLE);    //获取当前窗口style
    lStyle    &= ~LVS_TYPEMASK;    //清除显示方式位       
    lStyle    |= LVS_REPORT;        //设置style       
    SetWindowLong(musicList.m_hWnd, GWL_STYLE, lStyle);            //设置style      
    
    DWORD dwStyle    = musicList.GetExtendedStyle();       
    dwStyle    |= LVS_EX_FULLROWSELECT;    //选中某行使整行高亮（只适用与report风格的listctrl）       
    dwStyle    |= LVS_EX_GRIDLINES;        //网格线（只适用与report风格的listctrl）       
      
    musicList.SetExtendedStyle(dwStyle);    //设置扩展风格 

	
	musicList.InsertColumn(0,_T("歌曲名"),LVCFMT_LEFT,100);
	musicList.InsertColumn(1,_T("歌手"),LVCFMT_LEFT,50);
	musicList.InsertColumn(2,_T("专辑"),LVCFMT_LEFT,50);
	

	musicNameList = new CArray<CString,CString&>;

	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("G:\\music\\*.mp3"));
	while (bWorking)
	{
	bWorking = finder.FindNextFile();

	if (finder.IsDots())
		continue;
	static int i = 0;
	if (!finder.IsDirectory())
	{		
		CString str = finder.GetFileName();	
		musicNameList->Add(finder.GetFilePath());
		musicList.InsertItem(i,str);    //插入行
		musicList.SetItemText(i,1,_T("lzp"));    //设置该行的不同列的显示字符
		musicList.SetItemText(i,2,_T("范特西"));	                   
	}
	}	

	finder.Close();


	//PlayASong(_T("G:\\music\\Alison Krauss - Nothing At All.mp3"),_T(""));


	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTinyPurePlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTinyPurePlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTinyPurePlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CTinyPurePlayerDlg::OnBnClickedFomerBut()
{
	
	if(currentPlayIndex>=0&& currentPlayIndex<musicNameList->GetSize()){		
	}else{
		currentPlayIndex = musicNameList->GetSize() - 1;
	}
	CString name = musicNameList->GetAt(++currentPlayIndex);
	
	PlayASong(name,_T(""));
	
	// TODO: 在此添加控件通知处理程序代码
}


void CTinyPurePlayerDlg::OnBnClickedNextBut()
{
	
	if(currentPlayIndex>=0&& currentPlayIndex<musicNameList->GetSize()){		
	}else{
		currentPlayIndex = 0;
	}
	CString name = musicNameList->GetAt(--currentPlayIndex);
	
	PlayASong(name,_T(""));
	
	// TODO: 在此添加控件通知处理程序代码
}


void CTinyPurePlayerDlg::OnBnClickedRunBut()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTinyPurePlayerDlg::OnBnClickedStopBut()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTinyPurePlayerDlg::OnBnClickedChangeBut()
{
	TCHAR szPath[MAX_PATH]; 
    CString str;
	ZeroMemory(szPath, sizeof(szPath));
    BROWSEINFO bi;   
    bi.hwndOwner = m_hWnd;   
    bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
    bi.lpszTitle = _T("请选择音乐目录：");   
    bi.ulFlags = 0;   
    bi.lpfn = NULL;   
    bi.lParam = 0;   
    bi.iImage = 0;   
    //弹出选择目录对话框
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
    {
		
		CFileFind finder;		
		str.Format(_T("%s"),szPath);
		
		BOOL bWorking = finder.FindFile(str+_T("\\*.mp3"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			continue;
			static int i = 0;
			if (!finder.IsDirectory())
		{		
			CString str = finder.GetFileName();	
			musicNameList->Add(finder.GetFilePath());
			musicList.InsertItem(i,str);    //插入行
			musicList.SetItemText(i,1,_T("lzp"));    //设置该行的不同列的显示字符
			musicList.SetItemText(i,2,_T("范特西"));	                   
		}
		}	

		finder.Close();

    }
    else   
		MessageBox(_T("无效的目录，请重新选择"));   
}




void CTinyPurePlayerDlg::OnLvnItemchangedMusicList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;
}

void CTinyPurePlayerDlg::PlayASong(CString songName,CString act){
		
	MCI_OPEN_PARMS mciOpen;
	MCIERROR mciError;
	MCI_PLAY_PARMS mciPlay;
	mciSendCommand(currentPlayId,MCI_CLOSE,0 ,(DWORD)&mciPlay);
	mciOpen.lpstrDeviceType = _T("mpegvideo");
	mciOpen.lpstrElementName = songName;
	mciError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,(DWORD)&mciOpen);
	if(mciError){
		MessageBox(_T("打开文件失败"));
		return;
	}
	UINT DeviceID = mciOpen.wDeviceID;
	currentPlayId = DeviceID;
	
	mciError = mciSendCommand(DeviceID,MCI_PLAY,0 ,(DWORD)&mciPlay);
	
	if(mciError){
		MessageBox(_T("播放失败！"));
		return;
	}

}

void CTinyPurePlayerDlg::OnItemdblclickMusicList(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);	
		
	*pResult = 0;
}


void CTinyPurePlayerDlg::OnDblclkMusicList(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int index  = pNMItemActivate->iItem;
	CString name = musicNameList->GetAt(musicNameList->GetSize()- index -1);	
	PlayASong(name,_T(""));
	currentPlayIndex =  musicNameList->GetSize()- index -1;
	*pResult = 0;
}
