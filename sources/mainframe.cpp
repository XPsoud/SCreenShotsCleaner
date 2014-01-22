#include "mainframe.h"

#include "dlgabout.h"
#include "dlgoptions.h"
#include "menu_icons.h"
#include "settingsmanager.h"

MainFrame::MainFrame(wxWindow *parent, const wxString& title) :
    wxFrame(parent, wxID_ANY, title), m_options(SettingsManager::Get())
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Creating a \"MainFrame\" object\n"));
#endif // SHOW_DEBUG_MSG

    SetIcon(wxICON(appicon));

    CreateControls();
    ConnectControls();

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
    // Status Bar
    CreateStatusBar();
    // Menu Bar
    wxMenuItem *item;
    wxMenu *menu;
    wxMenuBar* mbar = new wxMenuBar();

    menu = new wxMenu(_T(""));
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
}

void MainFrame::ConnectControls()
{
    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrame::OnClose));
    Connect(wxEVT_SIZE, wxSizeEventHandler(MainFrame::OnSize));
    Connect(wxEVT_MOVE, wxMoveEventHandler(MainFrame::OnMove));

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuExitClicked));
    Connect(wxID_PREFERENCES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuOptionsClicked));
    Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuAboutClicked));
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

void MainFrame::OnMenuExitClicked(wxCommandEvent& event)
{
    Destroy();
}

void MainFrame::OnMenuAboutClicked(wxCommandEvent& event)
{
    DlgAbout dlg(this);
    dlg.ShowModal();
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
