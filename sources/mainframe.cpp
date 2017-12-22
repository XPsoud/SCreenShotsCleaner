#include "mainframe.h"

#include "dlgabout.h"
#include "dlgoptions.h"
#include "menu_icons.h"
#include "profilesmanager.h"
#include "settingsmanager.h"
#include "myfiledroptarget.h"

#include <wx/clipbrd.h>
#include <wx/filename.h>

MainFrame::MainFrame(wxWindow *parent, const wxString& title) :
	wxFrame(parent, wxID_ANY, title), m_options(SettingsManager::Get())
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Creating a \"MainFrame\" object\n"));
#endif // SHOW_DEBUG_MSG

	m_sSrcFName=wxEmptyString;
	m_sDstFName1=wxEmptyString;
	m_sDstFName2=wxEmptyString;

	SetIcon(wxICON(appicon));

	CreateControls();
	ConnectControls();

	wxSize szWnd=GetMinSize();
	if (szWnd.GetWidth()<MAINFRAME_MIN_SIZE.GetWidth()) szWnd.SetWidth(MAINFRAME_MIN_SIZE.GetWidth());
	if (szWnd.GetHeight()<MAINFRAME_MIN_SIZE.GetHeight()) szWnd.SetHeight(MAINFRAME_MIN_SIZE.GetHeight());

	SetMinSize(szWnd);

	wxPoint pt=m_options.GetLastSavedPos();
	wxSize sz=m_options.GetLastSavedSize();

	if (sz==wxDefaultSize)
	{
		Maximize(true);
	}
	else
	{
		Move(pt);
		SetSize(sz);
	}
}

MainFrame::~MainFrame()
{
#ifdef SHOW_DEBUG_MSG
	wxPrintf(_T("Destroying a \"MainFrame\" object\n"));
#endif // SHOW_DEBUG_MSG
}

