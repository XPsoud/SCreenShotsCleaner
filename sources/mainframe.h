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
        wxBitmapType GetBmpTypeFromFilename(const wxString& filename);
        // Events Handlers
        void OnClose(wxCloseEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnMove(wxMoveEvent &event);
        void OnMenuOpenClicked(wxCommandEvent &event);
        void OnMenuSaveClicked(wxCommandEvent &event);
        void OnMenuExitClicked(wxCommandEvent &event);
        void OnMenuOptionsClicked(wxCommandEvent &event);
        void OnMenuAboutClicked(wxCommandEvent &event);
        void OnChkIncreaseClicked(wxCommandEvent &event);
        void OnUpdateUI_MenuSave(wxUpdateUIEvent &event);
        // Controls vars
        wxTextCtrl *m_txtSrcFile;
        wxButton *m_btnBrwseSrc, *m_btnSave;
        wxCheckBox *m_chkIncrease;
        wxSpinCtrl *m_spnIncrease;
        // Other vars
        SettingsManager& m_options;
};

#endif //__MAINFRAME_H_INCLUDED__
