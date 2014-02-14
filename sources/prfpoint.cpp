#include "prfpoint.h"

#include <wx/xml/xml.h>
#include <wx/listimpl.cpp>
WX_DEFINE_LIST(PointsList);

const wxEdge PrfPoint::m_iEdge[KnownEdgesCount]={wxCenter, wxLeft, wxRight, wxTop, wxBottom};
const wxString PrfPoint::m_sEdge[KnownEdgesCount]={_T("Center"), _T("Left"), _T("Right"), _T("Top"), _T("Bottom")};

PrfPoint::PrfPoint(wxXmlNode* container)
{
    Initialize();

    if (container!=NULL)
        FromXmlNode(container);
}

PrfPoint::PrfPoint(long X, wxEdge refX, long Y, wxEdge refY)
{
    Initialize();
    m_lX=X; m_iRefX=refX;
    m_lY=Y; m_iRefY=refY;
    m_bCopy=false;
}

PrfPoint::PrfPoint(long X, wxEdge refX, long Y, wxEdge refY, long XF, wxEdge refXF, long YF, wxEdge refYF)
{
    Initialize();
    m_lX=X; m_iRefX=refX;
    m_lY=Y; m_iRefY=refY;
    m_bCopy=true;
    m_lXFrom=XF; m_iRefXFrom=refXF;
    m_lYFrom=YF; m_iRefYFrom=refYF;
}

PrfPoint::~PrfPoint()
{
    //
}

void PrfPoint::Initialize()
{
    m_lX=0; m_iRefX=wxCenter;
    m_lY=0; m_iRefY=wxCenter;
    m_bCopy=false;
    m_bMirrorH=true;
    m_bMirrorV=false;
    m_lXFrom=0; m_iRefXFrom=wxCenter;
    m_lYFrom=0; m_iRefYFrom=wxCenter;
}

void PrfPoint::UnsetCopyPoint()
{
    m_bCopy=false;
}

void PrfPoint::SetPoint(long X, wxEdge refX, long Y, wxEdge refY)
{
    m_lX=X; m_iRefX=refX;
    m_lY=Y; m_iRefY=refY;
}

void PrfPoint::SetCopyPoint(long XF, wxEdge refXF, long YF, wxEdge refYF)
{
    m_bCopy=true;
    m_lXFrom=XF; m_iRefXFrom=refXF;
    m_lYFrom=YF; m_iRefYFrom=refYF;
}

void PrfPoint::SetMirrors(bool hMirror, bool vMirror)
{
    m_bMirrorH=hMirror;
    m_bMirrorV=vMirror;
}

void PrfPoint::SetMirrorH(bool value)
{
    m_bMirrorH=value;
}

void PrfPoint::SetMirrorV(bool value)
{
    m_bMirrorV=value;
}

const wxString PrfPoint::wxEdge2String(wxEdge value)
{
    for (int i=0; i<KnownEdgesCount; i++)
    {
        if (value==m_iEdge[i]) return m_sEdge[i];
    }
    return m_sEdge[0];
}

wxEdge PrfPoint::wxString2Edge(const wxString& value)
{
    for (int i=0; i<KnownEdgesCount; i++)
    {
        if (value.IsSameAs(m_sEdge[i], false)) return m_iEdge[i];
    }
    return m_iEdge[0];
}

void PrfPoint::FromXmlNode(wxXmlNode* container)
{
    if (container==NULL) return;
    if (container->HasAttribute(_T("X")))
    {
        container->GetAttribute(_T("X"), _T("0")).ToLong(&m_lX);
    }
    if (container->HasAttribute(_T("RefX")))
    {
        m_iRefX=wxString2Edge(container->GetAttribute(_T("RefX"), wxEmptyString));
    }
    if (container->HasAttribute(_T("Y")))
    {
        container->GetAttribute(_T("Y"), _T("0")).ToLong(&m_lY);
    }
    if (container->HasAttribute(_T("RefY")))
    {
        m_iRefY=wxString2Edge(container->GetAttribute(_T("RefY"), wxEmptyString));
    }
    if (container->HasAttribute(_T("Copy")))
    {
        m_bCopy=(container->GetAttribute(_T("Copy"), _T("No"))==_T("Yes"));
    }
    if (container->HasAttribute(_T("HMirror")))
    {
        m_bMirrorH=(container->GetAttribute(_T("HMirror"), _T("Yes"))==_T("Yes"));
    }
    if (container->HasAttribute(_T("VMirror")))
    {
        m_bCopy=(container->GetAttribute(_T("VMirror"), _T("No"))==_T("Yes"));
    }
    if (container->HasAttribute(_T("XFrom")))
    {
        container->GetAttribute(_T("XFrom"), _T("0")).ToLong(&m_lXFrom);
    }
    if (container->HasAttribute(_T("RefXFrom")))
    {
        m_iRefXFrom=wxString2Edge(container->GetAttribute(_T("RefXFrom"), wxEmptyString));
    }
    if (container->HasAttribute(_T("YFrom")))
    {
        container->GetAttribute(_T("YFrom"), _T("0")).ToLong(&m_lYFrom);
    }
    if (container->HasAttribute(_T("RefYFrom")))
    {
        m_iRefYFrom=wxString2Edge(container->GetAttribute(_T("RefYFrom"),wxEmptyString));
    }
}

