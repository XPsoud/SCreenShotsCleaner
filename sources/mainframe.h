#ifndef __MAINFRAME_H_INCLUDED__
#define __MAINFRAME_H_INCLUDED__

#include <wx/wx.h>

class SettingsManager;

class MainFrame: public wxFrame
{
    public:
        MainFrame(wxWindow *parent, const wxString &title);
        ~MainFrame();
    protected:
    private:
        // Misc Methods
        void CreateControls();
        void ConnectControls();
        // Events Handlers
        void OnClose(wxCloseEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnMove(wxMoveEvent &event);
        void OnMenuAboutClicked(wxCommandEvent &event);
        void OnMenuOptionsClicked(wxCommandEvent &event);
        void OnMenuExitClicked(wxCommandEvent &event);
        // Controls vars
        // Other vars
        SettingsManager& m_options;
};

#endif //__MAINFRAME_H_INCLUDED__
