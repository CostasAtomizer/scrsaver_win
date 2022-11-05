// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "TwRand.h"
#include "TypeOfTheShowDialog.h"
#include <atlimage.h>
#include "math.h"

class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)



// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Фигуры рисуются по сообщениям от таймера
afx_msg void OnTimer(UINT id);

// Обработчик сообщения WM_ERASEBKGND. Обычно по этому сообщению
// стирается фон окна. Наш же обработчик ничего не делает, так как 
// в окнах с виртуальным окном перерисовывать фон не нужно.

afx_msg BOOL OnEraseBkgnd(CDC* pDC);
afx_msg void OnPaint();
afx_msg void OnDestroy();

// Implementation
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	//CChildView    m_wndView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

private: 
	CWinAppEx *pApp;
	CDC m_memDC;
	CBitmap m_bmp;
	CBrush m_bkbrush;
	int maxX, maxY;
	COLORREF m_textColor;
	int NCOLORS;
	CTwRand xRnd, yRnd;
	CImage* image_0;
	CImage* image_1;
	DWORD** meta_indexes;
	DWORD** meta_indexes_back;
	COLORREF* palette;  
	TypeOfTheShowDialog xOptions;        

	typedef UINT (*pf)(void*);
	pf ArrayOfTheFunctions[5];
 
public:  
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnStartEnd();
	afx_msg void OnStartStart();
	afx_msg void OnViewFullscreen();
	afx_msg void OnStartOptionsDlg();

protected:
    void InitFrame(int ww, int hh);
    static UINT DrawThread(void* param);	  
    static UINT DrawThread2(void* param);
    static UINT DrawThread3(void* param);
    static UINT DrawThread4(void* param);
    static UINT DrawColors(void* param);
    inline void N_Test(byte* lp_image, int x, int y, int ip, int bpp);
    inline void N_Test2(byte* lp_image1, int hh, int ww, int ip, int bpp);
    inline void N_Test3(byte* lp_image, int x, int y, int ip, int bpp);
    void ClearImage(byte* lp_image, int ip, int bpp, int ww, int hh);
    void CopyImage(byte* lp_image_s, byte* lp_image_d, int ip, int bpp, int ww, int hh);
    void Palette_Init();
    void Shifting_By_Palette(byte* lp_image, int ip, int bpp, int ww, int hh);

	bool m_isthread;
};


