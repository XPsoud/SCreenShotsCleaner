#ifndef __MAIN_H_INCLUDED__
#define __MAIN_H_INCLUDED__

#include <wx/wx.h>

class ScreenShotCleanerApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
	private:
};

DECLARE_APP(ScreenShotCleanerApp)

#endif //__MAIN_H_INCLUDED__
