#include "settingsmanager.h"

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
    // Set default language
    SetLanguage();
    // Default position and size fo the main window
    m_ptLastPos=wxDefaultPosition;
    m_szLastSize=MAINFRAME_MIN_SIZE;
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
    wxString sFName=m_sAppPath + _T("Settings.xml");
#else
    wxString sFName=m_sAppPath + _T("Settings.zml");
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
        }
        if (nodName==_T("WindowRect")) // Last known position of the main window
        {
            node->GetAttribute(_T("Left"), wxString::Format(_T("%0d"), m_ptLastPos.x)).ToLong(&lVal); m_ptLastPos.x=lVal;
            node->GetAttribute(_T("Top"), wxString::Format(_T("%0d"), m_ptLastPos.y)).ToLong(&lVal); m_ptLastPos.y=lVal;
            node->GetAttribute(_T("Width"), wxString::Format(_T("%0d"), m_szLastSize.GetWidth())).ToLong(&lVal); m_szLastSize.SetWidth(lVal);
            node->GetAttribute(_T("Height"), wxString::Format(_T("%0d"), m_szLastSize.GetHeight())).ToLong(&lVal); m_szLastSize.SetHeight(lVal);
        }
    	node = node->GetNext();
    }
    // Do we have to set the language ?
    if (iNewLang!=m_iLangIndex)
    {
        SetLanguage(iNewLang);
    }
    m_bModified=false;
    return true;
}

bool SettingsManager::SaveSettings()
{
#ifdef NO_SETTINGS_FILE_COMPRESSION
    wxString sFName=m_sAppPath + _T("Settings.xml");
#else
    wxString sFName=m_sAppPath + _T("Settings.zml");
#endif // NO_SETTINGS_FILE_COMPRESSION

    wxXmlNode *root = new wxXmlNode(NULL,wxXML_ELEMENT_NODE, _T("ScreenShotCleaner_Settings-file"), wxEmptyString,
                            new wxXmlProperty(_T("Version"),_T("1.0")));

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

    wxXmlDocument doc;
    doc.SetRoot(root);

    wxFileName fname(sFName);

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
    wxString sDir=m_sAppPath + _T("langs");
    // Default Language : english
    langs.Add(_T("en"));
    // Get all the files named "ScreenShotCleaner.mo" in the "langs" sub-folders
    wxDir::GetAllFiles(sDir, &langs, _T("ScreenShotCleaner.mo"), wxDIR_FILES|wxDIR_DIRS);
    // Keep only the folders names
    int iCount=langs.Count();
    for (int i=0;i<iCount;i++)
    {
        if (i)
        {
            wxFileName fname(langs[i]);
            fname.MakeRelativeTo(sDir);
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

    m_locale->AddCatalogLookupPathPrefix(m_sAppPath + _T("langs"));
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
        if (m_locale->Init(lng, wxLOCALE_DONT_LOAD_DEFAULT))
        {
            m_locale->AddCatalog(_T("ScreenShotCleaner"));
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
        m_locale->Init(wxLANGUAGE_ENGLISH, wxLOCALE_DONT_LOAD_DEFAULT);
    }
    setlocale(LC_NUMERIC,"C");
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
