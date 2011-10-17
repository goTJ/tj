// vim:ft=cpp:foldmethod=marker
#ifndef SDD_MAIN_H
#define SDD_MAIN_H
#include "events.h"

// {{{ includes
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
#include "mondrian.xpm"
#endif
#include <wx/toolbar.h>
#include <wx/log.h>
#include <wx/image.h>
// }}}
// {{{ class MyApp : public wxApp{
class MyApp : public wxApp{
public:
	virtual bool OnInit();
}; // }}}
// {{{ class MyFrame : public wxMDIParentFrame{
class MyFrame : public wxMDIParentFrame{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

	void OnSize(wxSizeEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnFileSave(wxCommandEvent &event);
	void OnFileOpen(wxCommandEvent &event);
	void OnRun(wxCommandEvent &event);
	void OnClickTool(wxCommandEvent& event);
private:
	void _initToolBar();
	wxToolBar *_sddToolBar;
	wxList _children;
	DECLARE_EVENT_TABLE()
}; // }}}

#endif
