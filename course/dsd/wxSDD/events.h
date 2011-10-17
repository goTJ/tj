// vim:ft=cpp:foldmethod=marker
#ifndef EVENTS_H
#define EVENTS_H

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
// }}}
enum{
	// {{{ MyFrame
	MENU_Quit = 1,
	MENU_NEW,
	MENU_OPEN,
	MENU_SAVE,
	MENU_RUN,
	MAIN_TOOLBAR,
	SDD_TOOLBAR,
	SDDTOOL_SEL,
	SDDTOOL_STATE,
	SDDTOOL_TRANS,
	SDDTOOL_MAGP,
	SDDTOOL_MAGM,
	SDDPANEL_MTYPE,
	SDDPANEL_INUM,
	SDDPANEL_ONUM,
	// }}}
	// {{{ SDD
	MDI_QUIT,
	/*
	MDI_NEW_WINDOW,
	MDI_REFRESH,
	MDI_CHANGE_TITLE,
	MDI_CHANGE_POSITION,
	MDI_CHANGE_SIZE,
	MDI_CHILD_QUIT,
	*/
	MDI_ABOUT,
	// }}}
	// MyFrame About
	MENU_About = wxID_ABOUT,
};

#endif
