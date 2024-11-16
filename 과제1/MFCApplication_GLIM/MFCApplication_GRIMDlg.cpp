// MFCApplication_GRIMDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication_GRIM.h"
#include "MFCApplication_GRIMDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <windows.h>
#include <winuser.h>
#include <gdiplusgraphics.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
/* 요구사항
1. 이미지를 표시할 수 있는 다이얼로그를 생성합니다. .. 완료
2. 시작좌표(x1, y1), 종료좌표(x2, y2)를 입력 받을 수 있는 UI를 생성합니다. .. 완료
3. Draw 버튼 클릭시(x1, y1) 좌표를 중심으로하는 랜덤한 크기의 원을 그립니다. 완료
4. Action 버튼 클릭시(x1, y1) 좌표에서(x2, y2) 좌표로 일정 픽셀 간격으로 원을 이동시키며, 
이동할 때마다 실행파일이 위치한 폴더 내의 image 폴더에 
bmp 또는 jpg(jpeg) 포맷으로 저장합니다.
   완료
5. Open 버튼 클릭시 탐색기 창을 통해 이미지를 1개를 선택해서 불러오고 다이얼로그에 출력합니다.
이 때 출력된 원의 중심 좌표에 X 모양을 그리고 좌표값을 같이 표시합니다.
*/

CPoint m_ptStart, m_ptPrev, point, m_ptOldStart;
double dOldRandom;
bool m_bFirst = true;
// m_bLButtonDown = false;
COLORREF m_PenColor = RGB(0, 0, 0); // Black
COLORREF m_PenXorColor = RGB(255, 255, 255); // White

COLORREF m_BrushColor = RGB(255, 255, 255); // White
COLORREF m_BrushXorColor = RGB(0, 0, 0); // Black
/*
COLORREF m_BrushColor = RGB(0, 0, 0); // White
COLORREF m_BrushXorColor = RGB(255, 255, 255); // Black
*/
COLORREF m_PenColorRed = RGB(255, 0, 0); // Black

CString GetProgramPath()
{
	CString strPath;
	TCHAR szPath[1024];
	GetModuleFileName(NULL, szPath, 1024);
	strPath.Format(_T("%s"), szPath);

	return strPath;
}

CString GetProgramDir()
{
	CString strPath;
	TCHAR szPath[1024];
	GetModuleFileName(NULL, szPath, 1024);
	strPath.Format(_T("%s"), szPath);

	// 경로명의 맨 뒤에 '\'가 붙음
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);

	return strPath;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
	CRect m_image_rect; // Picture Control 의 위치 
	CImage m_image; // Picture Control 에 표시할 이미지 파일
	CRect rect2;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};



CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplicationGRIMDlg 대화 상자



CMFCApplicationGRIMDlg::CMFCApplicationGRIMDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION_GRIM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationGRIMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Ed_x1);
	DDX_Control(pDX, IDC_EDIT3, m_Ed_y1);
	DDX_Control(pDX, IDC_EDIT2, m_Ed_x2);
	DDX_Control(pDX, IDC_EDIT4, m_Ed_y2);
	DDX_Control(pDX, IDC_ED_RAD, m_EdRad);
	DDX_Control(pDX, IDC_EDIT5, m_CenX);
	DDX_Control(pDX, IDC_EDIT6, m_CenY);
}

