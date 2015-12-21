// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "mc.h"
#include "math.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CRITICAL_SECTION gCS;

unsigned int Mirror(unsigned int Argon, unsigned int Top)
{
unsigned int fang = 0x80000000;
unsigned int talon = 0x00000001;
unsigned int NextArg = 0x00000000;
int i;
	
	for(i = 0; (i < 32)&&((fang&Top)==0); i++, fang>>=1) {}

	if(i==32) 
	return(NextArg);

	for(; i < 32; i++, fang>>=1, talon<<=1) 
	{
		if((fang&Argon)!=0) NextArg|=talon;
	}
	
	return(NextArg);
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_COMMAND(ID_START_END, OnStartEnd)
	ON_COMMAND(ID_START_START, OnStartStart)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
: m_isthread(false)
{
    // TODO: add member initialization code here
    maxX = ::GetSystemMetrics(SM_CXSCREEN);
    maxY = ::GetSystemMetrics(SM_CYSCREEN);
	
    InitializeCriticalSection(&gCS);

    RECT *r = new(RECT);
    r->bottom = 0;
    r->left = 0;
    r->right = maxX;
    r->top = maxY;

    this->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

    CClientDC dc(this); 

    // Создание совместимого контекста устройства и битового образа

    m_memDC.CreateCompatibleDC(&dc);
    m_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);
    m_memDC.SelectObject(&m_bmp);

    // Выбор черной кисти для стирания фона окна и стирание фона

    m_bkbrush.CreateStockObject(BLACK_BRUSH);
    m_memDC.SelectObject(&m_bkbrush);

    m_memDC.PatBlt(0, 0, maxX, maxY, PATCOPY);

    this->ShowWindow(SW_RESTORE);
    this->UpdateWindow();

    image_0 = new CImage();   
    image_1 = new CImage();
    this->Palette_Init(256);
}

CMainFrame::~CMainFrame()
{
	  delete image_0;
    delete image_1;
    delete[] palette;
}

void CMainFrame::Palette_Init(int n)
{
    palette = new COLORREF[n];
    keys = new int[n];
    int* tmp = new int[n];
    
    COLORREF first = 0x00000405;
    byte r = (first >> 16) & 0xff, g = (first >> 8) & 0xff, b = first & 0xff;
    palette[0] = ((DWORD)r << 16) | ((DWORD)g << 8) | ((DWORD)b);

    for(int i = 1; i < n; i++)
    {
        r += 1;            
        g += 3;
        b += 7;
        palette[i] = ((DWORD)r << 16) | ((DWORD)g << 8) | ((DWORD)b);
    }   

    for(int i = 0; i < n; i++)
    {
        tmp[i] = palette[i]%n;
        keys[tmp[i]] = i;
    }    
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0,0,(HBRUSH)::GetStockObject(BLACK_BRUSH),0);
	
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
//	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


afx_msg void CMainFrame::OnTimer(UINT id)
{

}
 

afx_msg BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)

{
// Ничего не делаем, так как фон окна прорисовывать не нужно - 
// он будет прорисован при копировании контекста памяти в контекст окна.

	return TRUE; // Якобы фон прорисован
}
 

afx_msg void CMainFrame::OnPaint()
{
	RECT clientRect;
	CPaintDC pdc(this);

	this->GetClientRect(&clientRect);
	pdc.BitBlt(0, 0, clientRect.right, clientRect.bottom, &m_memDC, 0, 0, SRCCOPY);
}

afx_msg void CMainFrame::OnDestroy()
{
//this->KillTimer(1);
}



void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CFrameWnd::OnChar(nChar, nRepCnt, nFlags);

m_bkbrush.CreateStockObject(BLACK_BRUSH);
m_memDC.SelectObject(&m_bkbrush);
m_memDC.PatBlt(0, 0, maxX, maxY, PATCOPY);

this->InvalidateRect(0, FALSE);

}

void CMainFrame::OnStartEnd()
{
	// TODO: Add your command handler code here
	m_isthread = false;
//	AfxEndThread();
}

void CMainFrame::OnStartStart()
{
	// TODO: Add your command handler code here
  m_isthread = false;
  Sleep(300);
	m_isthread = true;

	CWinThread* pThr = AfxBeginThread(CMainFrame::DrawThread3, this, THREAD_PRIORITY_NORMAL);
}

