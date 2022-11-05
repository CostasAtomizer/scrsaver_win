#pragma once


// TypeOfTheShowDialog dialog

class TypeOfTheShowDialog : public CDialogEx
{
	DECLARE_DYNAMIC(TypeOfTheShowDialog)

public:
	TypeOfTheShowDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TypeOfTheShowDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  BOOL TypeOfTheShowDialog::OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
//    int m_radio_show_types;
//    int m_radio_ts;
//    afx_msg void OnIddDialog1();
    int m_radio_ts;
    UINT m_number_of_colors;
    int m_number_pieces_of_circle;
    int m_circles_delta;
    BOOL m_synch_colors;
    double m_start_circle_offset;
};
