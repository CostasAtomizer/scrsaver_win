// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "mc.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CRITICAL_SECTION gCS;

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
  ON_COMMAND(ID_OPTIONS, OnStartOptionsDlg)
  END_MESSAGE_MAP()

  static UINT indicators[] =
  {
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

static const double PI = 3.14159265358979323846;

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

  m_memDC.CreateCompatibleDC(&dc);
  m_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);
  m_memDC.SelectObject(&m_bmp);

  m_bkbrush.CreateStockObject(BLACK_BRUSH);
  m_memDC.SelectObject(&m_bkbrush);

  m_memDC.PatBlt(0, 0, maxX, maxY, PATCOPY);

  this->ShowWindow(SW_RESTORE);
  this->UpdateWindow();

  image_0 = new CImage();
  image_1 = new CImage();
  palette = NULL; 
  this->Palette_Init();

  ArrayOfTheFunctions[0] = CMainFrame::DrawColors;
  ArrayOfTheFunctions[1] = CMainFrame::DrawThread4;
  ArrayOfTheFunctions[2] = CMainFrame::DrawThread3;
  ArrayOfTheFunctions[3] = CMainFrame::DrawThread3;
  ArrayOfTheFunctions[4] = CMainFrame::DrawThread4;
}

CMainFrame::~CMainFrame()
{
  delete image_0;
  delete image_1;
  delete[] palette;
}

void CMainFrame::Palette_Init()
{
  if(palette)
    delete palette;

  int n = xOptions.m_number_of_colors;
  NCOLORS = n;
  palette = new COLORREF[n];
 
  byte r = 0, g = 0, b = 0;
  memset(palette, 0, n * 4);
 
  for(int i = 1; i < n; i++)
  {
    double t = i * (PI * 2) / n;

    r = 128 + 127 * sin(t * (n / 256) * 1.7 + PI / 7);    
    g = 128 + 127 * sin(t * (n / 256) * 1.1 + PI / 4.5);
    b = 128 + 127 * sin(t * (n / 256) * 2.8 + PI / 9);
    
    if((i & 0xfedb) == i)
        palette[i] = ((DWORD)r << 16) | ((DWORD)g << 8) | ((DWORD)b);
  }

  //DWORD modifier = 0;
  //for(int i = 0; i < 2; i++)
  //{
  //    DWORD tmp;
  //    for(int k = 0; k < NCOLORS && m_isthread; k++)
  //    {
  //      tmp = palette[NCOLORS - 1];
  //      memcpy(palette + 2, palette + 1, (NCOLORS - 2) * 4);
  //      
  //      palette[1] = tmp ^ modifier;
  //    }
  //    modifier = (xRnd.genrand_int32() % 256) << 16 | (xRnd.genrand_int32() % 256) << 8 | (xRnd.genrand_int32() % 256);
  //}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1; 

  EnableFullScreenMode(ID_START_START);

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
//  m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // let the view have first crack at the command
  //if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
  //  return TRUE;

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
//  AfxEndThread();
}

void CMainFrame::OnViewFullscreen()
{
    ShowFullScreen();
}

void CMainFrame::OnStartStart()
{
  // TODO: Add your command handler code here
  m_isthread = false;
  Sleep(300);
  m_isthread = true;

  ShowFullScreen();  
  CWinThread* pThr = AfxBeginThread(ArrayOfTheFunctions[xOptions.m_radio_ts], this, THREAD_PRIORITY_NORMAL);
}

void CMainFrame::OnStartOptionsDlg()
{    
   xOptions.DoModal();    
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
      brushColor = RGB(rand()%256, rand()%256, rand()%256);
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

  int hh = clientRect.bottom - clientRect.top - 2;
  int ww = clientRect.right - clientRect.left - 2;

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
  bool b1 = false;
  bool b2 = false;

  if(meta_indexes[y][x] != 0)
  {
    return;
  }

  DWORD index1 = 0;
  DWORD index2 = 0;
  DWORD index = 0;
  for(int i = 0; i < 8; i++)
  {
    if(meta_indexes[y + nDelta[i][0]][x + nDelta[i][1]] != 0)
    {
      if(!b1)
      {        
        index1 = meta_indexes[y + nDelta[i][0]][x + nDelta[i][1]];
        b1 = true;
      }
      else
      {
        if(!b2)
        {
          index2 = meta_indexes[y + nDelta[i][0]][x + nDelta[i][1]];
          b2 = true;
        }
        else
        {
          return;
        }
      }
    }
  }
  index = (index1 > index2)? index1 : index2;

  if(index > 0)
  {
    index = (index + 1) % NCOLORS;
    if(index == 0) index++;
    meta_indexes[y][x] = index;
    *((DWORD*)(lp_image + y * ip + x * bpp)) = palette[index];
  }
}