BEGIN_MESSAGE_MAP(CMFCApplicationGRIMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Btn_DRaw, &CMFCApplicationGRIMDlg::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDC_Btn_Action, &CMFCApplicationGRIMDlg::OnBnClickedBtnAction)
	ON_BN_CLICKED(IDC_Btn_Open, &CMFCApplicationGRIMDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(ID_Btn_OK, &CMFCApplicationGRIMDlg::OnBnClickedBtnOk)	
	ON_EN_CHANGE(IDC_ED_Y2, &CMFCApplicationGRIMDlg::OnEnChangeEdY2)	
END_MESSAGE_MAP()

void CMFCApplicationGRIMDlg::OnCircleDraw(bool Mode)
{
	CClientDC dc(this);
	CPen pen, * oldpen;
	pen.CreatePen(PS_SOLID, 7, m_PenXorColor); //PEN 객체 생성
	CBrush brush, * oldbrush;
	brush.CreateSolidBrush(m_BrushXorColor); //BRUSH 객체 생성
	dc.SetROP2(R2_XORPEN);      //XOR펜으로 설정
	oldpen = dc.SelectObject(&pen);    //펜 객체, 아래는 브러시 객체 등록
	oldbrush = dc.SelectObject(&brush);

	HBRUSH hBrush;
	//hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	hBrush = CreateSolidBrush(m_BrushXorColor);

	double dRandom = 10;
	double dRandom2 = 10;
	//현재 원을 그림

	m_ptOldStart = m_ptStart;

	

	CRect rect2;
	//이전 원을 지움	
	if (!m_bFirst) {
		//Ellipse(dc, m_ptOldStart.x, m_ptOldStart.y, m_ptPrev.x, m_ptPrev.y);
		//FillEllipse(&hBrush, Ellipse);
		//graphics.FillEllipse(&blackBrush, ellipseRect);

		//dc.Ellipse(m_ptOldStart.x, m_ptOldStart.y, m_ptPrev.x, m_ptPrev.y);				
		dc.Ellipse(m_ptOldStart.x, m_ptOldStart.y, m_ptPrev.x, m_ptPrev.y);
		TRACE("delete circle %d %d %d %d %f \r\n", m_ptOldStart.x, m_ptOldStart.y, m_ptPrev.x, m_ptPrev.y, dRandom);
		//GetDlgItem(IDC_STATIC)->GetWindowRect(&rect2);
		//dc.Rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
		
	}
	else
		m_bFirst = false;
	
	if (Mode) {
		dRandom = rand() % 250;
		dRandom2 = rand() % 250;
		if (dRandom > 250) dRandom = 250;
		if (dRandom2 > 250) dRandom2 = 250;		
		if (dRandom < 10) dRandom = 10;
		if (dRandom2 < 10) dRandom2 = 10;
		m_ptStart.x = dRandom;
		m_ptStart.y = dRandom2;
		point.x = m_ptStart.x + dOldRandom;
		point.y = m_ptStart.y + dOldRandom;
	}
	else
	{
		dRandom = rand() % 300;
		if (dRandom > 300) dRandom = 300;
		if (dRandom < 10) dRandom = 10;

		m_ptStart.x = GetDlgItemInt(IDC_ED_X1);
		m_ptStart.y = GetDlgItemInt(IDC_ED_Y1);
		point.x = m_ptStart.x + dRandom;
		point.y = m_ptStart.y + dRandom;
		dOldRandom = dRandom;
	}
	

	
	dc.Ellipse(m_ptStart.x, m_ptStart.y, point.x, point.y);
	TRACE("draw circle %d %d %d %d %f \r\n", m_ptStart.x, m_ptStart.y, point.x, point.y, dRandom);
	m_ptPrev = point; //이전점에 현재점을 저장

	dc.SelectObject(oldpen);  //이전 PEN으로 설정
	dc.SelectObject(oldbrush);  //이전 BRUSH로 설정
	pen.DeleteObject();
	brush.DeleteObject();
}

void CMFCApplicationGRIMDlg::OnCrossLineDraw(CPoint Center)
{
	CClientDC dc(this);
	CPen pen, * oldpen;
	//pen.CreatePen(PS_SOLID, 7, m_PenXorColor); //PEN 객체 생성
	// m_PenColorRed
	//pen.CreatePen(PS_SOLID, 7, m_PenXorColor); //PEN 객체 생성
	pen.CreatePen(PS_SOLID, 1, m_PenColorRed); //PEN 객체 생성
	CBrush brush, * oldbrush;
	//brush.CreateSolidBrush(m_BrushXorColor); //BRUSH 객체 생성
	brush.CreateSolidBrush(m_PenColorRed); //BRUSH 객체 생성
	//dc.SetROP2(R2_XORPEN);      //XOR펜으로 설정
	dc.SetROP2(R2_COPYPEN);      //COPYPEN펜으로 설정
	oldpen = dc.SelectObject(&pen);    //펜 객체, 아래는 브러시 객체 등록
	oldbrush = dc.SelectObject(&brush);
	
	//현재 CrossLine을 그림 
	CPoint verticalstart, verticalend, horizontalstart, horizontalend;
	verticalstart.x = Center.x-8;
	verticalend.x = Center.x + 8;
	verticalstart.y = Center.y -1;
	verticalend.y = Center.y + 1;

	//dc.MoveTo(verticalstart);
	//dc.LineTo(verticalend);
	dc.Rectangle(verticalstart.x, verticalstart.y, verticalend.x, verticalend.y);
	horizontalstart.y = Center.y - 5;
	horizontalend.y = Center.y + 5;
	horizontalstart.x = Center.x-1;
	horizontalend.x = Center.x+1;

	//dc.MoveTo(horizontalstart);
	//dc.LineTo(horizontalend);
	dc.Rectangle(horizontalstart.x, horizontalstart.y, horizontalend.x, horizontalend.y);

	TRACE("CrossLinen Horizontal %d %d %d %d \r\n", horizontalstart.x, horizontalstart.y, horizontalend.x, horizontalend.y);
	TRACE("CrossLinen Vertical %d %d %d %d \r\n", verticalstart.x, verticalstart.y, verticalend.x, verticalend.y);
	
	dc.SelectObject(oldpen);  //이전 PEN으로 설정
	dc.SelectObject(oldbrush);  //이전 BRUSH로 설정
	pen.DeleteObject();
	brush.DeleteObject();
}

void CMFCApplicationGRIMDlg::OnCircleDelete()
{
	CClientDC dc(this);
	CPen pen, * oldpen;
	pen.CreatePen(PS_SOLID, 7, m_PenXorColor); //PEN 객체 생성
	CBrush brush, * oldbrush;
	brush.CreateSolidBrush(m_BrushXorColor); //BRUSH 객체 생성
	dc.SetROP2(R2_XORPEN);      //XOR펜으로 설정
	oldpen = dc.SelectObject(&pen);    //펜 객체, 아래는 브러시 객체 등록
	oldbrush = dc.SelectObject(&brush);


	double dRandom = rand() % 300;
	double dRandom2 = rand() % 300;

	//현재 원을 그림
	if (dRandom > 300) dRandom = 300;
	if (dRandom2 > 300) dRandom2 = 300;
	if (dRandom < 10) dRandom = 10;
	if (dRandom2 < 10) dRandom2 = 10;

	m_ptOldStart = m_ptStart;
	m_ptStart.x = dRandom;
	m_ptStart.y = dRandom;
	point.x = m_ptStart.x + dOldRandom;
	point.y = m_ptStart.y + dOldRandom;
	

	CRect rect2;
	//이전 원을 지움	
	if (!m_bFirst) {
		dc.Ellipse(m_ptOldStart.x, m_ptOldStart.y, m_ptPrev.x, m_ptPrev.y);
		TRACE("delete circle %d %d %d %d %f \r\n", m_ptOldStart.x, m_ptOldStart.y, m_ptPrev.x, m_ptPrev.y, dRandom);
		//GetDlgItem(IDC_STATIC)->GetWindowRect(&rect2);
		//dc.Rectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);

	}
	else
		m_bFirst = false;

	


	//dc.Ellipse(m_ptStart.x, m_ptStart.y, point.x, point.y);
	//TRACE("draw circle %d %d %d %d %f \r\n", m_ptStart.x, m_ptStart.y, point.x, point.y, dRandom);
	m_ptPrev = point; //이전점에 현재점을 저장

	dc.SelectObject(oldpen);  //이전 PEN으로 설정
	dc.SelectObject(oldbrush);  //이전 BRUSH로 설정
	pen.DeleteObject();
	brush.DeleteObject();
}


// CMFCApplicationGRIMDlg 메시지 처리기

BOOL CMFCApplicationGRIMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();	
	
#if 1
	_bstr_t gg(GetProgramDir() + "");
	BSTR lpszFileName = gg.copy();
	cv::Mat src = cv::imread("c:\\image\\GRIM20241115_000006.bmp", cv::IMREAD_UNCHANGED);
#endif
	m_ptStart =  m_ptPrev =  point =  m_ptOldStart = 0;
	dOldRandom = 0;
	SetDlgItemInt(IDC_ED_X1, 200);
	SetDlgItemInt(IDC_ED_Y1, 200);
	SetDlgItemInt(IDC_ED_X2, 200);
	SetDlgItemInt(IDC_ED_Y2, 200);
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplicationGRIMDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplicationGRIMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplicationGRIMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplicationGRIMDlg::OnBnClickedBtnDraw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// OnCircleDelete();
	OnCircleDraw(false);	
	//OnPaint();
}


