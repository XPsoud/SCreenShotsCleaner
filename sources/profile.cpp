#include "profile.h"

#include <wx/xml/xml.h>
#include <wx/listimpl.cpp>
WX_DEFINE_LIST(ProfilesList);

int Profile::m_iItemsCount = 0;

Profile::Profile(wxXmlNode* container)
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Creating a normal \"Profile\" object\n"));
#endif // SHOW_DEBUG_MSG

    Create();

    if (container!=NULL)
        FromXmlNode(container);
}

Profile::Profile(const wxString& name)
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Creating an embedded \"Profile\" object\n"));
#endif // SHOW_DEBUG_MSG

    Create();
    m_sName=name;
    m_bEmbedded=true;
}

Profile::~Profile()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Destroying a \"Profile\" object\n"));
#endif // SHOW_DEBUG_MSG
    Clear();

    m_iItemsCount--;
}

void Profile::Create()
{
    m_iItemsCount++;
    // Default values
    m_sName.Printf(_T("Profile #%d"), m_iItemsCount);
    m_sLastError=wxEmptyString;
    m_bEmbedded=false;

    m_bModified=true;
}

void Profile::Clear()
{
    m_Points.DeleteContents(true);
    m_Points.Clear();
}

void Profile::SetName(const wxString& name)
{
    if (name!=m_sName)
    {
        m_sName=name;
        m_bModified=true;
    }
}

void Profile::FromXmlNode(wxXmlNode* container)
{
    if (container==NULL) return;

    Clear();

    if (container->HasAttribute(_T("Name")))
        m_sName=container->GetAttribute(_T("Name"), wxEmptyString);
    wxXmlNode *node=container->GetChildren();
    wxString sNodName;
    while(node)
    {
        sNodName=node->GetName();
        if (sNodName==_T("Point"))
        {
            m_Points.Append(new PrfPoint(node));
        }
        node=node->GetNext();
    }

    m_bModified=false;
}

void Profile::ToXmlNode(wxXmlNode* container)
{
    // If the xml node isn't valid, we can't do anything
    if (container==NULL) return;
    // If no point in the list, no need to save
    if (m_Points.IsEmpty()) return;

    // Write the profile's data
    container->AddAttribute(_T("Name"), m_sName);
    container->AddAttribute(_T("PointsCount"), wxString::Format(_T("%d"), GetPointsCount()));
    // Start writing the points
    PointsList::iterator iter;
    wxXmlNode *node=NULL;
    for (iter=m_Points.begin(); iter!=m_Points.end(); ++iter)
    {
        PrfPoint *item=*iter;
        if (node==NULL)
        {
            container->AddChild(node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Point")));
        }
        else
        {
            node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Point")));
            node=node->GetNext();
        }
        item->ToXmlNode(node);
    }

    m_bModified=false;
}

int Profile::GetPointsCount()
{
    return m_Points.GetCount();
}

PrfPoint* Profile::AddNewPoint()
{
    PrfPoint *item=new PrfPoint();

    m_Points.Append(item);
    m_bModified=true;

    return item;
}

PrfPoint* Profile::AddNewPoint(long X, wxEdge refX, long Y, wxEdge refY)
{
    PrfPoint *item=new PrfPoint(X, refX, Y, refY);

    m_Points.Append(item);
    m_bModified=true;

    return item;
}

bool Profile::Apply(wxImage& image)
{
    if (m_Points.IsEmpty())
    {
        m_sLastError=_("Profile doesn't contain any point !");
        return false;
    }
    if (!image.IsOk())
    {
        m_sLastError=_("Invalid image !");
        return false;
    }
    PointsList::iterator iter;
    bool bRes=true;
    int iPoint=0;
    m_sLastError=wxEmptyString;
    for (iter=m_Points.begin(); iter!=m_Points.end(); ++iter)
    {
        PrfPoint* item=*iter;
        if (!item->Apply(image))
        {
            if (!m_sLastError.IsEmpty()) m_sLastError << _T("\n");
            m_sLastError << _("Point #") << iPoint << _T(" : ") << item->GetLastError();
            bRes=false;
        }
        iPoint++;
    }
    return bRes;
}
