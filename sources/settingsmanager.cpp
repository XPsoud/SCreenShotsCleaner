#include "settingsmanager.h"

#include "profilesmanager.h"

#include <wx/dir.h>
#include <wx/zstream.h>
#include <wx/xml/xml.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

SettingsManager SettingsManager::m_instance;

SettingsManager::SettingsManager()
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Creating a \"SettingsManager\" object\n"));
#endif // SHOW_DEBUG_MSG
	m_bInitialized=false;
	m_locale=NULL;
	m_bModified=false;
}

SettingsManager::~SettingsManager()
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Destroying a \"SettingsManager\" object\n"));
#endif // SHOW_DEBUG_MSG
	wxDELETE(m_locale);
}

SettingsManager& SettingsManager::Get()
{
	if (!m_instance.m_bInitialized)
	{
		m_instance.Initialize();
	}

	return m_instance;
}

void SettingsManager::Initialize()
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Initializing the SettingsManager\n"));
#endif // ENABLE_DEBUG_MSG
	// Full path of the application
	m_sAppPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
	if (!m_sAppPath.EndsWith(wxFileName::GetPathSeparator()))
		m_sAppPath.Append(wxFileName::GetPathSeparator());
#ifndef __WXMSW__
    m_sLngPath=wxStandardPaths::Get().GetResourcesDir();
	if (!m_sLngPath.EndsWith(wxFileName::GetPathSeparator()))
		m_sLngPath.Append(wxFileName::GetPathSeparator());
	m_sSettingsPath=wxStandardPaths::Get().GetUserDataDir();
    if (!m_sSettingsPath.EndsWith(wxFileName::GetPathSeparator()))
        m_sSettingsPath.Append(wxFileName::GetPathSeparator());
#else
    m_sLngPath=m_sAppPath;
	m_sSettingsPath=m_sAppPath;
#endif // ndef __WXMSW__

#ifdef __WXGTK__
	if (!wxDirExists(m_sLngPath)) // Not a real installation ?
	{
		m_sLngPath=m_sAppPath;
		m_sSettingsPath=m_sAppPath;
	}
	wxPrintf(_T("Languages path = %s\n"), m_sLngPath);
	wxPrintf(_T("Settings path  = %s\n"), m_sSettingsPath);
#endif // __WXGTK__
	m_sLngPath.Append(_T("langs"));
	// Set default language
	SetLanguage();
	// Default position and size fo the main window
	m_ptLastPos=wxDefaultPosition;
	m_szLastSize=MAINFRAME_MIN_SIZE;
	// Last Selected profile
	m_iLastProfile=0;
	// Other default settings

	m_bInitialized=true;
}

bool SettingsManager::IsModified()
{
	return m_bModified;
}