void CMFCApplicationGRIMDlg::OnBnClickedBtnAction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.		
	OnCircleDraw(true);
	

	CDC* pDC = GetDlgItem(IDC_STATIC)->GetDC();
	HDC hDC = pDC->m_hDC;

	// picture control 크기 
	RECT rc;
	GetDlgItem(IDC_STATIC)->GetClientRect(&rc);

	// bitmap 셍성
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rc.right,rc.bottom);
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, rc.right, rc.bottom, hDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hBmpOld);
	DeleteDC(hMemDC);

	// 비트맵 사양 설정
	BITMAPINFOHEADER bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = rc.right;
	bmih.biHeight = rc.bottom;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;

	// Bitmap(DIB) 데이터 추출
	GetDIBits(hDC, hBitmap, 0, rc.bottom, NULL, (LPBITMAPINFO)&bmih, 1);
	LPBYTE lpBits = new BYTE[bmih.biSizeImage];
	GetDIBits(hDC, hBitmap, 0, rc.bottom, lpBits, (LPBITMAPINFO)&bmih, 1);
	ReleaseDC(pDC);
	DeleteObject(hBitmap);

	//bitmap file header setting
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 'MB';
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmih.biSizeImage;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// file name setting
	SYSTEMTIME time;
	GetLocalTime(&time);
	
	CString Name;
	Name.Format(_T("\\image\\GRIM%04d%02d%02d_%02d%02d%02d.bmp"),
		time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute, time.wSecond);
	_bstr_t gg(GetProgramDir()+Name);
	BSTR lpszFileName = gg.copy();

	// bitmap file create and data save
	DWORD dwWritten;
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE,0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(hFile, &bmih, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	WriteFile(hFile, lpBits, bmih.biSizeImage, &dwWritten, NULL);

	CloseHandle(hFile);
	delete[] lpBits;
	
	//CString strCString = lpszFileName;
	
	std::string fname;
	String f_name;
	CString strName = lpszFileName;
	CT2CA convertedString(strName);
	std::string str = std::string(convertedString);	
	cv::Mat src = cv::imread(str, cv::IMREAD_UNCHANGED);
	//cv::imshow("color", src);
	TRACE("File Name : %s", strName);
//	OnCircleDelete();
}