void CMainFrame::N_Test2(byte* lp_image1, int hh, int ww, int ip, int bpp)
{
  int  nDelta[8][2] = { {-1, -1}, {0, -1}, {1, -1}, { 1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0} };

  COLORREF brushColor = 0;
  for(int y = 1; y < hh; y++)
  {
    for(int x = 1; x < ww && m_isthread; x++)
    {
      meta_indexes_back[y][x] = meta_indexes[y][x];

      if(meta_indexes_back[y][x] != 0)
      {
        continue;
      }

      bool bx = false;
      for(int i = 0; i < 8; i++)
      {
        if(meta_indexes[y + nDelta[i][0]][x + nDelta[i][1]] != 0)
        {
          if(!bx)
          {
            meta_indexes_back[y][x] = (meta_indexes[y + nDelta[i][0]][x + nDelta[i][1]] + 1) % NCOLORS;
            if(meta_indexes_back[y][x] == 0) meta_indexes_back[y][x]++;
            bx = true;
          }
          else
          {
            //meta_indexes_back[y][x] = 0;
            break;
          }
        }
      }
    }
   }

   for(int y = 1; y < hh; y++)
   {
     for(int x = 1; x < ww; x++)
     {
       *(DWORD*)(lp_image1 + y * ip + x * bpp) = palette[meta_indexes_back[y][x]];
       meta_indexes[y][x] = meta_indexes_back[y][x];
       meta_indexes_back[y][x] = 0;
     }
   }
}

void CMainFrame::N_Test3(byte* lp_image, int x, int y, int ip, int bpp)
{
  int  nDelta[8][2] = { {-1, -1}, {0, -1}, {1, -1}, { 1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0} };

  if(meta_indexes[y][x] != 0)
  {
    return;
  }

  DWORD index1 = 0;
  DWORD index2 = 0;
  DWORD index = 0;
  for(int i = 0; i < 8; i++)
  {
    if(meta_indexes[y + nDelta[i][0]][x + nDelta[i][1]] != 0)
    {
        index = meta_indexes[y + nDelta[i][0]][x + nDelta[i][1]];
        break;
    }
  }
  //index = (index1 > index2)? index1 : index2;

  if(index > 0)
  {
    index = (index + 1) % NCOLORS;
    if(index == 0) index++;
    meta_indexes[y][x] = index;
    *((DWORD*)(lp_image + y * ip + x * bpp)) = palette[index];
  }
}

void CMainFrame::ClearImage(byte* lp_image, int ip, int bpp, int ww, int hh)
{
  for(int i = 0; i < hh; i++)
  {
    for(int j = 0; j < ww; j++)
    {
      *((DWORD*)(lp_image + i * ip + (j * bpp))) = 0x00;
      meta_indexes[i][j] = 0x00;
    }
  }
}

void CMainFrame::CopyImage(byte* lp_image_s, byte* lp_image_d, int ip, int bpp, int ww, int hh)
{
  for(int i = 0; i < hh; i++)
  {
    for(int j = 0; j < ww; j++)
    {
      *((DWORD*)(lp_image_d + i * ip + (j * bpp))) = *((DWORD*)(lp_image_s + i * ip + (j * bpp)));
    }
  }
}

void CMainFrame::Shifting_By_Palette(byte* lp_image, int ip, int bpp, int ww, int hh)
{
  CClientDC cdc(this);

  DWORD modifier = 0;
  
  for(byte x = 0; m_isthread; x++)
  {
      DWORD tmp;
      for(int k = 0; k < NCOLORS && m_isthread; k++)
      {
        tmp = palette[NCOLORS - 1];
        memcpy(palette + 2, palette + 1, (NCOLORS - 2) * 4);

        if(tmp != 0x0)
            palette[1] = tmp ^ modifier;
        else
            palette[1] = tmp;

        for(int i = 0; i < hh; i++)
          for(int j = 0; j < ww; j++)
            *((DWORD*)(lp_image + i * ip + j * bpp)) = palette[meta_indexes[i][j]];

        image_0->BitBlt(cdc, 0, 0, SRCCOPY);
        Sleep(40);
      }
      modifier = (xRnd.genrand_int32() % 256) << 16 | (xRnd.genrand_int32() % 256) << 8 | (xRnd.genrand_int32() % 256);
  }
}

