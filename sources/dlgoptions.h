#ifndef __DLGOPTIONS_H_INCLUDED__
#define __DLGOPTIONS_H_INCLUDED__

#include <wx/wx.h>

class SettingsManager;
class wxNotebook;

class DlgOptions : public wxDialog
{
    public:
        DlgOptions(wxWindow *parent=NULL);
        virtual ~DlgOptions();
    private:
        void CreateControls();
        void ConnectControls();
        void ApplySettings();
        // Classics Event handlers
        void OnSomethingHasChanged(wxCommandEvent &event);
        void OnBtnApplyClicked(wxCommandEvent &event);
        void OnBtnOkClicked(wxCommandEvent &event);
        // "General" tab controls
        wxChoice *m_cmbLang;
        // Main Controls
        wxNotebook *m_nBook;
        wxButton *m_btnOk, *m_btnCancel, *m_btnApply;
        SettingsManager& m_options;
};

#endif // __DLGOPTIONS_H_INCLUDED__