bool SettingsManager::ReadSettings()
{
#ifdef NO_SETTINGS_FILE_COMPRESSION
	wxString sFName=m_sSettingsPath + _T("Settings.xml");
#else
	wxString sFName=m_sSettingsPath + _T("Settings.zml");
#endif // NO_SETTINGS_FILE_COMPRESSION

	if (!wxFileExists(sFName)) return false;

	wxXmlDocument doc;
	wxXmlNode *root, *node;

	wxFileInputStream f_in(sFName);
	if (!f_in.IsOk()) return false;

#ifdef NO_SETTINGS_FILE_COMPRESSION
	if (!doc.Load(f_in)) return false;
#else
	wxZlibInputStream z_in(f_in);
	if (!doc.Load(z_in)) return false;
#endif // NO_SETTINGS_FILE_COMPRESSION

	root=doc.GetRoot();
	node=root->GetChildren();

	wxString nodName, sValue;
	long lVal;
	int iNewLang=m_iLangIndex;

	while(node)
	{
		nodName=node->GetName();

		if (nodName==_T("Lang")) // Language for the GUI strings
		{
			if (node->HasAttribute(_T("Code")))
			{
				sValue=node->GetAttribute(_T("Code"), _T("en"));
				const wxLanguageInfo* inf=wxLocale::FindLanguageInfo(sValue);
				if (inf!=NULL)
				{
					lVal=inf->Language;
				} else {
					lVal=wxLANGUAGE_DEFAULT;
				}
			} else {
				sValue=node->GetAttribute(_T("Id"), wxString::Format(_T("%0d"), wxLANGUAGE_DEFAULT));
				sValue.ToLong(&lVal);
			}
			iNewLang=lVal;
			// Do we have to set the language ?
			if (iNewLang!=m_iLangIndex)
			{
				SetLanguage(iNewLang);
			}
		}
		if (nodName==_T("WindowRect")) // Last known position of the main window
		{
			node->GetAttribute(_T("Left"), wxString::Format(_T("%0d"), m_ptLastPos.x)).ToLong(&lVal); m_ptLastPos.x=lVal;
			node->GetAttribute(_T("Top"), wxString::Format(_T("%0d"), m_ptLastPos.y)).ToLong(&lVal); m_ptLastPos.y=lVal;
			node->GetAttribute(_T("Width"), wxString::Format(_T("%0d"), m_szLastSize.GetWidth())).ToLong(&lVal); m_szLastSize.SetWidth(lVal);
			node->GetAttribute(_T("Height"), wxString::Format(_T("%0d"), m_szLastSize.GetHeight())).ToLong(&lVal); m_szLastSize.SetHeight(lVal);
		}
		if (nodName==_T("Profiles")) // Custom screenshots profiles
		{
			ProfilesManager& prfMngr=ProfilesManager::Get();
			prfMngr.ReadFromXmlNode(node);
		}
		if (nodName==_T("SelectedProfile"))
		{
			node->GetNodeContent().ToLong(&lVal);
			m_iLastProfile=lVal;
		}
		node = node->GetNext();
	}

	m_bModified=false;
	return true;
}

bool SettingsManager::SaveSettings()
{
#ifdef NO_SETTINGS_FILE_COMPRESSION
	wxString sFName=m_sSettingsPath + _T("Settings.xml");
#else
	wxString sFName=m_sSettingsPath + _T("Settings.zml");
#endif // NO_SETTINGS_FILE_COMPRESSION

	wxXmlNode *root = new wxXmlNode(NULL,wxXML_ELEMENT_NODE, _T("ScreenShotCleaner_Settings-file"), wxEmptyString,
							new wxXmlAttribute(_T("Version"),_T("1.0")));

	wxXmlNode *node;
	// Last known position and size of the main window
	root->AddChild(node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("WindowRect")));
		node->AddAttribute(_T("Left"), wxString::Format(_T("%0d"), m_ptLastPos.x));
		node->AddAttribute(_T("Top"), wxString::Format(_T("%0d"), m_ptLastPos.y));
		node->AddAttribute(_T("Width"), wxString::Format(_T("%0d"), m_szLastSize.GetWidth()));
		node->AddAttribute(_T("Height"), wxString::Format(_T("%0d"), m_szLastSize.GetHeight()));
	// Language for the interface
	node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Lang")));
	node=node->GetNext();
	node->AddAttribute(_T("Code"), wxLocale::GetLanguageCanonicalName(m_iLangIndex));
	node->AddAttribute(_T("Name"), wxLocale::GetLanguageName(m_iLangIndex));
	// Custom screenshots profiles
	ProfilesManager& prfMngr=ProfilesManager::Get();

	if (prfMngr.HasNonEmbeddedProfiles())
	{
		node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Profiles")));
		node=node->GetNext();
		prfMngr.SaveToXmlNode(node);
	}
	// Last selected profile
	node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("SelectedProfile")));
	node=node->GetNext();
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), wxString::Format(_T("%d"), m_iLastProfile)));

	wxXmlDocument doc;
	doc.SetRoot(root);

	wxFileName fname(sFName);

	//Check if the folder exists
    if (!fname.DirExists())
    {
        // Try to create the folder
        if (!fname.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
            return false;
    }
	if (fname.FileExists())
	{
		if (!wxRemoveFile(fname.GetFullPath()))
			return false;
	}

	wxFileOutputStream f_out(fname.GetFullPath());
	m_bModified=false;
#ifdef NO_SETTINGS_FILE_COMPRESSION
	return doc.Save(f_out);
#else
	wxZlibOutputStream z_out(f_out, 9);
	return doc.Save(z_out);
#endif // NO_SETTINGS_FILE_COMPRESSION
}