void PrfPoint::ToXmlNode(wxXmlNode* container)
{
    if (container==NULL) return;

    container->AddAttribute(_T("X"), wxString::Format(_T("%d"), m_lX));
    container->AddAttribute(_T("RefX"), wxEdge2String(m_iRefX));
    container->AddAttribute(_T("Y"), wxString::Format(_T("%d"), m_lY));
    container->AddAttribute(_T("RefY"), wxEdge2String(m_iRefY));
    container->AddAttribute(_T("Copy"), (m_bCopy?_T("Yes"):_T("No")));

    if (!m_bCopy) return;

    container->AddAttribute(_T("XFrom"), wxString::Format(_T("%d"), m_lXFrom));
    container->AddAttribute(_T("RefXFrom"), wxEdge2String(m_iRefXFrom));
    container->AddAttribute(_T("YFrom"), wxString::Format(_T("%d"), m_lYFrom));
    container->AddAttribute(_T("RefYFrom"), wxEdge2String(m_iRefYFrom));
}

long PrfPoint::GetRealCoord(long total, long delta, wxEdge refSide)
{
    switch(refSide)
    {
        case wxLeft:
        case wxTop:
            return delta;
        case wxCenter:
            return (total/2)+delta;
        case wxRight:
        case wxBottom:
            return total+delta;
        default:
            return 0;
    }
}

long PrfPoint::GetMirroredCoord(long total, long delta, wxEdge refSide)
{
    switch(refSide)
    {
        case wxLeft:
        case wxTop:
            return total-1-delta;
        case wxCenter:
            return (total/2)-delta;
        case wxRight:
        case wxBottom:
            return -1*delta;
        default:
            return 0;
    }
}

bool PrfPoint::Apply(wxImage& image)
{
// TODO (Xaviou#1#): Check for compatibility between X, Y, XF, YF and image size
    long lWdth=image.GetWidth(), lHght=image.GetHeight();

    long X=GetRealCoord(lWdth, m_lX, m_iRefX), XF=0;
    long Y=GetRealCoord(lHght, m_lY, m_iRefY), YF=0;
    if (m_bCopy)
    {
        XF=GetRealCoord(lWdth, m_lXFrom, m_iRefXFrom);
        YF=GetRealCoord(lHght, m_lYFrom, m_iRefYFrom);
        image.SetRGB(X, Y, image.GetRed(XF, YF), image.GetGreen(XF, YF), image.GetBlue(XF, YF));
    }
    else
    {
        image.SetAlpha(X, Y, 0); image.SetRGB(X, Y, 255, 255, 255);
    }

    if (m_bMirrorH)
    {
        X=GetMirroredCoord(lWdth, m_lX, m_iRefX);
        if (m_bCopy)
        {
            XF=GetMirroredCoord(lWdth, m_lXFrom, m_iRefXFrom);
            image.SetRGB(X, Y, image.GetRed(XF, YF), image.GetGreen(XF, YF), image.GetBlue(XF, YF));
        }
        else
        {
            image.SetAlpha(X, Y, 0); image.SetRGB(X, Y, 255, 255, 255);
        }
    }
    if (m_bMirrorV)
    {
        Y=GetMirroredCoord(lHght, m_lY, m_iRefY);
        if (m_bCopy)
        {
            YF=GetMirroredCoord(lHght, m_lYFrom, m_iRefYFrom);
            image.SetRGB(X, Y, image.GetRed(XF, YF), image.GetGreen(XF, YF), image.GetBlue(XF, YF));
        }
        else
        {
            image.SetAlpha(X, Y, 0); image.SetRGB(X, Y, 255, 255, 255);
        }
    }
    if (m_bMirrorH & m_bMirrorV)
    {
        X=GetRealCoord(lWdth, m_lX, m_iRefX);
        if (m_bCopy)
        {
            XF=GetRealCoord(lWdth, m_lXFrom, m_iRefXFrom);
            image.SetRGB(X, Y, image.GetRed(XF, YF), image.GetGreen(XF, YF), image.GetBlue(XF, YF));
        }
        else
        {
            image.SetAlpha(X, Y, 0); image.SetRGB(X, Y, 255, 255, 255);
        }
    }

    return true;
}
