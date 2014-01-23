#include "mainframe.h"

#include "dlgabout.h"
#include "dlgoptions.h"
#include "menu_icons.h"
#include "settingsmanager.h"

#include <wx/rawbmp.h>
#include <wx/filename.h>

MainFrame::MainFrame(wxWindow *parent, const wxString& title) :
    wxFrame(parent, wxID_ANY, title), m_options(SettingsManager::Get())
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Creating a \"MainFrame\" object\n"));
#endif // SHOW_DEBUG_MSG

    SetIcon(wxICON(appicon));

    CreateControls();
    ConnectControls();

    UpdateControlsState();

    SetMinSize(MAINFRAME_MIN_SIZE);

    wxPoint pt=m_options.GetLastSavedPos();
    wxSize sz=m_options.GetLastSavedSize();

    if (sz==wxDefaultSize)
    {
        Maximize(true);
    }
    else
    {
        Move(pt);
        SetSize(sz);
    }
}

MainFrame::~MainFrame()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Destroying a \"MainFrame\" object\n"));
#endif // SHOW_DEBUG_MSG
}

void MainFrame::CreateControls()
{
    const int iSpaces = 5;
    // Status Bar
    CreateStatusBar();
    // Menu Bar
    wxMenuItem *item;
    wxMenu *menu;
    wxMenuBar* mbar = new wxMenuBar();

    menu = new wxMenu(_T(""));
        item=new wxMenuItem(menu, wxID_OPEN, _("Open"), _("Select the screenshot file to clean"));
        item->SetBitmap(wxGet_file_open_png_Bitmap());
        menu->Append(item);

        menu->AppendSeparator();

        item=new wxMenuItem(menu, wxID_SAVE, _("Save"), _("Save the cleaned image"));
        item->SetBitmap(wxGet_picture_save_png_Bitmap());
        menu->Append(item);

        menu->AppendSeparator();

        item=new wxMenuItem(menu, wxID_EXIT, _("Exit"), _("Quit this application"));
        item->SetBitmap(wxGet_app_exit_png_Bitmap());
        menu->Append(item);
    mbar->Append(menu, _("&File"));

    menu = new wxMenu(_T(""));
        item=new wxMenuItem(menu, wxID_PREFERENCES, _("Preferences"), _("Change the application settings"));
        item->SetBitmap(wxGet_configure_png_Bitmap());
        menu->Append(item);
    mbar->Append(menu, _("&Edit"));

    menu = new wxMenu(_T(""));
        item=new wxMenuItem(menu, wxID_ABOUT, _("About"), _("Show informations about this application"));
        item->SetBitmap(wxGet_help_about_png_Bitmap());
        menu->Append(item);
    mbar->Append(menu, _("&Help"));

    SetMenuBar(mbar);

    // Controls
    wxPanel *pnl=new wxPanel(this, -1);
    wxStaticText *label;
    wxBoxSizer *szrMain=new wxBoxSizer(wxVERTICAL), *lnszr;

        label=new wxStaticText(pnl, -1, _("Source file to clean:"));
        szrMain->Add(label, 0, wxLEFT|wxRIGHT|wxTOP, iSpaces);
        szrMain->AddSpacer(1);

        lnszr=new wxBoxSizer(wxHORIZONTAL);
            m_txtSrcFile=new wxTextCtrl(pnl, -1, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
            lnszr->Add(m_txtSrcFile, 1, wxALL|wxALIGN_CENTER_VERTICAL, 0);
            m_btnBrwseSrc=new wxButton(pnl, -1, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
            lnszr->Add(m_btnBrwseSrc, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, iSpaces);
        szrMain->Add(lnszr, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);

        lnszr=new wxBoxSizer(wxHORIZONTAL);
            m_chkIncrease=new wxCheckBox(pnl, -1, _("Increase the image size by"));
            lnszr->Add(m_chkIncrease, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
            m_spnIncrease=new wxSpinCtrl(pnl, -1, _T("5"), wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 1, 100, 5);
            lnszr->Add(m_spnIncrease, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, iSpaces);
            label=new wxStaticText(pnl, -1, _("pixels per side"));
            lnszr->Add(label, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, iSpaces);
        szrMain->Add(lnszr, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);

        lnszr=new wxBoxSizer(wxHORIZONTAL);
            lnszr->AddStretchSpacer(1);
            m_btnSave=new wxButton(pnl, wxID_SAVE, _("Save the cleaned image"));
            lnszr->Add(m_btnSave, 0, wxALL, 0);
        szrMain->Add(lnszr, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);
    pnl->SetSizer(szrMain);

    // Default values
    m_chkIncrease->SetValue(false);
}

void MainFrame::ConnectControls()
{
    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrame::OnClose));
    Connect(wxEVT_SIZE, wxSizeEventHandler(MainFrame::OnSize));
    Connect(wxEVT_MOVE, wxMoveEventHandler(MainFrame::OnMove));

    Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuOpenClicked));
    Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuSaveClicked));
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuExitClicked));
    Connect(wxID_PREFERENCES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuOptionsClicked));
    Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuAboutClicked));

    m_btnBrwseSrc->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnMenuOpenClicked), NULL, this);
    m_chkIncrease->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainFrame::OnChkIncreaseClicked), NULL, this);
    m_btnSave->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnMenuSaveClicked), NULL, this);

    Connect(wxID_SAVE, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnUpdateUI_MenuSave));
}