//http://www.codeproject.com/Articles/1988/Guide-to-WIN-Paint-for-Beginners
//http://www.winprog.org/tutorial/bitmaps.html
UINT CMainFrame::DrawThread(void* param)
{
	CMainFrame* l_this = (CMainFrame*)param;	
	CClientDC cdc(l_this);

	RECT clientRect;
	l_this->GetClientRect(&clientRect);
	
	unsigned int x = 0;
	unsigned int y = 0;

	CBrush brush;
	CPen pen;
	COLORREF picolo, brushColor;
	LOGBRUSH logBrush;
	CPen pend;

	pend.CreatePen(PS_NULL, 0, &logBrush);

	cdc.BitBlt(0, 0, clientRect.right, clientRect.bottom, &l_this->m_memDC, 0, 0, SRCCOPY);	

	unsigned long int LL = 0; 
	
	unsigned long decor = 0;
	while(l_this->m_isthread)
	{
		x = clientRect.right - l_this->xRnd.genrand_int32()%clientRect.right; 
		y = clientRect.bottom - l_this->yRnd.genrand_int32()%clientRect.bottom;
		
		
		picolo = l_this->m_memDC.GetPixel(x, y);

		if(picolo == 0)
		{
			brushColor = RGB(rand()%256, rand()%256, rand()%256) ;
		}
		else
		{
			brushColor = picolo;
		}

		brush.CreateSolidBrush(brushColor);
		l_this->m_memDC.SelectObject(pend);
		l_this->m_memDC.SelectObject(brush);
		l_this->m_memDC.Rectangle(x - 1, y - 1, x + 3, y + 3);

		if(LL++%500 == 0)
			cdc.BitBlt(0, 0, clientRect.right, clientRect.bottom, &l_this->m_memDC, 0, 0, SRCCOPY);			

		brush.DeleteObject();
		pend.DeleteObject();
	}
	return 0;
}

UINT CMainFrame::DrawThread2(void* param)
{
	CMainFrame* l_this = (CMainFrame*)param;	
	CClientDC cdc(l_this);

	RECT clientRect;
	l_this->GetClientRect(&clientRect);

	CImage* image_ = new CImage();
	image_ -> Create( clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, 32 );
	image_->BitBlt(cdc, 0, 0, SRCCOPY);	

    byte* image_bits = (byte*)image_->GetBits();
    int image_bpp = image_->GetBPP()/8;
    int image_pitch = image_->GetPitch();

	unsigned int x = 0;
	unsigned int y = 0;
	unsigned long int LL = 0; 
	unsigned long decor = 0;
	COLORREF picolo, brushColor;

    unsigned int hh = clientRect.bottom - clientRect.top - 2;
    unsigned int ww = clientRect.right - clientRect.left - 2;

    for(int i = 0; i < hh + 2; i++)
    {
        for(int j = 0; j < ww + 2; j++)
        {
            *((DWORD*)(image_bits + i * image_pitch + j * image_bpp)) = ((int)((float)255*((float)i/(float)(hh + 2))))<<16;
        }
        image_->BitBlt(cdc, 0, 0, SRCCOPY);			
    }

    while(l_this->m_isthread)
	  {
        x = l_this->xRnd.genrand_int32() % ww + 1; 
        y = l_this->yRnd.genrand_int32() % hh + 1;
		
        picolo = (*((DWORD*)(image_bits + y * image_pitch + x * image_bpp)));
		brushColor = (picolo + 0x03) & 0xffffff;

        (*((DWORD*)(image_bits + (y-1) * image_pitch + (x-1) * image_bpp))) = brushColor;
        (*((DWORD*)(image_bits + (y) * image_pitch + (x-1) * image_bpp))) = brushColor;
        (*((DWORD*)(image_bits + (y+1) * image_pitch + (x-1) * image_bpp))) = brushColor;
        (*((DWORD*)(image_bits + (y-1) * image_pitch + (x) * image_bpp))) = brushColor;
        (*((DWORD*)(image_bits + (y-1) * image_pitch + (x+1) * image_bpp))) = brushColor;
        (*((DWORD*)(image_bits + (y) * image_pitch + (x+1) * image_bpp))) = brushColor;
        (*((DWORD*)(image_bits + (y+1) * image_pitch + (x) * image_bpp))) = brushColor;
        (*((DWORD*)(image_bits + (y+1) * image_pitch + (x+1) * image_bpp))) = brushColor;

		if(LL++%500 == 0)
			image_->BitBlt(cdc, 0, 0, SRCCOPY);			
	  }

	
	delete image_;

	return 0;
}

