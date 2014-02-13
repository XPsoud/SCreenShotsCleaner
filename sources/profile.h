#ifndef __PROFILE_H_INCLUDED__
#define __PROFILE_H_INCLUDED__

#include <wx/wx.h>

class wxXmlNode;
#include "prfpoint.h"

class Profile
{
    public:
        Profile(wxXmlNode* container=NULL);
        virtual ~Profile();
        void Clear();
        void FromXmlNode(wxXmlNode* container);
        void ToXmlNode(wxXmlNode* container);
        bool IsModified() { return m_bModified; }
        bool IsEmbedded() { return m_bEmbedded; }
        const wxString& GetLastError() { return m_sLastError; }
        void SetName(const wxString& name);
        const wxString& GetName() const { return m_sName; }
        bool Apply(wxImage& image);
        // Points management
        int GetPointsCount();
        PrfPoint* AddNewPoint();
    protected:
    private:
        //
        static int m_iItemsCount;
        bool m_bModified, m_bEmbedded;
        wxString m_sName, m_sLastError;
        PointsList m_Points;
};

#include <wx/list.h>
WX_DECLARE_LIST(Profile, ProfilesList);

#endif // __PROFILE_H_INCLUDED__
