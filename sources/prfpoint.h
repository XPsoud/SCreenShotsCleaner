#ifndef __PRFPOINT_H_INCLUDED__
#define __PRFPOINT_H_INCLUDED__

#include <wx/wx.h>
class wxXmlNode;

#define KnownEdgesCount 5

class PrfPoint
{
    public:
        PrfPoint(wxXmlNode* container=NULL);
        PrfPoint(long X, wxEdge refX, long Y, wxEdge refY);
        PrfPoint(long X, wxEdge refX, long Y, wxEdge refY, long XF, wxEdge refXF, long YF, wxEdge refYF);
        virtual ~PrfPoint();
        void UnsetCopyPoint();
        void SetPoint(long X, wxEdge refX, long Y, wxEdge refY);
        void SetCopyPoint(long XF, wxEdge refXF, long YF, wxEdge refYF);
        void SetMirrorH(bool value=true);
        void SetMirrorV(bool value=true);
        void FromXmlNode(wxXmlNode* container);
        void ToXmlNode(wxXmlNode* container);
        const wxString& GetLastError() { return m_sLastError; }
        bool Apply(wxImage& image);
    protected:
    private:
        void Initialize();
        long GetRealCoord(long total, long delta, wxEdge refSide);
        long GetMirroredCoord(long total, long delta, wxEdge refSide);
        static const wxEdge m_iEdge[KnownEdgesCount];
        static const wxString m_sEdge[KnownEdgesCount];
        const wxString wxEdge2String(wxEdge value);
        wxEdge wxString2Edge(const wxString& value);
        wxString m_sLastError;
        bool m_bCopy, m_bMirrorH, m_bMirrorV;
        long m_lX, m_lY, m_lXFrom, m_lYFrom;
        wxEdge m_iRefX, m_iRefY, m_iRefXFrom, m_iRefYFrom;
};

#include <wx/list.h>
WX_DECLARE_LIST(PrfPoint, PointsList);

#endif // __PRFPOINT_H_INCLUDED__