void CMainFrame::N_Test(byte* lp_image, int x, int y, int ip, int bpp)
{
int  nDelta[8][2] = { {-1, -1}, {0, -1}, {1, -1}, { 1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0} };
bool bx = false;

COLORREF brushColor = 0;
DWORD* x_lp = ((DWORD*)(lp_image + y * ip + x * bpp));

    if(*x_lp != 0)
    {
        //*x_lp = 0;
        return;
    }

    for(int i = 0; i < 8; i++)
    {
        DWORD x_bits = *((DWORD*)(lp_image + (y + nDelta[i][0]) * ip + (x + nDelta[i][1]) * bpp));
        if(x_bits != 0)
        {
            if(!bx)
            {
                brushColor = palette[(keys[x_bits % 256] + 1) % 256];
                bx = true;
            }
            else
                return;
        }
    }      
    *x_lp = brushColor;
}

void CMainFrame::ClearImage(byte* lp_image, int ip, int bpp, int ww, int hh)
{
    for(int i = 0; i < hh; i++)
    {
        for(int j = 0; j < ww; j++)        
        {
            *((DWORD*)(lp_image + i * ip + (j * bpp))) = 0x00;
        }
    }      
}

UINT CMainFrame::DrawThread3(void* param)
{
	  CMainFrame* l_this = (CMainFrame*)param;	
	  CClientDC cdc(l_this);

	  RECT clientRect;
	  l_this->GetClientRect(&clientRect);

	  l_this->image_0->Create( clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, 32);
	  l_this->image_0->BitBlt(cdc, 0, 0, SRCCOPY);	  

    byte* image_bits = (byte*)l_this->image_0->GetBits();
    int image_bpp = l_this->image_0->GetBPP()/8;
    int image_pitch = l_this->image_0->GetPitch();

	  unsigned int x = 0;
	  unsigned int y = 0;
	  unsigned long int LL = 0, XL = 0; 
	  COLORREF picolo;

    unsigned int hh = clientRect.bottom - clientRect.top - 2;
    unsigned int ww = clientRect.right - clientRect.left - 2;

    while(l_this->m_isthread)
	  {
        x = l_this->xRnd.genrand_int32() % ww + 1; 
        y = l_this->yRnd.genrand_int32() % hh + 1;
		        
        l_this->N_Test(image_bits, x, y, image_pitch, image_bpp);

        if(LL++%1500 == 0)
        {
            if(XL++%27000 == 0)
            {
                l_this->ClearImage(image_bits, image_pitch, image_bpp, ww + 1, hh + 1);
                //Draw for something shapes..
                HDC dc = l_this->image_0->GetDC();
                CDC cdc2;
                cdc2.Attach(dc);

		            COLORREF color;

		            CBrush brush;
                CPen pen;

                int x1, y1, x2, y2;
                for(int i = 0; i < 5; i++)
                {                            
                    color = l_this->palette[l_this->xRnd.genrand_int32()%5+3];
                    pen.CreatePen(0, 0, color);
		                cdc2.SelectObject(pen);

                    x1 = l_this->xRnd.genrand_int32()%ww;
                    y1 = l_this->xRnd.genrand_int32()%hh;
                    x2 = l_this->xRnd.genrand_int32()%ww;
                    y2 = l_this->xRnd.genrand_int32()%hh;


		                cdc2.MoveTo(ww/4, hh/2);
                    cdc2.LineTo((ww*3)/4, hh/2);


		                cdc2.MoveTo(x1, y1);
                    cdc2.LineTo(x2, y2);
		                cdc2.MoveTo(ww - x1, y1);
                    cdc2.LineTo(ww - x2, y2);
		                cdc2.MoveTo(x1, hh - y1);
                    cdc2.LineTo(x2, hh - y2);
		                cdc2.MoveTo(ww - x1, hh - y1);
                    cdc2.LineTo(ww - x2, hh - y2);

                    pen.Detach();
                }

                l_this->image_0->ReleaseDC();
                cdc2.Detach();
            }

            l_this->image_0->BitBlt(cdc, 0, 0, SRCCOPY);			
        }
	  }   

	  l_this->image_0->Destroy();
	  return 0;
}