void MainFrame::CreateControls()
{
	const int iSpaces = 5;
	// Status Bar
	CreateStatusBar();
	// Menu Bar
	wxMenuItem *item;
	wxMenu *menu;
	wxMenuBar* mbar = new wxMenuBar();

	#define StdMenuLabel(item) wxGetStockLabel(item, wxSTOCK_WITH_MNEMONIC|wxSTOCK_WITH_ACCELERATOR)

	menu = new wxMenu(_T(""));
		item=new wxMenuItem(menu, wxID_OPEN, StdMenuLabel(wxID_OPEN), _("Select the screenshot file to clean"));
		item->SetBitmap(wxGet_file_open_png_Bitmap());
		menu->Append(item);

		menu->AppendSeparator();
		item=new wxMenuItem(menu, wxID_SAVEAS, StdMenuLabel(wxID_SAVEAS), _("Browse and enter the name of the file to create"));
		menu->Append(item);

		menu->AppendSeparator();

		item=new wxMenuItem(menu, wxID_SAVE, StdMenuLabel(wxID_SAVE), _("Save the cleaned image"));
		item->SetBitmap(wxGet_picture_save_png_Bitmap());
		menu->Append(item);

		menu->AppendSeparator();

		item=new wxMenuItem(menu, wxID_EXIT, StdMenuLabel(wxID_EXIT));
		item->SetBitmap(wxGet_app_exit_png_Bitmap());
		menu->Append(item);
	mbar->Append(menu, wxGetStockLabel(wxID_FILE, wxSTOCK_WITH_MNEMONIC));

	menu = new wxMenu(_T(""));
		item=new wxMenuItem(menu, wxID_PREFERENCES, StdMenuLabel(wxID_PREFERENCES), _("Change the application settings"));
		item->SetBitmap(wxGet_configure_png_Bitmap());
		menu->Append(item);
	mbar->Append(menu, wxGetStockLabel(wxID_EDIT, wxSTOCK_WITH_MNEMONIC));

	menu = new wxMenu(_T(""));
		item=new wxMenuItem(menu, wxID_ABOUT, StdMenuLabel(wxID_ABOUT), _("Show informations about this application"));
		item->SetBitmap(wxGet_help_about_png_Bitmap());
		menu->Append(item);
	mbar->Append(menu, wxGetStockLabel(wxID_HELP, wxSTOCK_WITH_MNEMONIC));

	SetMenuBar(mbar);

	// Controls
	wxPanel *pnl=new wxPanel(this, -1);
	wxStaticText *label;

	wxBoxSizer *szrMain=new wxBoxSizer(wxVERTICAL), *lnszr;

		wxStaticBoxSizer *box=new wxStaticBoxSizer(wxVERTICAL, pnl, _("Input:"));

			box->AddSpacer(1);
			m_optSource[0]=new wxRadioButton(pnl, -1, _("Get source image from file:"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
			box->Add(m_optSource[0], 0, wxLEFT|wxRIGHT, iSpaces);
			box->AddSpacer(1);

			lnszr=new wxBoxSizer(wxHORIZONTAL);
				lnszr->AddSpacer(iSpaces);
				m_txtSrcFile=new wxTextCtrl(pnl, -1, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
				lnszr->Add(m_txtSrcFile, 1, wxALL|wxALIGN_CENTER_VERTICAL, 0);
				m_btnBrwseSrc=new wxButton(pnl, -1, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
				lnszr->Add(m_btnBrwseSrc, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, iSpaces);
			box->Add(lnszr, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);

			m_optSource[1]=new wxRadioButton(pnl, -1, _("Get source image from clipboard"));
			box->Add(m_optSource[1], 0, wxLEFT|wxRIGHT|wxBOTTOM, iSpaces);

		szrMain->Add(box, 0, wxALL|wxEXPAND, iSpaces);

		box=new wxStaticBoxSizer(wxVERTICAL, pnl, _("Parameters:"));
			box->AddSpacer(1);
			lnszr=new wxBoxSizer(wxHORIZONTAL);
				m_chkIncrease=new wxCheckBox(pnl, -1, _("Increase the image size by"));
				lnszr->Add(m_chkIncrease, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
				m_spnIncrease=new wxSpinCtrl(pnl, -1, _T("5"), wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 1, 100, 5);
				lnszr->Add(m_spnIncrease, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, iSpaces);
				label=new wxStaticText(pnl, -1, _("pixels per side"));
				lnszr->Add(label, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, iSpaces);
			box->Add(lnszr, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);

			lnszr=new wxBoxSizer(wxHORIZONTAL);
				label=new wxStaticText(pnl, -1, _("Processing profile:"));
				lnszr->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
				m_cmbProfile=new wxChoice(pnl, -1);
				lnszr->Add(m_cmbProfile, 1, wxLEFT|wxALIGN_CENTER_VERTICAL, iSpaces);
			box->Add(lnszr, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);
		szrMain->Add(box, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);

		box=new wxStaticBoxSizer(wxVERTICAL, pnl, _("Output:"));
			box->AddSpacer(1);
			m_chkOverwrite=new wxCheckBox(pnl, -1, _("Overwrite the source file"));
			box->Add(m_chkOverwrite, 0, wxLEFT|wxRIGHT|wxBOTTOM, iSpaces);

			lnszr=new wxBoxSizer(wxHORIZONTAL);
				m_txtDstFile=new wxTextCtrl(pnl, -1, _T(""));
				lnszr->Add(m_txtDstFile, 1, wxALL|wxALIGN_CENTER_VERTICAL, 0);
				m_btnBrwseDst=new wxButton(pnl, -1, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
				lnszr->Add(m_btnBrwseDst, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, iSpaces);
			box->Add(lnszr, 0, wxALL|wxEXPAND, iSpaces);
		szrMain->Add(box, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);

		lnszr=new wxBoxSizer(wxHORIZONTAL);
			lnszr->AddStretchSpacer(1);
			m_btnSave=new wxButton(pnl, wxID_SAVE, _("Save the cleaned image"));
			lnszr->Add(m_btnSave, 0, wxALL, 0);
		szrMain->Add(lnszr, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, iSpaces);
	pnl->SetSizer(szrMain);

	szrMain->SetSizeHints(this);

	// Default values
	m_chkIncrease->SetValue(false);
	// Processing profiles list
	ProfilesManager& prfMngr=ProfilesManager::Get();
	int iCount=prfMngr.GetProfilesCount();
	for (int i=0; i<iCount; i++)
	{
		Profile* prf=prfMngr.GetProfile(i);
		m_cmbProfile->Append(prf->GetName(), (void*)prf);
	}
	int iSel=m_options.GetLastSelectedProfile();
	if ((iSel<0)||(iSel>=iCount))
		iSel=0;
	m_cmbProfile->SetSelection(iSel);
	// Accept dropped file into the source wxTextCtrl
	m_txtSrcFile->SetDropTarget(new MyFileDropTarget(this));
}

void MainFrame::ConnectControls()
{
	// General events handlers
	Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
	Bind(wxEVT_SIZE, &MainFrame::OnSize, this);
	Bind(wxEVT_MOVE, &MainFrame::OnMove, this);

	Bind(wxEVT_FILE_DROPPED, &MainFrame::OnFileDropped, this);
	// Menus items events handlers
	Bind(wxEVT_MENU, &MainFrame::OnMenuOpenClicked, this, wxID_OPEN);
	Bind(wxEVT_MENU, &MainFrame::OnMenuSaveClicked, this, wxID_SAVE);
	Bind(wxEVT_MENU, &MainFrame::OnMenuSaveAsClicked, this, wxID_SAVEAS);
	Bind(wxEVT_MENU, &MainFrame::OnMenuExitClicked, this, wxID_EXIT);
	Bind(wxEVT_MENU, &MainFrame::OnMenuOptionsClicked, this, wxID_PREFERENCES);
	Bind(wxEVT_MENU, &MainFrame::OnMenuAboutClicked, this, wxID_ABOUT);
	// Controls events handlers
	m_btnBrwseSrc->Bind(wxEVT_BUTTON, &MainFrame::OnMenuOpenClicked, this);
	m_btnBrwseDst->Bind(wxEVT_BUTTON, &MainFrame::OnMenuSaveAsClicked, this);
	m_btnSave->Bind(wxEVT_BUTTON, &MainFrame::OnMenuSaveClicked, this);
	m_txtDstFile->Bind(wxEVT_TEXT, &MainFrame::OnTxtDstFileChanged, this);
	m_cmbProfile->Bind(wxEVT_CHOICE, &MainFrame::OnProfileChanged, this);
	// Update_UI events handlers
	Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_MenuSave, this, wxID_SAVE);
	Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_BtnBrwseDst, this, wxID_SAVEAS);
	m_optSource[0]->Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_OptSource, this);
	m_optSource[1]->Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_OptSource, this);
	m_txtSrcFile->Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_TxtSrcFile, this);
	m_btnBrwseSrc->Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_BtnBrwseSrc, this);
	m_spnIncrease->Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_SpnIncrease, this);
	m_chkOverwrite->Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_ChkOverwrite, this);
	m_txtDstFile->Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_TxtDstFile, this);
	m_btnBrwseDst->Bind(wxEVT_UPDATE_UI, &MainFrame::OnUpdateUI_BtnBrwseDst, this);
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	event.Skip();
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	if (!IsShown()) return;

	if (IsMaximized())
	{
		m_options.SetLastWindowRect(wxDefaultPosition, wxDefaultSize);
	}
	else
	{
		m_options.SetLastWindowRect(GetPosition(), GetSize());
	}

	event.Skip();
}

