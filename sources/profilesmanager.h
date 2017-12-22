#ifndef __PROFILESMANAGER_H_INCLUDED__
#define __PROFILESMANAGER_H_INCLUDED__

#include <wx/wx.h>

#include "profile.h"
class wxXmlNode;

class ProfilesManager
{
	public:
		static ProfilesManager& Get();
		void SaveToXmlNode(wxXmlNode* container);
		void ReadFromXmlNode(wxXmlNode* container);
		// Accessors
		virtual bool IsModified();
		bool HasNonEmbeddedProfiles();
		// Profiles management
		void Clear();
		int GetProfilesCount();
		Profile* CreateNewProfile();
		Profile* GetProfile(int prfIndex);
		Profile* GetProfile(const wxString& prfName);
	protected:
	private:
		ProfilesManager();
		virtual ~ProfilesManager();
		void Initialize();
		void CreateEmbeddedProfiles();
		// Unique instance of the singleton
		static ProfilesManager m_instance;
		// Misc vars
		bool m_bInitialized, m_bModified;
		ProfilesList m_profiles;
};

#endif // __PROFILESMANAGER_H_INCLUDED__
