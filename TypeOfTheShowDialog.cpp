// TypeOfTheShowDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mc.h"
#include "TypeOfTheShowDialog.h"
#include "afxdialogex.h"


// TypeOfTheShowDialog dialog

IMPLEMENT_DYNAMIC(TypeOfTheShowDialog, CDialogEx)

TypeOfTheShowDialog::TypeOfTheShowDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TypeOfTheShowDialog::IDD, pParent)  
  , m_radio_ts(2)
  , m_number_of_colors(5000)
  , m_number_pieces_of_circle(190)
  , m_circles_delta(5)
  , m_synch_colors(TRUE)
  , m_start_circle_offset(-30.0)
{    
}

TypeOfTheShowDialog::~TypeOfTheShowDialog()
{
}

void TypeOfTheShowDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RADIO1, m_radio_ts);
    DDV_MinMaxInt(pDX, m_radio_ts, 0, 4);
    DDX_Text(pDX, IDC_EDIT1, m_number_of_colors);
    DDV_MinMaxUInt(pDX, m_number_of_colors, 256, 16777215);
    DDX_Text(pDX, IDC_EDIT2, m_number_pieces_of_circle);
    DDV_MinMaxInt(pDX, m_number_pieces_of_circle, 3, 25000);
    DDX_Text(pDX, IDC_EDIT3, m_circles_delta);
    DDV_MinMaxInt(pDX, m_circles_delta, 1, 90);
    DDX_Check(pDX, IDC_CHECK1, m_synch_colors);
    DDX_Text(pDX, IDC_EDIT4, m_start_circle_offset);
	DDV_MinMaxDouble(pDX, m_start_circle_offset, -1000.0, 1000.0);
}


BEGIN_MESSAGE_MAP(TypeOfTheShowDialog, CDialogEx)
END_MESSAGE_MAP()


// TypeOfTheShowDialog message handlers

BOOL TypeOfTheShowDialog::OnInitDialog()
{
   CDialog::OnInitDialog();

   // TODO: Add extra initialization here   
   UpdateData(false);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}


//void TypeOfTheShowDialog::OnIddDialog1()
//{
//    // TODO: Add your command handler code here
//}