wxArrayString SettingsManager::GetAvailableLanguages()
{
	wxArrayString langs;
	// Default Language : english
	langs.Add(_T("en"));
	// Get all the files named "ScreenShotCleaner.mo" in the "langs" sub-folders
	wxDir::GetAllFiles(m_sLngPath, &langs, _T("ScreenShotCleaner.mo"), wxDIR_FILES|wxDIR_DIRS);
	// Keep only the folders names
	int iCount=langs.Count();
	for (int i=0;i<iCount;i++)
	{
		if (i)
		{
			wxFileName fname(langs[i]);
			fname.MakeRelativeTo(m_sLngPath);
			langs[i]=fname.GetPath();
		}
		const wxLanguageInfo* info=wxLocale::FindLanguageInfo(langs[i]);
		langs[i]=wxString::Format(_T("%05d%s"), info->Language, info->Description.c_str());
	}
	return langs;
}

void SettingsManager::SetLanguage(int lang)
{
	if (m_locale!=NULL)
	{
		wxDELETE(m_locale);
	}

	m_locale=new wxLocale();
	m_locale->AddCatalogLookupPathPrefix(m_sLngPath);
	int lng;
	if (lang==wxLANGUAGE_DEFAULT)
	{
		lng=wxLocale::GetSystemLanguage();
	} else {
		lng=lang;
	}
	if (lng!=m_iLangIndex)
	{
		m_bModified=true;
	}
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("\tSetting Language to id= %d Name = %s\n"), lng, wxLocale::GetLanguageCanonicalName(lng).c_str());
#endif // SHOW_DEBUG_MSG
	if (lng!=wxLANGUAGE_ENGLISH)
	{
		if (m_locale->Init(lng, wxLOCALE_LOAD_DEFAULT))
		{
			m_locale->AddCatalog(_T("ScreenShotCleaner"));
#ifdef __WXGTK__
        	m_locale->AddCatalog(_T("wxStd")); // Don't know yet why it isn't loaded automatically :-/
#endif // __WXGTK__
		}
		else
		{
			if (lang!=wxLANGUAGE_DEFAULT)
			{
				wxMessageBox(_("The selected language isn't supported yet !"), _("Error"), wxICON_EXCLAMATION);
			}
		}
		if (lng!=wxLANGUAGE_DEFAULT)
		{
			m_iLangIndex=m_locale->GetLanguage();
		}
	} else {
		m_iLangIndex=wxLANGUAGE_ENGLISH;
#ifndef __WXGTK__
		m_locale->Init(wxLANGUAGE_ENGLISH, wxLOCALE_LOAD_DEFAULT);
#endif // ndef __WXGTK__
	}
}

void SettingsManager::SetLastWindowRect(const wxPoint& pos, const wxSize& size)
{
	if ((pos!=m_ptLastPos)||(size!=m_szLastSize))
	{
		m_bModified=true;
		m_ptLastPos=pos;
		m_szLastSize=size;
	}
}

void SettingsManager::SetLastSelectedProfile(int profile)
{
	if (profile!=m_iLastProfile)
	{
		m_iLastProfile=profile;
		m_bModified=true;
	}
}

int SettingsManager::GetLastSelectedProfile()
{
	return m_iLastProfile;
}
