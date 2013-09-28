
// TinyPurePlayerDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTinyPurePlayerDlg �Ի���



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


// CTinyPurePlayerDlg ��Ϣ�������

BOOL CTinyPurePlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	LONG lStyle = GetWindowLong(musicList.m_hWnd, GWL_STYLE);    //��ȡ��ǰ����style
    lStyle    &= ~LVS_TYPEMASK;    //�����ʾ��ʽλ       
    lStyle    |= LVS_REPORT;        //����style       
    SetWindowLong(musicList.m_hWnd, GWL_STYLE, lStyle);            //����style      
    
    DWORD dwStyle    = musicList.GetExtendedStyle();       
    dwStyle    |= LVS_EX_FULLROWSELECT;    //ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��       
    dwStyle    |= LVS_EX_GRIDLINES;        //�����ߣ�ֻ������report����listctrl��       
      
    musicList.SetExtendedStyle(dwStyle);    //������չ��� 

	
	musicList.InsertColumn(0,_T("������"),LVCFMT_LEFT,100);
	musicList.InsertColumn(1,_T("����"),LVCFMT_LEFT,50);
	musicList.InsertColumn(2,_T("ר��"),LVCFMT_LEFT,50);
	

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
		musicList.InsertItem(i,str);    //������
		musicList.SetItemText(i,1,_T("lzp"));    //���ø��еĲ�ͬ�е���ʾ�ַ�
		musicList.SetItemText(i,2,_T("������"));	                   
	}
	}	

	finder.Close();


	//PlayASong(_T("G:\\music\\Alison Krauss - Nothing At All.mp3"),_T(""));


	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTinyPurePlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CTinyPurePlayerDlg::OnBnClickedNextBut()
{
	
	if(currentPlayIndex>=0&& currentPlayIndex<musicNameList->GetSize()){		
	}else{
		currentPlayIndex = 0;
	}
	CString name = musicNameList->GetAt(--currentPlayIndex);
	
	PlayASong(name,_T(""));
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CTinyPurePlayerDlg::OnBnClickedRunBut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CTinyPurePlayerDlg::OnBnClickedStopBut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
    bi.lpszTitle = _T("��ѡ������Ŀ¼��");   
    bi.ulFlags = 0;   
    bi.lpfn = NULL;   
    bi.lParam = 0;   
    bi.iImage = 0;   
    //����ѡ��Ŀ¼�Ի���
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
			musicList.InsertItem(i,str);    //������
			musicList.SetItemText(i,1,_T("lzp"));    //���ø��еĲ�ͬ�е���ʾ�ַ�
			musicList.SetItemText(i,2,_T("������"));	                   
		}
		}	

		finder.Close();

    }
    else   
		MessageBox(_T("��Ч��Ŀ¼��������ѡ��"));   
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
		MessageBox(_T("���ļ�ʧ��"));
		return;
	}
	UINT DeviceID = mciOpen.wDeviceID;
	currentPlayId = DeviceID;
	
	mciError = mciSendCommand(DeviceID,MCI_PLAY,0 ,(DWORD)&mciPlay);
	
	if(mciError){
		MessageBox(_T("����ʧ�ܣ�"));
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
