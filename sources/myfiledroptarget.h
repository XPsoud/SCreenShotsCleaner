#ifndef __MYFILEDROPTARGET_H_INCLUDED__
#define __MYFILEDROPTARGET_H_INCLUDED__

#include <wx/wx.h>
#include <wx/dnd.h>

wxDECLARE_EVENT(wxEVT_FILE_DROPPED, wxCommandEvent);

class MyFileDropTarget : public wxFileDropTarget
{
	public:
		MyFileDropTarget(wxWindow* parent);
		virtual ~MyFileDropTarget();
		virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames);
	protected:
	private:
		wxWindow *m_parent;
};

#endif // __MYFILEDROPTARGET_H_INCLUDED__
