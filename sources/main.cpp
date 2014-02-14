#include "main.h"

#include "mainframe.h"
#include "appversion.h"
#include "profilesmanager.h"
#include "settingsmanager.h"
#include "myfiledroptarget.h"

IMPLEMENT_APP(ScreenShotCleanerApp)

bool ScreenShotCleanerApp::OnInit()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Initializing a \"wxApp\" derived object\n"));
#endif // SHOW_DEBUG_MSG

    wxInitAllImageHandlers();

    // Init the SettingManager instance
    SettingsManager& settings=SettingsManager::Get();
    // Read the settings file
    settings.ReadSettings();
    // Init the ProfilesManager instance if not done by ReadSettings
    ProfilesManager::Get();

    // If a file has been passed by command line
	// Do as if it has been dropped on the source textbox
	wxString sFile=wxEmptyString;
	if (argc>1)
    {
        sFile=argv[1];
        if (!wxFileExists(sFile))
        {
            wxMessageBox(_("Invalid source file name !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
            return false;
        }
        if (!wxImage::CanRead(sFile))
        {
            wxMessageBox(_("Unsupported image format !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
            return false;
        }
    }

	// Main window creation
	wxString sTitle=_T(PRODUCTNAME);
	sTitle << _T(" ") << _T("(v") << VERSION_MAJOR << _T(".") << VERSION_MINOR << _T(") by X@v'");

	MainFrame *frame = new MainFrame(NULL, sTitle);

	if (!sFile.IsEmpty())
    {
        wxCommandEvent evt(wxEVT_FILE_DROPPED, wxID_ANY);
        evt.SetString(sFile);
        frame->GetEventHandler()->AddPendingEvent(evt);
    }

	// Show the main window
	frame->Show();

    return true;
}

int ScreenShotCleanerApp::OnExit()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Exiting from a \"wxApp\" derived object\n"));
#endif // SHOW_DEBUG_MSG

    // Save the settings if they have been modified
    SettingsManager& settings=SettingsManager::Get();
    if (settings.IsModified()) settings.SaveSettings();

    return wxApp::OnExit();
}
