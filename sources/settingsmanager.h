#ifndef __SETTINGSMANAGER_H_INCLUDED__
#define __SETTINGSMANAGER_H_INCLUDED__

#include <wx/wx.h>

#define NO_SETTINGS_FILE_COMPRESSION
#define MAINFRAME_MIN_SIZE wxSize(400, 300)

class SettingsManager
{
	public:
		static SettingsManager& Get();
		bool ReadSettings();
			bool SaveSettings();
			// Accessors
			virtual bool IsModified();
			wxString GetAppPath() { return m_sAppPath; }
		// Language
		void SetLanguage(int lang=wxLANGUAGE_DEFAULT);
		int GetCurrentLanguage() { return m_iLangIndex; }
		wxArrayString GetAvailableLanguages();
		// Position and size of the main window
		const wxPoint& GetLastSavedPos() { return m_ptLastPos; }
		const wxSize& GetLastSavedSize() { return m_szLastSize; }
		void SetLastWindowRect(const wxPoint& pos, const wxSize& size);
		// Selected profile at startup
		void SetLastSelectedProfile(int profile);
		int GetLastSelectedProfile();
	protected:
	private:
		SettingsManager();
		virtual ~SettingsManager();
		void Initialize();
		// Unique instance of the singleton
		static SettingsManager m_instance;
		// Misc vars
		wxString m_sAppPath;
		bool m_bInitialized, m_bModified;
		wxLocale *m_locale;
		int m_iLangIndex;
		wxPoint m_ptLastPos;
		wxSize m_szLastSize;
		int m_iLastProfile;
};

#endif // __SETTINGSMANAGER_H_INCLUDED__
