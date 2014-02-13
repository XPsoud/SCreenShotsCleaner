#include "profilesmanager.h"

#include <wx/xml/xml.h>

ProfilesManager ProfilesManager::m_instance;

ProfilesManager::ProfilesManager()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Creating a \"ProfilesManager\" object\n"));
#endif // SHOW_DEBUG_MSG
    m_bInitialized=false;
    m_bModified=false;

    m_profiles.Clear();
}

ProfilesManager::~ProfilesManager()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Destroying a \"ProfilesManager\" object\n"));
#endif // SHOW_DEBUG_MSG

    Clear();
}

ProfilesManager& ProfilesManager::Get()
{
    if (!m_instance.m_bInitialized)
    {
        m_instance.Initialize();
    }

    return m_instance;
}

void ProfilesManager::Initialize()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Initializing the ProfilesManager\n"));
#endif // ENABLE_DEBUG_MSG
    // Be sure that nothing remains in the Profile list
    Clear();

    m_bInitialized=true;
}

void ProfilesManager::Clear()
{
    m_profiles.DeleteContents(true);
    m_profiles.Clear();
}


bool ProfilesManager::IsModified()
{
    return m_bModified;
}

bool ProfilesManager::HasNonEmbeddedProfiles()
{
    ProfilesList::iterator iter;

    for (iter=m_profiles.begin(); iter!=m_profiles.end(); ++iter)
    {
        Profile* item=*iter;
        if (!item->IsEmbedded()) return true;
    }

    return false;
}

void ProfilesManager::CreateEmbeddedProfiles()
{
    // If the profiles list isn't empty, the embedded profiles has been created yet
    if (!m_profiles.IsEmpty()) return;

    // Windows 7 Default Aero Theme
    // Windows XP Default Theme
}

void ProfilesManager::SaveToXmlNode(wxXmlNode* container)
{
    ProfilesList::iterator iter;

    wxXmlNode *node=NULL;

    for (iter=m_profiles.begin(); iter!=m_profiles.end(); ++iter)
    {
        Profile* item=*iter;
        if (!item->IsEmbedded())
        {
            if (node==NULL)
            {
                container->AddChild(node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Profile")));
            }
            else
            {
                node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Profile")));
                node=node->GetNext();
            }
            item->ToXmlNode(node);
        }
    }
    m_bModified=false;
}

void ProfilesManager::ReadFromXmlNode(wxXmlNode* container)
{
    Clear();
    CreateEmbeddedProfiles();

    wxXmlNode *node=container->GetChildren();

    wxString sNodName;

    while(node!=NULL)
    {
        sNodName=node->GetName();
        if (sNodName==_T("Profile"))
        {
            Profile* item=new Profile(node);
            m_profiles.Append(item);
        }
        node=node->GetNext();
    }
}

int ProfilesManager::GetProfilesCount()
{
    return m_profiles.GetCount();
}

Profile* ProfilesManager::CreateNewProfile()
{
    Profile* item=new Profile();
    m_profiles.Append(item);

    return item;
}

Profile* ProfilesManager::GetProfile(int prfIndex)
{
    // Check for an invalid index
    if ((prfIndex<0) || (prfIndex>GetProfilesCount()))
        return NULL;

    // Get the corresponding profile
    wxProfilesListNode* node=m_profiles.Item(prfIndex);
    if (node)
    {
        Profile* item=node->GetData();
        return item;
    }
    // If we get here, the profile has not been found
    return NULL;
}

Profile* ProfilesManager::GetProfile(const wxString& prfName)
{
    // Can't return a profiles if no name supplied
    if (prfName.IsEmpty()) return NULL;

    ProfilesList::iterator iter;

    // Try to find the profile in the list
    for (iter=m_profiles.begin(); iter!=m_profiles.end(); ++iter)
    {
        Profile* item=*iter;
        if (item->GetName()==prfName)
            return item;
    }
    // If we get here, the profile has bnot been found
    return NULL;
}