void MainFrame::UpdateControlsState()
{
    m_spnIncrease->Enable(m_chkIncrease->IsChecked());
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    event.Skip();
}

void MainFrame::OnSize(wxSizeEvent& event)
{
    if (!IsShown()) return;

    if (IsMaximized())
    {
        m_options.SetLastWindowRect(wxDefaultPosition, wxDefaultSize);
    }
    else
    {
        m_options.SetLastWindowRect(GetPosition(), GetSize());
    }

    event.Skip();
}

void MainFrame::OnMove(wxMoveEvent& event)
{
    if (!IsShown()) return;
    m_options.SetLastWindowRect(GetPosition(), GetSize());
    event.Skip();
}

void MainFrame::OnMenuOpenClicked(wxCommandEvent &event)
{
    wxString sMsg=_("Select an image file");
    wxString sWlcrd=_("Images files|*.png;*.jpg;*.bmp|All files|*.*");
    wxString sName, sPath;
    if (!m_txtSrcFile->IsEmpty())
    {
        sName=wxFileNameFromPath(m_txtSrcFile->GetValue());
        sPath=wxPathOnly(m_txtSrcFile->GetValue());
    }
    wxFileDialog fdlg(this, sMsg, sPath, sName, sWlcrd, wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (fdlg.ShowModal()!=wxID_OK) return;

    m_txtSrcFile->ChangeValue(fdlg.GetPath());
}

void MainFrame::OnMenuExitClicked(wxCommandEvent& event)
{
    Destroy();
}

void MainFrame::OnMenuOptionsClicked(wxCommandEvent& event)
{
    int iLngCode=m_options.GetCurrentLanguage();
    DlgOptions dlg(this);
    dlg.ShowModal();
    if (iLngCode!=m_options.GetCurrentLanguage())
    {
        wxMessageBox(_("The interface language has been changed.\nYou must restart the application for this change to take effect."), _("Restart needed"), wxICON_INFORMATION|wxOK|wxCENTER);
    }
}

void MainFrame::OnMenuAboutClicked(wxCommandEvent& event)
{
    DlgAbout dlg(this);
    dlg.ShowModal();
}

void MainFrame::OnChkIncreaseClicked(wxCommandEvent& event)
{
    UpdateControlsState();
}

void MainFrame::OnUpdateUI_MenuSave(wxUpdateUIEvent& event)
{
    bool bEnable=true;
    if (m_txtSrcFile->IsEmpty())
    {
        bEnable=false;
    }
    else
    {
        if (!wxFileExists(m_txtSrcFile->GetValue()))
            bEnable=false;
    }
    event.Enable(bEnable);
}

void MainFrame::OnMenuSaveClicked(wxCommandEvent& event)
{
    wxFileName SrcFName(m_txtSrcFile->GetValue());
    // First, check the file name and path
    if (!SrcFName.IsOk() || !SrcFName.FileExists())
    {
        wxMessageBox(_("Invalid source file name !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
        m_txtSrcFile->SetFocus();
        return;
    }
    if (!wxImage::CanRead(SrcFName.GetFullPath()))
    {
        wxMessageBox(_("Unsupported image format !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
        return;
    }
    wxImage imgSrc;
    // Try to load the corresponding image
    if (!imgSrc.LoadFile(SrcFName.GetFullPath(), wxBITMAP_TYPE_ANY))
    {
        wxMessageBox(_("Unable to load the image file !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
        return;
    }
    wxBitmapType iSrcType=imgSrc.GetType();

    if (!imgSrc.HasAlpha()) imgSrc.InitAlpha();

    // Clear pixels of rounded angles
    wxSize szImg=imgSrc.GetSize();
    int iWdth=szImg.GetWidth()-1, iHght=szImg.GetHeight()-1;
    int iClear[5]={5,3,2,1,1};
    for (int y=0; y<5; y++)
    {
        for (int x=0; x<iClear[y]; x++)
        {
            imgSrc.SetAlpha(x, y, 0); imgSrc.SetRGB(x, y, 255, 255, 255);
            imgSrc.SetAlpha(iWdth-x, y, 0); imgSrc.SetRGB(iWdth-x, y, 255, 255, 255);
            imgSrc.SetAlpha(x, iHght-y, 0); imgSrc.SetRGB(x, iHght-y, 255, 255, 255);
            imgSrc.SetAlpha(iWdth-x, iHght-y, 0); imgSrc.SetRGB(iWdth-x, iHght-y, 255, 255, 255);
        }
    }
    int iXFrom[4]={6, 4, 2, 1}, iXTo[4]={5, 3, 2, 1};
    int iYFrom[4]={1, 2, 4, 6}, iYTo[4]={1, 2, 3, 5};
    int xT, yT, xF, yF;
    for (int i=0; i<4; i++)
    {
        xF=iXFrom[i]; yF=iYFrom[i]; xT=iXTo[i]; yT=iYTo[i];
        imgSrc.SetRGB(xT, yT, imgSrc.GetRed(xF, yF), imgSrc.GetGreen(xF, yF), imgSrc.GetBlue(xF, yF));
        xF=iWdth-iXFrom[i]; xT=iWdth-iXTo[i];
        imgSrc.SetRGB(xT, yT, imgSrc.GetRed(xF, yF), imgSrc.GetGreen(xF, yF), imgSrc.GetBlue(xF, yF));
        yF=iHght-iYFrom[i]; yT=iHght-iYTo[i];
        imgSrc.SetRGB(xT, yT, imgSrc.GetRed(xF, yF), imgSrc.GetGreen(xF, yF), imgSrc.GetBlue(xF, yF));
        xF=iXFrom[i]; xT=iXTo[i];
        imgSrc.SetRGB(xT, yT, imgSrc.GetRed(xF, yF), imgSrc.GetGreen(xF, yF), imgSrc.GetBlue(xF, yF));
    }

    if (m_chkIncrease->IsChecked())
    {
        int iInc=m_spnIncrease->GetValue();
        szImg.IncBy(iInc*2);
        // Set mask color to white (for images formats that don't support alpha)
        imgSrc.SetMaskColour(255, 255, 255);
        imgSrc.Resize(szImg, wxPoint(iInc, iInc));
    }

    SrcFName.SetName(SrcFName.GetName()+_T("_Cleaned"));
    imgSrc.SaveFile(SrcFName.GetFullPath(), iSrcType);
    wxMessageBox(_T("Done !"));
}
