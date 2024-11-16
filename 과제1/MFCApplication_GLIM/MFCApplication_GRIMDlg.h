
// MFCApplication_GRIMDlg.h: 헤더 파일
//

#pragma once


// CMFCApplicationGRIMDlg 대화 상자
class CMFCApplicationGRIMDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplicationGRIMDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION_GRIM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnOk();
	
	CEdit m_Ed_x1;
	CEdit m_Ed_y1;
	CEdit m_Ed_x2;
	CEdit m_Ed_y2;
	CEdit m_EdRad;
	afx_msg void OnEnChangeEdY2();
	afx_msg void OnCircleDraw(bool Mode);
	afx_msg void  OnCircleDelete();
	afx_msg void OnCrossLineDraw(CPoint Center);
	CEdit m_CenX;
	CEdit m_CenY;
};
