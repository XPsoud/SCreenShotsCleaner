#include "myfiledroptarget.h"

#include <wx/filename.h>

wxDEFINE_EVENT(wxEVT_FILE_DROPPED, wxCommandEvent);

MyFileDropTarget::MyFileDropTarget(wxWindow* parent)
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Creating a \"MyFileDropTarget\" object\n"));
#endif // SHOW_DEBUG_MSG
	m_parent=parent;
}

MyFileDropTarget::~MyFileDropTarget()
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Destroying a \"MyFileDropTarget\" object\n"));
#endif // SHOW_DEBUG_MSG
}

bool MyFileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames)
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("%d file(s) dropped\n"), filenames.GetCount());
#endif // SHOW_DEBUG_MSG

	// We only accept one file at a time
	// So we take the first element of the array
	if (wxImage::CanRead(filenames[0]))
	{
		wxCommandEvent evt(wxEVT_FILE_DROPPED, wxID_ANY);
		evt.SetString(filenames[0]);
		m_parent->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		wxMessageBox(_("Unsupported image format !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
		return false;
	}

	return true;
}
