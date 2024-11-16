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
        //������ ���� �׸�
        pDC->Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
        break;
    case RECTANGLE_MODE:
        pDC->Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
        break;
    case POLY_RUBBER_MODE:
    case POLY_NOTRUBBER_MODE:
        //�迭�� ����� ������� ������ ������ �ٰ����� �׸�
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
    AfxMessageBox("��� �׸��⸦ �����մϴ�");
    m_nDrawMode = LINE_MODE;
}
void CRectrictZoneView::OnClr()
{
    AfxMessageBox("��� ���� ������ �����մϴ�");
}
void CRectrictZoneView::OnMouseMove(UINT nFlags, CPoint point)
{
    CClientDC dc(this);   //Ŭ���̾�Ʈ ���� DC�� ����
    CPen pen, * oldpen;
    pen.CreatePen(PS_SOLID, 1, m_PenXorColor); //PEN ��ü ����
    CBrush brush, * oldbrush;
    brush.CreateSolidBrush(m_BrushXorColor); //BRUSH ��ü ����
    dc.SetROP2(R2_XORPEN);      //XOR������ ����
    oldpen = dc.SelectObject(&pen);    //�� ��ü, �Ʒ��� �귯�� ��ü ���
    oldbrush = dc.SelectObject(&brush);
    switch (m_nDrawMode)
    {
    case LINE_MODE:
        if (m_bLButtonDown)
        {

        }
        break;

    case ELLIPSE_MODE:  //�� �׸���
        if (m_bLButtonDown)
        {
            //���� ���� ����
            dc.Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
            //���� ���� �׸�
            dc.Ellipse(m_ptStart.x, m_ptStart.y, point.x, point.y);
            m_ptPrev = point; //�������� �������� ����
        }
        break;
    case RECTANGLE_MODE: //�簢�� �׸���
        if (m_bLButtonDown)
        {
            //���� �簢���� ����
            dc.Rectangle(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
            //���� ���� �׸�
            dc.Rectangle(m_ptStart.x, m_ptStart.y, point.x, point.y);
            m_ptPrev = point;
        }
        break;
    case POLY_RUBBER_MODE:
        if (!m_bFirst)
        {
            //���������� ������������ ���� ����
            dc.MoveTo(m_ptStart);
            dc.LineTo(m_ptPrev);
            //���������� ���������� ���� �׸�
            dc.MoveTo(m_ptStart);
            dc.LineTo(point);
            m_ptPrev = point;
        }
        break;
    }
    dc.SelectObject(oldpen);  //���� PEN���� ����
    dc.SelectObject(oldbrush);  //���� BRUSH�� ����
    pen.DeleteObject();
    brush.DeleteObject();

    //���콺 ��ġ ��ǥ�� status bar�� ����Ѵ�
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CString strPoint;
    strPoint.Format("���콺 ��ġ x : %d, y : %d", point.x, point.y);
    pFrame->m_wndStatusBar.SetWindowText(strPoint);
    CFormView::OnMouseMove(nFlags, point);
}
void CRectrictZoneView::OnLButtonDown(UINT nFlags, CPoint point)
{
    //�� ������ �ʱ�ȭ �� ȭ�� �ʱ�ȭ
    if (m_bFirst)
    {
        m_ptStart = m_ptPrev = 0;
        m_bLButtonDown = false;
        m_bFirst = true;
        m_nCount = 0;
        for (int i = 0; i < 50; i++)
            m_ptData[i] = 0;
        Invalidate(true); //ȭ�� ���� - true:ó������ ������ ����
    }
    CClientDC dc(this);  //Ŭ���̾�Ʈ ���� DC ����
    RECT rectClient;  //����ü ���� ����
    CPen pen, * oldpen;
    pen.CreatePen(PS_SOLID, 1, m_PenColor);  //PEN ��ü ����
    dc.SetROP2(R2_COPYPEN);  //COPYPEN���� ����
    oldpen = dc.SelectObject(&pen);
    switch (m_nDrawMode)
    {
    case LINE_MODE:  //���� �׸���
    case ELLIPSE_MODE: //�� �׸���
    case RECTANGLE_MODE: //�簢�� �׸���
        m_ptStart = m_ptPrev = point;
        m_bLButtonDown = true;
        m_bFirst = false;
        break;
    case POLY_RUBBER_MODE: //�ٰ��� �׸��� ������� �̿�
        if (m_bFirst)
        {
            m_bFirst = false;
        }
        //�������� �������� �������� ����
        m_ptStart = m_ptPrev = point;
        m_ptData[m_nCount] = point; //�迭�� ���� ���� ����
        m_nCount++;   //ī��Ʈ ����
        break;
    case POLY_NOTRUBBER_MODE:  //�ٰ��� �׸��� ������� �̿� ����
        if (m_bFirst)
        {
            m_ptStart = point; //�������� �������� ����
            m_bFirst = false;
        }
        else
        {
            dc.MoveTo(m_ptStart);
            dc.LineTo(point);  //�������� ���� ���� �׸�
            m_ptStart = point;
        }
        m_ptData[m_nCount] = point;  //�迭�� ���� ���� ����
        m_nCount++;  //ī��Ʈ ����
        break;
    }
    SetCapture(); //���콺 ĸ��
    GetClientRect(&rectClient);  //Ŭ���̾�Ʈ ���� ����
    ClientToScreen(&rectClient); //��ũ�� ��ǥ��� ��ȯ
    ::ClipCursor(&rectClient);  //���콺 �̵������� Ŭ���̾�Ʈ �������� ����
    dc.SelectObject(oldpen);  //���� PEN���� ����
    pen.DeleteObject();    //PEN��ü ����

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
    if (m_bLButtonDown) //���� ��ư�� ���� ���¸�
    {
        if (m_nDrawMode == LINE_MODE || m_nDrawMode == ELLIPSE_MODE || m_nDrawMode == RECTANGLE_MODE)
        {
            //üũ ���� �ʱ�ȭ => �ٽ� ����, ��, �簢���� �׸��� ����
            m_bLButtonDown = false;
            m_bFirst = true;
            ReleaseCapture();  //���콺 ĸ��, Ŭ�� ����
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
            //�迭�� �������� �����ϰ� ī��Ʈ ����
            m_ptData[m_nCount] = point;
            m_nCount++;
            //üũ���� �ʱ�ȭ => �ٽ� �簢���� �׸��� ����
            m_bFirst = true;
            ReleaseCapture(); //���콺 ĸ��, Ŭ�� ����
            ::ClipCursor(NULL);
            Invalidate(true);
        }
    }

    CFormView::OnRButtonDown(nFlags, point);
}
*/