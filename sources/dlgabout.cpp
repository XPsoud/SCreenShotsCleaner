#include "dlgabout.h"

#include "about.png.h"
#include "appversion.h"

#include <wx/wx.h>
#include <wx/statbmp.h>
#include <wx/statline.h>

DlgAbout::DlgAbout(wxWindow *parent) : wxDialog(parent, -1, _T("..."), wxDefaultPosition, wxDefaultSize)
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Creating a \"DlgAbout\" object\n"));
#endif // SHOW_DEBUG_MSG

    wxString sTitle;
    sTitle.Printf(_("About %s"), _T(PRODUCTNAME));
    SetTitle(sTitle);

	wxBitmap bmp(wxGet_about_png_Bitmap());

    wxStaticText *label;
    wxString sMsg;

    wxBoxSizer *vszr0=new wxBoxSizer(wxVERTICAL);
        wxBoxSizer *hszr1=new wxBoxSizer(wxHORIZONTAL);
            wxStaticBitmap *stbLogo=new wxStaticBitmap(this,-1,bmp);
            hszr1->Add(stbLogo,0,wxALL,5);

            wxBoxSizer *vszr1=new wxBoxSizer(wxVERTICAL);
                sMsg=_T(PRODUCTNAME);
                sMsg << _T(" ") << _T("(v") << VERSION_MAJOR << _T(".") << VERSION_MINOR << _T(".") << VERSION_REV << _T(")");
                label=new wxStaticText(this, -1, sMsg, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER|wxST_NO_AUTORESIZE);
                    wxFont bFnt=label->GetFont();
                    bFnt.SetPointSize(2+bFnt.GetPointSize());
                    bFnt.SetWeight(wxBOLD);
                    label->SetFont(bFnt);
                vszr1->Add(label, 0, wxALL|wxEXPAND, 5);

                label=new wxStaticText(this, -1, wDESCRIPTION, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER|wxST_NO_AUTORESIZE);
                vszr1->Add(label, 0, wxALL|wxEXPAND, 5);

                label=new wxStaticText(this, -1, _T(COPYRIGHT), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER|wxST_NO_AUTORESIZE);
                vszr1->Add(label, 0, wxALL|wxEXPAND, 5);

                sMsg.Printf(_("Made with Code::Blocks && %s - %d bits"), wxVERSION_STRING, (int)(8*sizeof(void*)));
                label=new wxStaticText(this, -1, sMsg, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER|wxST_NO_AUTORESIZE);
                vszr1->Add(label, 0, wxALL|wxEXPAND, 5);
            hszr1->Add(vszr1, 0, wxALL|wxEXPAND, 5);
        vszr0->Add(hszr1, 0, wxALL|wxEXPAND, 0);

        wxStaticLine *hline=new wxStaticLine(this, -1, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        vszr0->Add(hline, 0, wxALL|wxEXPAND, 5);

        wxButton *btnOK=new wxButton(this, wxID_CANCEL, _("Close"));
        vszr0->Add(btnOK, 0, wxALL|wxALIGN_RIGHT, 5);
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
