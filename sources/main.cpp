#include "main.h"

#include "mainframe.h"
#include "appversion.h"
#include "settingsmanager.h"

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

	// Main window creation
	wxString sTitle=_T(PRODUCTNAME);
	sTitle << _T(" ") << _T("(v") << VERSION_MAJOR << _T(".") << VERSION_MINOR << _T(") by X@v'");

	MainFrame *frame = new MainFrame(NULL, sTitle);

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
