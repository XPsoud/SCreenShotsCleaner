#include "dlgabout.h"

#include "aboutlogo.h"
#include "appversion.h"

#include <wx/wx.h>
#include <wx/statbmp.h>
#include <wx/statline.h>

DlgAbout::DlgAbout(wxWindow *parent) : wxDialog(parent, -1, _T("..."))
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Creating a \"DlgAbout\" object\n"));
#endif // SHOW_DEBUG_MSG

	const int iSpacer=10;

	wxString sTitle;
	sTitle.Printf(_("About %s"), _T(PRODUCTNAME));
	SetTitle(sTitle);

	wxBitmap bmp(wxGet_wxWidgets_png_Bitmap());

	wxStaticText *label;
	wxString sMsg;

	wxBoxSizer *vszr0=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *hszr1=new wxBoxSizer(wxHORIZONTAL);
			wxStaticBitmap *stbLogo=new wxStaticBitmap(this, wxID_STATIC, bmp);
			hszr1->Add(stbLogo, 0, wxALL|wxALIGN_CENTER_VERTICAL, iSpacer);

			wxBoxSizer *vszr1=new wxBoxSizer(wxVERTICAL);
				sMsg=_T(PRODUCTNAME);
				sMsg << _T(" ") << _T("(v") << VERSION_MAJOR << _T(".") << VERSION_MINOR << _T(".") << VERSION_REV << _T(".") << VERSION_BUILD << _T(")");
				label=new wxStaticText(this, wxID_STATIC, sMsg, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER|wxST_NO_AUTORESIZE);
					wxFont bFnt=label->GetFont();
					bFnt.MakeLarger();
					bFnt.SetWeight(wxFONTWEIGHT_BOLD);
					label->SetFont(bFnt);
				vszr1->Add(label, 0, wxALL|wxEXPAND, iSpacer);

				label=new wxStaticText(this, wxID_STATIC, wDESCRIPTION);
				vszr1->Add(label, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER, iSpacer);

				label=new wxStaticText(this, wxID_STATIC, _T(COPYRIGHT));
				vszr1->Add(label, 0, wxALL|wxALIGN_CENTER, iSpacer);

				sMsg.Printf(_("Made with %s - %d bits"), wxVERSION_STRING, (int)(8*sizeof(void*)));
				label=new wxStaticText(this, wxID_STATIC, sMsg);
				vszr1->Add(label, 0, wxALL|wxALIGN_CENTER, iSpacer);
			hszr1->Add(vszr1, 0, wxALL|wxEXPAND, iSpacer);
		vszr0->Add(hszr1, 0, wxALL|wxEXPAND, 0);

		vszr0->Add(CreateSeparatedButtonSizer(wxCLOSE), 0, wxALL|wxEXPAND, iSpacer);

	SetSizer(vszr0);

	vszr0->SetSizeHints(this);

	CenterOnParent();
}

DlgAbout::~DlgAbout()
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Destroying a \"DlgAbout\" object\n"));
#endif // SHOW_DEBUG_MSG
}