void MainFrame::OnMove(wxMoveEvent& event)
{
	if (!IsShown()) return;
	m_options.SetLastWindowRect(GetPosition(), GetSize());
	event.Skip();
}

void MainFrame::OnMenuOpenClicked(wxCommandEvent &event)
{
	wxString sMsg=_("Select an image file");
	wxString sWlcrd=_("Images files|*.png;*.jpg;*.bmp|All files|*.*");
	wxString sName, sPath;
	if (!m_txtSrcFile->IsEmpty())
	{
		sName=wxFileNameFromPath(m_txtSrcFile->GetValue());
		sPath=wxPathOnly(m_txtSrcFile->GetValue());
	}
	wxFileDialog fdlg(this, sMsg, sPath, sName, sWlcrd, wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if (fdlg.ShowModal()!=wxID_OK) return;

	m_sSrcFName=fdlg.GetPath();
	wxFileName fname(m_sSrcFName);
	fname.SetName(fname.GetName() + _("-Cleaned"));
	m_sDstFName1=fname.GetFullPath();
}

void MainFrame::OnMenuSaveAsClicked(wxCommandEvent &event)
{
	wxString sMsg=_("Enter the name of the file to create");
	wxString sWlcrd=_("PNG Images files|*.png|JPEG Images files|*.jpg");
	wxString sName, sPath;
	if (!m_txtDstFile->IsEmpty())
	{
		sName=wxFileNameFromPath(m_txtDstFile->GetValue());
		sPath=wxPathOnly(m_txtDstFile->GetValue());
	}
	wxFileDialog fdlg(this, sMsg, sPath, sName, sWlcrd, wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (fdlg.ShowModal()!=wxID_OK) return;

	if (m_optSource[0]->GetValue())
	{
		m_sDstFName1=fdlg.GetPath();
	}
	else
	{
		m_sDstFName2=fdlg.GetPath();
	}
}

void MainFrame::OnMenuExitClicked(wxCommandEvent& event)
{
	Destroy();
}

void MainFrame::OnMenuOptionsClicked(wxCommandEvent& event)
{
	int iLngCode=m_options.GetCurrentLanguage();
	DlgOptions dlg(this);
	dlg.ShowModal();
	if (iLngCode!=m_options.GetCurrentLanguage())
	{
		wxMessageBox(_("The interface language has been changed.\nYou must restart the application for this change to take effect."), _("Restart needed"), wxICON_INFORMATION|wxOK|wxCENTER);
	}
}

void MainFrame::OnMenuAboutClicked(wxCommandEvent& event)
{
	DlgAbout dlg(this);
	dlg.ShowModal();
}

void MainFrame::OnUpdateUI_MenuSave(wxUpdateUIEvent& event)
{
	bool bEnable=true;
	if (m_optSource[0]->GetValue() && m_txtSrcFile->IsEmpty())
	{
		bEnable=false;
		SetStatusText(_("No source file specified !"));
	}
	if (bEnable)
	{
		if (m_txtDstFile->IsEmpty())
		{
			bEnable=false;
			SetStatusText(_("No output file name specified !"));
		}
		else
		{
			wxFileName fname(m_txtDstFile->GetValue());
			if (!fname.IsOk())
			{
				bEnable=false;
				SetStatusText(_("Invalid output file name !"));
			}
		}
	}

	if (bEnable)
	{
		SetStatusText(_("Ready to save the cleaned image"));
	}

	event.Enable(bEnable);
}


void MainFrame::OnUpdateUI_OptSource(wxUpdateUIEvent& event)
{
	bool bClpbrdHasImage=wxTheClipboard->IsSupported(wxDF_BITMAP);
	if (event.GetEventObject()==m_optSource[0])
	{
		if (!bClpbrdHasImage)
			event.Check(true);
	}
	else
	{
		event.Enable(bClpbrdHasImage);
	}
}

void MainFrame::OnUpdateUI_TxtSrcFile(wxUpdateUIEvent& event)
{
	if (m_optSource[0]->GetValue())
	{
		event.SetText(m_sSrcFName);
	}
	else
	{
		event.SetText(wxEmptyString);
	}
}

void MainFrame::OnUpdateUI_BtnBrwseSrc(wxUpdateUIEvent& event)
{
	event.Enable(m_optSource[0]->GetValue());
}

void MainFrame::OnUpdateUI_SpnIncrease(wxUpdateUIEvent& event)
{
	event.Enable(m_chkIncrease->IsChecked());
}

void MainFrame::OnUpdateUI_ChkOverwrite(wxUpdateUIEvent& event)
{
	event.Enable(m_optSource[0]->GetValue());
}

void MainFrame::OnUpdateUI_TxtDstFile(wxUpdateUIEvent& event)
{
	if (m_optSource[0]->GetValue()==true)
	{
		if (m_chkOverwrite->IsChecked())
		{
			event.SetText(m_sSrcFName);
			event.Enable(false);
		}
		else
		{
			event.SetText(m_sDstFName1);
			event.Enable(true);
		}
	}
	else
	{
		event.SetText(m_sDstFName2);
		event.Enable(true);
	}
}

void MainFrame::OnUpdateUI_BtnBrwseDst(wxUpdateUIEvent& event)
{
	event.Enable((m_optSource[0]->GetValue()==false) || (m_chkOverwrite->IsChecked()==false));
}

void MainFrame::OnTxtDstFileChanged(wxCommandEvent& event)
{
	if (m_optSource[0]->GetValue())
	{
		if (m_chkOverwrite->IsChecked()==false)
		{
			m_sDstFName1=m_txtDstFile->GetValue();
		}
	}
	else
	{
		m_sDstFName2=m_txtDstFile->GetValue();
	}
}

void MainFrame::OnMenuSaveClicked(wxCommandEvent& event)
{
	wxImage imgSrc;
	if (m_optSource[0]->GetValue())
	{
		wxFileName SrcFName(m_txtSrcFile->GetValue());
		// First, check the file name and path
		if (!SrcFName.IsOk() || !SrcFName.FileExists())
		{
			wxMessageBox(_("Invalid source file name !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
			m_txtSrcFile->SetFocus();
			return;
		}
		if (!wxImage::CanRead(SrcFName.GetFullPath()))
		{
			wxMessageBox(_("Unsupported image format !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
			return;
		}
		// Try to load the corresponding image
		if (!imgSrc.LoadFile(SrcFName.GetFullPath(), wxBITMAP_TYPE_ANY))
		{
			wxMessageBox(_("Unable to load the image file !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
			return;
		}
	}
	else
	{
		// Check if the clipboard has correct datas
		if (!wxTheClipboard->IsSupported(wxDF_BITMAP))
		{
			wxMessageBox(_("No image datas available from clipboard !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
			return;
		}
		// Try to get the datas from the clipboard
		wxBitmapDataObject bmpData;
		if (!wxTheClipboard->Open())
		{
			wxMessageBox(_("An error occurred while opening the clipboard !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
			return;
		}
		if (!wxTheClipboard->GetData(bmpData))
		{
			wxTheClipboard->Close();
			wxMessageBox(_("An error occurred while retreiving the datas from the clipboard !"), _("Error"), wxICON_EXCLAMATION|wxOK|wxCENTER);
			return;
		}
		wxTheClipboard->Close();
		imgSrc=bmpData.GetBitmap().ConvertToImage();
	}

	if (!imgSrc.HasAlpha()) imgSrc.InitAlpha();

	ProfilesManager& prfMngr=ProfilesManager::Get();
	int iPrfIndex=m_cmbProfile->GetSelection();
	Profile* prf=prfMngr.GetProfile(iPrfIndex);
	if (prf==NULL) return;
	prf->Apply(imgSrc);

	if (m_chkIncrease->IsChecked())
	{
		wxSize szImg=imgSrc.GetSize();
		int iInc=m_spnIncrease->GetValue();
		szImg.IncBy(iInc*2);
		// Set mask color to white (for images formats that don't support alpha)
		imgSrc.SetMaskColour(255, 255, 255);
		imgSrc.Resize(szImg, wxPoint(iInc, iInc));
	}

	wxFileName DstFName(m_txtDstFile->GetValue());
	imgSrc.SaveFile(DstFName.GetFullPath());
	wxMessageBox(_T("Done !"));
}

void MainFrame::OnFileDropped(wxCommandEvent& event)
{
	// The source file name is the one of the dropped file
	m_sSrcFName=event.GetString();
	// Switch to "Source file" mode
	m_optSource[0]->SetValue(true);
	// Update the output file name
	wxFileName fname(m_sSrcFName);
	fname.SetName(fname.GetName() + _("-Cleaned"));
	m_sDstFName1=fname.GetFullPath();
}

void MainFrame::OnProfileChanged(wxCommandEvent& event)
{
	int iSel=m_cmbProfile->GetSelection();
	if (iSel!=wxNOT_FOUND)
		m_options.SetLastSelectedProfile(iSel);
}