UINT CMainFrame::DrawColors(void* param)
{
  CMainFrame* l_this = (CMainFrame*)param;

  l_this->ShowFullScreen();

  CClientDC cdc(l_this);
  RECT clientRect;
  l_this->GetClientRect(&clientRect);

  l_this->Palette_Init();

  l_this->image_0->Create( clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, 32);
  l_this->image_0->BitBlt(cdc, 0, 0, SRCCOPY);

  byte* image_bits = (byte*)l_this->image_0->GetBits();
  int image_bpp = l_this->image_0->GetBPP()/8;
  int image_pitch = l_this->image_0->GetPitch();

  unsigned int hh = clientRect.bottom - clientRect.top - 2;
  unsigned int ww = clientRect.right - clientRect.left - 2;

  //----- Indexes Frames Initializations ----------->>
  l_this->meta_indexes = new DWORD*[hh + 2];
  for(int i = 0; i < hh + 2; i++)
  {
     l_this->meta_indexes[i] = new DWORD[ww + 2];
     memset(l_this->meta_indexes[i], 0, (ww + 2) * 4);
  }
  //----- Indexes Frames Initializations -----------<<

  int x = clientRect.left;
  int y = clientRect.top;
  int x_delta = ww/256;

  for(int i = 1; i < l_this->NCOLORS; i++)
  {    
    for(int m = 0; m < 15; m++)
    {
       for(int k = 0; k < x_delta; k++)
       {     
         l_this->meta_indexes[y + m][x + k] = i;
         *((DWORD*)(image_bits + (y + m) * image_pitch + ((x + k) * image_bpp))) = l_this->palette[i];
       }
    }
    x += x_delta;

    if((i % 256) == 0 && i >= 256)
    {
      x = clientRect.left;
      y += 16;
    }
  }

  l_this->image_0->BitBlt(cdc, 0, 0, SRCCOPY);
  
  Sleep(2000);
  l_this->Shifting_By_Palette(image_bits, image_pitch, image_bpp, ww + 1, hh + 1);

  //----- Indexes Frames Releasing --------------->>
  for(int i = 0; i < hh + 2; i++)
  {
     delete[] l_this->meta_indexes[i];
  }
  delete[] l_this->meta_indexes;
  //----- Indexes Frames Releasing ---------------<<

  l_this->image_0->Destroy();

  return 0;
}

//void CMainFrame::InitFrame(int ww, int hh)
//{
//    int x1, y1;
//    DWORD c_ind = 1;
//
//    byte* image_bits_0 = (byte*)image_0->GetBits();
//
//    int bpp = image_0->GetBPP()/8;
//    int ip = image_0->GetPitch();
//
//    c_ind = xRnd.genrand_int32() % (NCOLORS - 1) + 1;
//
//    for(int i = 0; i < 18000; i++ )
//    {
//        x1 = xRnd.genrand_int32() % ww + 1;
//        y1 = xRnd.genrand_int32() % hh + 1;
//
//        c_ind = xRnd.genrand_int32() % (NCOLORS - 1) + 1;
//
//        meta_indexes[y1][x1] = c_ind;
//        *((DWORD*)(image_bits_0 + y1 * ip + (x1 * bpp))) = palette[c_ind];
//    }
//}