void CMFCApplicationGRIMDlg::OnBnClickedBtnOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		// picture control 크기 
		CRect rect;
		CWnd* pPictureControl = GetDlgItem(IDC_STATIC);
		pPictureControl->GetWindowRect(&rect);
		ScreenToClient(&rect);

		

		CString pathName = dlg.GetPathName();
		
		CImage image;
		image.Load(pathName);
		HBITMAP h_bmp = (HBITMAP)image;
		CWnd* p_wnd = GetDlgItem(IDC_STATIC);
		CStatic* p_static = (CStatic*)p_wnd;
		p_static->SetBitmap(h_bmp);
		//MessageBox(pathName);

		std::string fname;
		//String f_name;
		//CString strName = lpszFileName;
		CT2CA convertedString(pathName);
		std::string str = std::string(convertedString);
		cv::Mat src = cv::imread(str, cv::IMREAD_UNCHANGED);
		//cv::imshow("color", src);
		TRACE("File Name : %s", pathName);

		if (src.empty()) return;

		cv::Mat src_gray;
		cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

		cv::Mat blurred;
		cv::blur(src_gray, blurred, cv::Size(7, 7));

		std::vector<cv::Vec3f> circles;

		//cv::HoughCircles(blurred, circles, cv::HOUGH_GRADIENT, 1, 30, 150, 60);
		cv::HoughCircles(blurred, circles, cv::HOUGH_GRADIENT, 1, 7, 150, 60);
		for (const auto& c : circles) {
			cv::Point center(cvRound(c[0]), cvRound(c[1]));
			int radius = cvRound(c[2]);
			cv::circle(src, center, 2, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
			cv::circle(src, center, radius, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
			//SetDlgItemTextA("IDC_EDIT5", center.x);
			//SetDlgItemTextA("IDC_EDIT6", center.x);
			
			//string tempString  = itoa(center.x);
			//LPCSTR lpcString  = itoa(center.x);
			CString tmpCstr = _T("1");
			tmpCstr.Format(_T("%d"), center.x);
			m_CenX.SetWindowTextW(tmpCstr);

			tmpCstr.Format(_T("%d"), center.y);
			m_CenY.SetWindowTextW(tmpCstr);
			//m_CenY.SetWindowTextW(to_string(center.y).c_str());
			TRACE("Center of Circle %d %d  \r\n", center.x, center.y);
			CPoint Center = 0;
			Center.x = center.x + rect.left;
			Center.y = center.y + rect.top;

			OnCrossLineDraw(Center);
		}

	}
}


void CMFCApplicationGRIMDlg::OnBnClickedBtnOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->CloseWindow();
	this->DestroyWindow();	
}




void CMFCApplicationGRIMDlg::OnEnChangeEdY2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이a 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}




