#ifndef __PROFILE_H_INCLUDED__
#define __PROFILE_H_INCLUDED__

#include <wx/wx.h>

class wxXmlNode;

class PrfPoint
{
    public:
        PrfPoint(wxXmlNode* container=NULL);
        virtual ~PrfPoint();
        void FromXmlNode(wxXmlNode* container);
        void ToXmlNode(wxXmlNode* container);
    protected:
    private:
        void Initialize();
        bool m_bCopy;
        long m_lX, m_lY, m_lRefX, m_lRefY;
        long m_lXFrom, m_lYFrom, m_lRefXFrom, m_lRefYFrom;
};

class Profile
{
    public:
        Profile(wxXmlNode* container=NULL);
        virtual ~Profile();
        void FromXmlNode(wxXmlNode* container);
        void ToXmlNode(wxXmlNode* container);
        bool IsModified() { return m_bModified; }
        bool IsEmbedded() { return m_bEmbedded; }
        void SetName(const wxString& name);
        const wxString& GetName() const { return m_sName; }
    protected:
    private:
        //
        static int m_iItemsCount;
        bool m_bModified, m_bEmbedded;
        wxString m_sName;
};

#include <wx/list.h>
WX_DECLARE_LIST(Profile, ProfilesList);
WX_DECLARE_LIST(PrfPoint, PointsList);

#endif // __PROFILE_H_INCLUDED__