void CMainFrame::InitFrame(int ww, int hh)
{
    int x1, y1, x2, y2, x_p, y_p;
    DWORD color;
    DWORD c_ind = 1;
    int circle_pieces = xOptions.m_number_pieces_of_circle;
    int delta = xOptions.m_circles_delta;

    byte* image_bits_0 = (byte*)image_0->GetBits();

    int bpp = image_0->GetBPP()/8;
    int ip = image_0->GetPitch();

    for(int j = 0; j < 360; j += delta )
    {
        x2 = ww/4.5 * sin(j * PI/180.0) + ww/2.0;
        y2 = hh/4.5 * cos(j * PI/180.0) + hh/2.0;

        c_ind = 1;
        double offset = xOptions.m_start_circle_offset * ((double)j / (double)delta) * PI / 180.0;

        for(int i = 0; i < circle_pieces; i++ )
        {

            if(xOptions.m_synch_colors)
            {
                x1 = ww/4.0 * sin(j * PI / 180.0 + i * 2 * PI / (float)circle_pieces) + x2;
                y1 = hh/4.0 * cos(j * PI / 180.0 + i * 2 * PI / (float)circle_pieces) + y2;
            }
            else
            {
                x1 = ww/4.0 * sin(offset + (double)i * 2.0 * PI / (float)circle_pieces) + x2;
                y1 = hh/4.0 * cos(offset + (double)i * 2.0 * PI / (float)circle_pieces) + y2;
            }

            meta_indexes[y1][x1] = c_ind;            
            *((DWORD*)(image_bits_0 + y1 * ip + (x1 * bpp))) = palette[c_ind];

            if((x_p != x1 || y_p != y1) && (i != 0))
            {
                c_ind = (c_ind + 1) % NCOLORS;
                if(c_ind == 0) c_ind++;
            }
            x_p = x1;
            y_p = y1;
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

  unsigned int hh = abs(clientRect.bottom - clientRect.top) - 2;
  unsigned int ww = abs(clientRect.right - clientRect.left) - 2;

  //----- Indexes Frames Initializations ----------->>
  l_this->meta_indexes = new DWORD*[hh + 2];
  for(int i = 0; i < hh + 2; i++)
  {
     l_this->meta_indexes[i] = new DWORD[ww + 2];
     memset(l_this->meta_indexes[i], 0, (ww + 2) * 4);
  }
  //----- Indexes Frames Initializations -----------<<

  l_this->ClearImage(image_bits, image_pitch, image_bpp, ww + 1, hh + 1);
  l_this->InitFrame(ww, hh);

  while(l_this->m_isthread)
  {
    if(LL++%750000 == 0)
    {
      if(XL++%150 == 0)
      {
        if(XL > 150)
        {
          l_this->Shifting_By_Palette(image_bits, image_pitch, image_bpp, ww + 1, hh + 1);
        }
      }
      l_this->image_0->BitBlt(cdc, 0, 0, SRCCOPY);
    }

    x = l_this->xRnd.genrand_int32() % ww + 1;
    y = l_this->yRnd.genrand_int32() % hh + 1;
    
    switch(l_this->xOptions.m_radio_ts)
    {
        case 2:    l_this->N_Test3(image_bits, x, y, image_pitch, image_bpp);
        break;
        case 3:    l_this->N_Test(image_bits, x, y, image_pitch, image_bpp);
        break;
    }
  }

  //----- Indexes Frames Releasing --------------->>
  for(int i = 0; i < hh + 2; i++)
  {
     delete[] l_this->meta_indexes[i];
  }
  delete[] l_this->meta_indexes;
  //----- Indexes Frames Releasing ---------------<<

  l_this->image_0->Destroy();
  return 0;
}

UINT CMainFrame::DrawThread4(void* param)
{
  CMainFrame* l_this = (CMainFrame*)param;
  CClientDC cdc(l_this);

  RECT clientRect;
  l_this->GetClientRect(&clientRect);
  l_this->Palette_Init();

  l_this->image_0->Create( clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, 32);
  l_this->image_1->Create( clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, 32);

  byte* image_bits_0 = (byte*)l_this->image_0->GetBits();
  byte* image_bits_1 = (byte*)l_this->image_1->GetBits();
  byte* image_swap;

  int image_bpp = l_this->image_0->GetBPP()/8;
  int image_pitch = l_this->image_0->GetPitch();

  unsigned long int LL = 0, XL = 0;

  unsigned int hh = abs(clientRect.bottom - clientRect.top) - 2;
  unsigned int ww = abs(clientRect.right - clientRect.left) - 2;

  //----- Indexes Frames Initializations ----------->>
  l_this->meta_indexes = new DWORD*[hh + 2];
  l_this->meta_indexes_back = new DWORD*[hh + 2];
  for(int i = 0; i < hh + 2; i++)
  {
     l_this->meta_indexes[i] = new DWORD[ww + 2];
     memset(l_this->meta_indexes[i], 0, (ww + 2) * 4);
     l_this->meta_indexes_back[i] = new DWORD[ww + 2];
     memset(l_this->meta_indexes_back[i], 0, (ww + 2) * 4);
  }
  //----- Indexes Frames Initializations -----------<<

  while(l_this->m_isthread)
  {
    l_this->ClearImage(image_bits_0, image_pitch, image_bpp, ww + 1, hh + 1);
    l_this->Palette_Init();
    l_this->InitFrame(ww, hh);

    for(LL = 0; LL < 350 && l_this->m_isthread; LL++) // It (350) depends from client size.
    {
        l_this->image_0->BitBlt(cdc, 0, 0, SRCCOPY);

        if(l_this->xOptions.m_radio_ts == 4)
          break;

        l_this->N_Test2(image_bits_0, hh, ww, image_pitch, image_bpp);
    }
    l_this->Shifting_By_Palette(image_bits_0, image_pitch, image_bpp, ww + 1, hh + 1);
  }

  //----- Indexes Frames Releasing --------------->>
  for(int i = 0; i < hh + 2; i++)
  {
     delete[] l_this->meta_indexes[i];
     delete[] l_this->meta_indexes_back[i];
  }
  delete[] l_this->meta_indexes;
  delete[] l_this->meta_indexes_back;
  //----- Indexes Frames Releasing ---------------<<

  l_this->image_0->Destroy();
  l_this->image_1->Destroy();
  return 0;
}