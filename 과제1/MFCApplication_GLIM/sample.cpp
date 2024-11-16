/*
// RectrictZoneView.cpp : implementation of the CRectrictZoneView class
//
#include "stdafx.h"
#include "RectrictZone.h"
#include "RectrictZoneDoc.h"
#include "RectrictZoneView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CRectrictZoneView
IMPLEMENT_DYNCREATE(CRectrictZoneView, CFormView)
BEGIN_MESSAGE_MAP(CRectrictZoneView, CFormView)
    //{{AFX_MSG_MAP(CRectrictZoneView)
    ON_BN_CLICKED(IDC_ADD, OnAdd)
    ON_BN_CLICKED(IDC_CLR, OnClr)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_COMMAND(ID_ELLIPSE, OnEllipse)
    ON_COMMAND(ID_RECTANGLE, OnRectangle)
    ON_COMMAND(ID_POLY_RUBBER, OnPolyRubber)
    ON_COMMAND(ID_POLY_NOTRUBBER, OnPolyNotrubber)
    ON_UPDATE_COMMAND_UI(ID_ELLIPSE, OnUpdateEllipse)
    ON_UPDATE_COMMAND_UI(ID_RECTANGLE, OnUpdateRectangle)
    ON_UPDATE_COMMAND_UI(ID_POLY_RUBBER, OnUpdatePolyRubber)
    ON_UPDATE_COMMAND_UI(ID_POLY_NOTRUBBER, OnUpdatePolyNotrubber)
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CRectrictZoneView construction/destruction
CRectrictZoneView::CRectrictZoneView()
    : CFormView(CRectrictZoneView::IDD)
{
    m_bFirst = true;
    m_bLButtonDown = false;
    m_PenColor = RGB(0, 0, 0);
    m_PenXorColor = RGB(255, 255, 255);
    m_BrushColor = RGB(255, 255, 255);
    m_BrushXorColor = RGB(0, 0, 0);
    m_nCount = 0;
    for (int i = 0; i < 50; i++) {
        m_ptData[i] = 0;
    }
}
CRectrictZoneView::~CRectrictZoneView()
{
}
void CRectrictZoneView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
}
BOOL CRectrictZoneView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CFormView::PreCreateWindow(cs);
}
void CRectrictZoneView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    GetParentFrame()->RecalcLayout();
    ResizeParentToFit();
}
/////////////////////////////////////////////////////////////////////////////
// CRectrictZoneView printing
BOOL CRectrictZoneView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}
void CRectrictZoneView::OnBeginPrinting(CDC*, CPrintInfo*)
{
    // TODO: add extra initialization before printing
}
void CRectrictZoneView::OnEndPrinting(CDC* pDC, CPrintInfo*)
{
}
void CRectrictZoneView::OnPrint(CDC* pDC, CPrintInfo*)
{
    CRectrictZoneDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    CPen pen, * oldpen;
    pen.CreatePen(PS_SOLID, 1, m_PenColor);
    CBrush brush, * oldbrush;
    brush.CreateSolidBrush(m_BrushColor);
    pDC->SetROP2(R2_COPYPEN);
    oldpen = pDC->SelectObject(&pen);
    oldbrush = pDC->SelectObject(&brush);
    switch (m_nDrawMode)
    {
    case LINE_MODE:
        pDC->MoveTo(m_ptStart);
        pDC->LineTo(m_ptPrev);
        break;
    case ELLIPSE_MODE:
        //마지막 원을 그림
        pDC->Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
        break;
    case RECTANGLE_MODE:
        pDC->Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
        break;
    case POLY_RUBBER_MODE:
    case POLY_NOTRUBBER_MODE:
        //배열에 저장된 순서대로 연결해 마지막 다각형을 그림
        pDC->Polygon(m_ptData, m_nCount);
        break;
    }
    pDC->SelectObject(oldpen);
    pDC->SelectObject(oldbrush);
    pen.DeleteObject();
    brush.DeleteObject();
}
/////////////////////////////////////////////////////////////////////////////
// CRectrictZoneView diagnostics
#ifdef _DEBUG
void CRectrictZoneView::AssertValid() const
{
    CFormView::AssertValid();
}
void CRectrictZoneView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
CRectrictZoneDoc* CRectrictZoneView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRectrictZoneDoc)));
    return (CRectrictZoneDoc*)m_pDocument;
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRectrictZoneView message handlers
void CRectrictZoneView::OnAdd()
{
    AfxMessageBox("경로 그리기를 시작합니다");
    m_nDrawMode = LINE_MODE;
}
void CRectrictZoneView::OnClr()
{
    AfxMessageBox("모든 제한 구역을 제거합니다");
}
void CRectrictZoneView::OnMouseMove(UINT nFlags, CPoint point)
{
    CClientDC dc(this);   //클라이언트 영역 DC를 얻음
    CPen pen, * oldpen;
    pen.CreatePen(PS_SOLID, 1, m_PenXorColor); //PEN 객체 생성
    CBrush brush, * oldbrush;
    brush.CreateSolidBrush(m_BrushXorColor); //BRUSH 객체 생성
    dc.SetROP2(R2_XORPEN);      //XOR펜으로 설정
    oldpen = dc.SelectObject(&pen);    //펜 객체, 아래는 브러시 객체 등록
    oldbrush = dc.SelectObject(&brush);
    switch (m_nDrawMode)
    {
    case LINE_MODE:
        if (m_bLButtonDown)
        {

        }
        break;

    case ELLIPSE_MODE:  //원 그리기
        if (m_bLButtonDown)
        {
            //이전 원을 지움
            dc.Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
            //현재 원을 그림
            dc.Ellipse(m_ptStart.x, m_ptStart.y, point.x, point.y);
            m_ptPrev = point; //이전점에 현재점을 저장
        }
        break;
    case RECTANGLE_MODE: //사각형 그리기
        if (m_bLButtonDown)
        {
            //이전 사각형을 지움
            dc.Rectangle(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
            //현재 원을 그림
            dc.Rectangle(m_ptStart.x, m_ptStart.y, point.x, point.y);
            m_ptPrev = point;
        }
        break;
    case POLY_RUBBER_MODE:
        if (!m_bFirst)
        {
            //시작점에서 이전점까지의 선을 지움
            dc.MoveTo(m_ptStart);
            dc.LineTo(m_ptPrev);
            //시작점에서 현재점까지 선을 그림
            dc.MoveTo(m_ptStart);
            dc.LineTo(point);
            m_ptPrev = point;
        }
        break;
    }
    dc.SelectObject(oldpen);  //이전 PEN으로 설정
    dc.SelectObject(oldbrush);  //이전 BRUSH로 설정
    pen.DeleteObject();
    brush.DeleteObject();

    //마우스 위치 좌표를 status bar에 출력한다
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CString strPoint;
    strPoint.Format("마우스 위치 x : %d, y : %d", point.x, point.y);
    pFrame->m_wndStatusBar.SetWindowText(strPoint);
    CFormView::OnMouseMove(nFlags, point);
}
void CRectrictZoneView::OnLButtonDown(UINT nFlags, CPoint point)
{
    //각 변수들 초기화 및 화면 초기화
    if (m_bFirst)
    {
        m_ptStart = m_ptPrev = 0;
        m_bLButtonDown = false;
        m_bFirst = true;
        m_nCount = 0;
        for (int i = 0; i < 50; i++)
            m_ptData[i] = 0;
        Invalidate(true); //화면 갱신 - true:처음부터 끝가지 새로
    }
    CClientDC dc(this);  //클라이언트 영역 DC 받음
    RECT rectClient;  //구조체 변수 선언
    CPen pen, * oldpen;
    pen.CreatePen(PS_SOLID, 1, m_PenColor);  //PEN 객체 생성
    dc.SetROP2(R2_COPYPEN);  //COPYPEN으로 설정
    oldpen = dc.SelectObject(&pen);
    switch (m_nDrawMode)
    {
    case LINE_MODE:  //직선 그리기
    case ELLIPSE_MODE: //원 그리기
    case RECTANGLE_MODE: //사각형 그리기
        m_ptStart = m_ptPrev = point;
        m_bLButtonDown = true;
        m_bFirst = false;
        break;
    case POLY_RUBBER_MODE: //다각형 그리기 러버밴드 이용
        if (m_bFirst)
        {
            m_bFirst = false;
        }
        //시작점과 이전점에 현재점을 저장
        m_ptStart = m_ptPrev = point;
        m_ptData[m_nCount] = point; //배열에 현재 점을 저장
        m_nCount++;   //카운트 증가
        break;
    case POLY_NOTRUBBER_MODE:  //다각형 그리기 러버밴드 이용 안함
        if (m_bFirst)
        {
            m_ptStart = point; //시작점에 현재점을 저장
            m_bFirst = false;
        }
        else
        {
            dc.MoveTo(m_ptStart);
            dc.LineTo(point);  //시작점과 현재 점을 그림
            m_ptStart = point;
        }
        m_ptData[m_nCount] = point;  //배열에 현재 점을 저장
        m_nCount++;  //카운트 증가
        break;
    }
    SetCapture(); //마우스 캡쳐
    GetClientRect(&rectClient);  //클라이언트 영역 얻음
    ClientToScreen(&rectClient); //스크린 좌표계로 변환
    ::ClipCursor(&rectClient);  //마우스 이동범위를 클라이언트 영역으로 제한
    dc.SelectObject(oldpen);  //이전 PEN으로 설정
    pen.DeleteObject();    //PEN객체 삭제

    CFormView::OnLButtonDown(nFlags, point);
}
void CRectrictZoneView::OnEllipse()
{
    m_nDrawMode = ELLIPSE_MODE;
}
void CRectrictZoneView::OnRectangle()
{
    m_nDrawMode = RECTANGLE_MODE;

}
void CRectrictZoneView::OnPolyRubber()
{
    m_nDrawMode = POLY_RUBBER_MODE;
}
void CRectrictZoneView::OnPolyNotrubber()
{
    m_nDrawMode = POLY_NOTRUBBER_MODE;

}
void CRectrictZoneView::OnUpdateEllipse(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_nDrawMode == ELLIPSE_MODE ? 1 : 0);

}
void CRectrictZoneView::OnUpdateRectangle(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_nDrawMode == RECTANGLE_MODE ? 1 : 0);
}
void CRectrictZoneView::OnUpdatePolyRubber(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_nDrawMode == POLY_RUBBER_MODE ? 1 : 0);
}
void CRectrictZoneView::OnUpdatePolyNotrubber(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_nDrawMode == POLY_NOTRUBBER_MODE ? 1 : 0);
}
void CRectrictZoneView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bLButtonDown) //왼쪽 버튼이 눌린 상태면
    {
        if (m_nDrawMode == LINE_MODE || m_nDrawMode == ELLIPSE_MODE || m_nDrawMode == RECTANGLE_MODE)
        {
            //체크 변수 초기화 => 다시 직선, 원, 사각형을 그리기 위해
            m_bLButtonDown = false;
            m_bFirst = true;
            ReleaseCapture();  //마우스 캡쳐, 클립 해제
            ::ClipCursor(NULL);
            Invalidate(true);
        }
        if (m_bFirst)
        {
            ReleaseCapture();
            ::ClipCursor(NULL);
        }
    }

    CFormView::OnLButtonUp(nFlags, point);
}
void CRectrictZoneView::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (m_nDrawMode == POLY_RUBBER_MODE || m_nDrawMode == POLY_NOTRUBBER_MODE)
    {
        if (!m_bFirst)
        {
            //배열에 현재점을 저장하고 카운트 증가
            m_ptData[m_nCount] = point;
            m_nCount++;
            //체크변수 초기화 => 다시 사각형을 그리기 위해
            m_bFirst = true;
            ReleaseCapture(); //마우스 캡쳐, 클립 해제
            ::ClipCursor(NULL);
            Invalidate(true);
        }
    }

    CFormView::OnRButtonDown(nFlags, point);
}
*/