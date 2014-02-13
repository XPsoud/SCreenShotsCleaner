#include "profile.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(ProfilesList);

#include <wx/xml/xml.h>

PrfPoint::PrfPoint(wxXmlNode* container)
{
    Initialize();

    if (container!=NULL)
        FromXmlNode(container);
}

PrfPoint::~PrfPoint()
{
    //
}

void PrfPoint::Initialize()
{
    m_lX=0; m_lRefX=wxCENTER;
    m_lY=0; m_lRefY=wxCENTER;
    m_bCopy=false;
    m_lXFrom=0; m_lRefXFrom=wxCENTER;
    m_lYFrom=0; m_lRefYFrom=wxCENTER;
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
        container->GetAttribute(_T("RefX"), _T("0")).ToLong(&m_lRefX);
    }
    if (container->HasAttribute(_T("Y")))
    {
        container->GetAttribute(_T("Y"), _T("0")).ToLong(&m_lY);
    }
    if (container->HasAttribute(_T("RefY")))
    {
        container->GetAttribute(_T("RefY"), _T("0")).ToLong(&m_lRefY);
    }
    if (container->HasAttribute(_T("Copy")))
    {
        m_bCopy=(container->GetAttribute(_T("Copy"), _T("No"))==_T("Yes"));
    }
    if (container->HasAttribute(_T("XFrom")))
    {
        container->GetAttribute(_T("XFrom"), _T("0")).ToLong(&m_lXFrom);
    }
    if (container->HasAttribute(_T("RefXFrom")))
    {
        container->GetAttribute(_T("RefXFrom"), _T("0")).ToLong(&m_lRefXFrom);
    }
    if (container->HasAttribute(_T("YFrom")))
    {
        container->GetAttribute(_T("YFrom"), _T("0")).ToLong(&m_lYFrom);
    }
    if (container->HasAttribute(_T("RefYFrom")))
    {
        container->GetAttribute(_T("RefYFrom"), _T("0")).ToLong(&m_lRefYFrom);
    }
}

void PrfPoint::ToXmlNode(wxXmlNode* container)
{
    if (container==NULL) return;

    container->AddAttribute(_T("X"), wxString::Format(_T("%d"), m_lX));
    container->AddAttribute(_T("RefX"), wxString::Format(_T("%d"), m_lRefX));
    container->AddAttribute(_T("Y"), wxString::Format(_T("%d"), m_lY));
    container->AddAttribute(_T("RefY"), wxString::Format(_T("%d"), m_lRefY));
    container->AddAttribute(_T("Copy"), (m_bCopy?_T("Yes"):_T("No")));

    if (!m_bCopy) return;

    container->AddAttribute(_T("XFrom"), wxString::Format(_T("%d"), m_lXFrom));
    container->AddAttribute(_T("RefXFrom"), wxString::Format(_T("%d"), m_lRefXFrom));
    container->AddAttribute(_T("YFrom"), wxString::Format(_T("%d"), m_lYFrom));
    container->AddAttribute(_T("RefYFrom"), wxString::Format(_T("%d"), m_lRefYFrom));
}


int Profile::m_iItemsCount = 0;

Profile::Profile(wxXmlNode* container)
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Creating a \"Profile\" object\n"));
#endif // SHOW_DEBUG_MSG
    m_iItemsCount++;
    // Default values
    m_sName.Printf(_T("Profile #%d"), m_iItemsCount);

    m_bModified=true;

    if (container!=NULL)
        FromXmlNode(container);
}

Profile::~Profile()
{
#ifdef SHOW_DEBUG_MSG
    wxPrintf(_T("Destroying a \"Profile\" object\n"));
#endif // SHOW_DEBUG_MSG
    m_iItemsCount--;
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

    if (container->HasAttribute(_T("Name")))
        m_sName=container->GetAttribute(_T("Name"), wxEmptyString);

    m_bModified=false;
}

void Profile::ToXmlNode(wxXmlNode* container)
{
    if (container==NULL) return;

    container->AddAttribute(_T("Name"), m_sName);

    m_bModified=false;
}
