#include "dlgoptions.h"

#include "settingsmanager.h"

#include <wx/statline.h>
#include <wx/notebook.h>

DlgOptions::DlgOptions(wxWindow *parent)
    : wxDialog(parent, -1, _("Preferences"), wxDefaultPosition, wxDefaultSize),
    m_options(SettingsManager::Get())
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Creating a \"DlgOptions\" object\n"));
#endif // SHOW_DEBUG_MSG
	CreateControls();
	ConnectControls();

	CenterOnParent();

    m_btnApply->Disable();
}

DlgOptions::~DlgOptions()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Destroying a \"DlgOptions\" object\n"));
#endif // SHOW_DEBUG_MSG
}

void DlgOptions::CreateControls()
{
    wxBoxSizer *szrMain=new wxBoxSizer(wxVERTICAL), *lnszr, *pageszr;

        m_nBook=new wxNotebook(this, -1);
        wxPanel *page;
        wxStaticBoxSizer *box;
        wxStaticText *label;

        // Page "General"
        page=new wxPanel(m_nBook, -1);
            pageszr=new wxBoxSizer(wxVERTICAL);
                box=new wxStaticBoxSizer(wxHORIZONTAL, page, _("Language :"));
                    label=new wxStaticText(page, -1, _("Select language (restart needed) :"));
                    box->Add(label, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5);
                    m_cmbLang = new wxChoice(page, -1);
                        // Add the available languages
                        wxArrayString langs=m_options.GetAvailableLanguages();
                        int iCurLang=m_options.GetCurrentLanguage();
                        int iCount=langs.Count();
                        for (int i=0;i<iCount; i++)
                        {
                            long l;
                            langs[i].Left(5).ToLong(&l);
                            langs[i].Remove(0,5);
                            int id=m_cmbLang->Append(langs[i], (void*)((unsigned long)l));
                            if (iCurLang==l) m_cmbLang->Select(id);
                        }
                    box->Add(m_cmbLang, 1, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5);
                pageszr->Add(box, 0, wxALL|wxEXPAND, 5);

            page->SetSizer(pageszr);
        m_nBook->AddPage(page, _("General"));

        szrMain->Add(m_nBook, 1, wxALL|wxEXPAND, 0);

        szrMain->Add(new wxStaticLine(this, -1), 0, wxTOP|wxBOTTOM|wxEXPAND, 5);

        lnszr=new wxBoxSizer(wxHORIZONTAL);
            m_btnOk=new wxButton(this, wxID_OK, _("OK"));
            lnszr->Add(m_btnOk, 0, wxALL, 0);
            m_btnCancel=new wxButton(this, wxID_CANCEL, _("Cancel"));
            lnszr->Add(m_btnCancel, 0, wxLEFT|wxRIGHT, 5);
            m_btnApply=new wxButton(this, wxID_APPLY, _("Apply"));
            lnszr->Add(m_btnApply, 0, wxALL, 0);
        szrMain->Add(lnszr, 0, wxALL|wxALIGN_RIGHT, 5);
    SetSizer(szrMain);

    szrMain->SetSizeHints(this);

    // Fill controls
}

void DlgOptions::ConnectControls()
{
    Connect(wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgOptions::OnBtnApplyClicked));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgOptions::OnBtnOkClicked));

    //Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(DlgOptions::OnSomethingHasChanged));
    //Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(DlgOptions::OnSomethingHasChanged));
    //Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(DlgOptions::OnSomethingHasChanged));
    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(DlgOptions::OnSomethingHasChanged));
}

void DlgOptions::ApplySettings()
{
    // GUI language
    long l=wxUIntPtr(m_cmbLang->GetClientData(m_cmbLang->GetSelection()));
    m_options.SetLanguage((int)l);

    m_btnApply->Disable();
}

void DlgOptions::OnSomethingHasChanged(wxCommandEvent& event)
{
    m_btnApply->Enable();
}

void DlgOptions::OnBtnApplyClicked(wxCommandEvent& event)
{
    ApplySettings();
}

void DlgOptions::OnBtnOkClicked(wxCommandEvent& event)
{
    ApplySettings();
    EndModal(wxID_OK);
}
