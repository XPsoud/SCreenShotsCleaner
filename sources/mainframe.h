#ifndef __MAINFRAME_H_INCLUDED__
#define __MAINFRAME_H_INCLUDED__

#include <wx/wx.h>
#include <wx/spinctrl.h>

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
        void UpdateControlsState();
        // Events Handlers
        void OnClose(wxCloseEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnMove(wxMoveEvent &event);
        void OnMenuAboutClicked(wxCommandEvent &event);
        void OnMenuOptionsClicked(wxCommandEvent &event);
        void OnMenuExitClicked(wxCommandEvent &event);
        void OnChkIncreaseClicked(wxCommandEvent &event);
        // Controls vars
        wxTextCtrl *m_txtSrcFile;
        wxButton *m_btnBrwseSrc;
        wxCheckBox *m_chkIncrease;
        wxSpinCtrl *m_spnIncrease;
        // Other vars
        SettingsManager& m_options;
};

#endif //__MAINFRAME_H_INCLUDED__
