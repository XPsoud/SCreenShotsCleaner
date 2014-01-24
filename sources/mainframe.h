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
        // Events Handlers
        void OnClose(wxCloseEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnMove(wxMoveEvent &event);
        void OnMenuOpenClicked(wxCommandEvent &event);
        void OnMenuSaveClicked(wxCommandEvent &event);
        void OnMenuSaveAsClicked(wxCommandEvent &event);
        void OnMenuExitClicked(wxCommandEvent &event);
        void OnMenuOptionsClicked(wxCommandEvent &event);
        void OnMenuAboutClicked(wxCommandEvent &event);
        void OnTxtDstFileChanged(wxCommandEvent &event);
        void OnUpdateUI_MenuSave(wxUpdateUIEvent &event);
        void OnUpdateUI_OptSource(wxUpdateUIEvent &event);
        void OnUpdateUI_TxtSrcFile(wxUpdateUIEvent &event);
        void OnUpdateUI_BtnBrwseSrc(wxUpdateUIEvent &event);
        void OnUpdateUI_SpnIncrease(wxUpdateUIEvent &event);
        void OnUpdateUI_ChkOverwrite(wxUpdateUIEvent &event);
        void OnUpdateUI_TxtDstFile(wxUpdateUIEvent &event);
        void OnUpdateUI_BtnBrwseDst(wxUpdateUIEvent &event);
        // Controls vars
        wxRadioButton *m_optSource[2];
        wxTextCtrl *m_txtSrcFile, *m_txtDstFile;
        wxButton *m_btnBrwseSrc, *m_btnBrwseDst, *m_btnSave;
        wxCheckBox *m_chkIncrease, *m_chkOverwrite;
        wxSpinCtrl *m_spnIncrease;
        // Other vars
        wxString m_sSrcFName, m_sDstFName1, m_sDstFName2;
        SettingsManager& m_options;
};

#endif //__MAINFRAME_H_INCLUDED__
