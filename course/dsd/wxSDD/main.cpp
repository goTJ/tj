// vim:ft=cpp:foldmethod=marker
#include "main.h"
#include "sdd.h"
#include "forward.h"
#include <string>
#include <cstdlib>
#include <cstdio>

using std::string;

IMPLEMENT_APP(MyApp)

// {{{ event table
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(MENU_Quit,  MyFrame::OnQuit)
	EVT_MENU(MENU_About, MyFrame::OnAbout)
	EVT_MENU(MENU_NEW, MyFrame::OnNew)
	EVT_MENU(MENU_SAVE, MyFrame::OnFileSave)
	EVT_MENU(MENU_OPEN, MyFrame::OnFileOpen)
	EVT_MENU(MENU_RUN, MyFrame::OnRun)
	EVT_MENU(SDDTOOL_SEL, MyFrame::OnClickTool)
	EVT_MENU(SDDTOOL_STATE, MyFrame::OnClickTool)
	EVT_MENU(SDDTOOL_TRANS, MyFrame::OnClickTool)
	EVT_MENU(SDDTOOL_MAGP, MyFrame::OnClickTool)
	EVT_MENU(SDDTOOL_MAGM, MyFrame::OnClickTool)
	EVT_SIZE(MyFrame::OnSize)
END_EVENT_TABLE() // }}}
// {{{ bool MyApp::OnInit()
bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame(_T("State Diagram Designer"), wxPoint(-1, -1), wxSize(800, 600)
			,wxDEFAULT_FRAME_STYLE | wxHSCROLL | wxVSCROLL);
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
} // }}}
// {{{ MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxMDIParentFrame(NULL, -1, title, pos, size, style|wxNO_FULL_REPAINT_ON_RESIZE)
{
	SetIcon(wxICON(mondrian));
#ifdef __WXMSW__
#if 0
	// Experimental: change the window menu
	wxMenu* windowMenu = new wxMenu;
	windowMenu->Append(5000, _T("My menu item!"));
	frame->SetWindowMenu(windowMenu);
#endif
#endif
#if wxUSE_MENUS
	wxMenu *menuFile = new wxMenu;
	wxMenu *helpMenu = new wxMenu;
	menuFile->Append(MENU_NEW, _T("&New\tCtrl-N"), _T("New File"));
	menuFile->Append(MENU_SAVE, _T("&Save\tCtrl-S"), _T("Save File"));
	menuFile->Append(MENU_OPEN, _T("&Open\tCtrl-O"), _T("Open File"));
	menuFile->AppendSeparator();
	menuFile->Append(MENU_RUN, _T("&Run\tCtrl-R"), _T("Run SDDL File"));
	menuFile->AppendSeparator();
	menuFile->Append(MENU_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
	helpMenu->Append(MENU_About, _T("&About...\tF1"), _T("Show about dialog"));
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, _T("&File"));
	menuBar->Append(helpMenu, _T("&Help"));
	SetMenuBar(menuBar);
#endif // wxUSE_MENUS
	_initToolBar();
#if wxUSE_STATUSBAR
	CreateStatusBar(2);
	SetStatusText(_T("Welcome to wxWindows!"));
#endif // wxUSE_STATUSBAR
} // }}}
// {{{ void MyFrame::_initToolBar(){
void MyFrame::_initToolBar(){
	wxBitmap toolBarBitmaps[5];
	// initialize main toolbar
	wxToolBar *toolBar=CreateToolBar(wxNO_BORDER|wxHORIZONTAL|wxTB_FLAT, MAIN_TOOLBAR);
	toolBarBitmaps[0] = wxBITMAP(new);
	toolBarBitmaps[1] = wxBITMAP(open);
	toolBarBitmaps[2] = wxBITMAP(save);
	toolBarBitmaps[3] = wxBITMAP(run);
	toolBarBitmaps[4] = wxBITMAP(help);

	toolBar->AddTool(MENU_NEW, _T("New"), toolBarBitmaps[0], _T("New file"));
	toolBar->AddTool(MENU_OPEN, _T("Open"), toolBarBitmaps[1], _T("Open file"));
	toolBar->AddTool(MENU_SAVE, _T("Save"), toolBarBitmaps[2], _T("Save file"));
	toolBar->AddSeparator();
	toolBar->AddTool(MENU_RUN, _T("Run"), toolBarBitmaps[3], _T("Run SDDL file"));
	toolBar->AddSeparator();
	toolBar->AddTool(MENU_About, _T("About"), toolBarBitmaps[4], _T("About me"));
	toolBar->AddSeparator();

	toolBar->Realize();
	// initial sdd toolbar
	toolBarBitmaps[0] = wxBITMAP(selection);
	toolBarBitmaps[1] = wxBITMAP(state);
	toolBarBitmaps[2] = wxBITMAP(transmission);
	toolBarBitmaps[3] = wxBITMAP(magp);
	toolBarBitmaps[4] = wxBITMAP(magm);

	_sddToolBar = new wxToolBar(this, SDD_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxVERTICAL|wxTB_FLAT);
	_sddToolBar->AddTool(SDDTOOL_SEL, _T("Selection"), toolBarBitmaps[0], _T("Selection"), wxITEM_RADIO);
	_sddToolBar->AddTool(SDDTOOL_STATE, _T("State"), toolBarBitmaps[1], _T("State"), wxITEM_RADIO);
	_sddToolBar->AddTool(SDDTOOL_TRANS, _T("Transmission"), toolBarBitmaps[2], _T("Transmission"), wxITEM_RADIO);
	_sddToolBar->AddSeparator();
	_sddToolBar->AddTool(SDDTOOL_MAGP, _T("Zoom in"), toolBarBitmaps[3], _T("Zoom in"));
	_sddToolBar->AddTool(SDDTOOL_MAGM, _T("Zoom out"), toolBarBitmaps[4], _T("Zoom out"));
	_sddToolBar->AddSeparator();
	_sddToolBar->Realize();
} // }}}
// {{{ void MyFrame::OnSize(wxSizeEvent& event){
void MyFrame::OnSize(wxSizeEvent& event){
	int w, h;
	GetClientSize(&w, &h);
	int offset;
	if(_sddToolBar){
		_sddToolBar->SetSize(-1, h);
		_sddToolBar->Move(0, 0);
		offset = _sddToolBar->GetSize().x;
	}else{
		offset = 0;
	}
	GetClientWindow()->SetSize(offset, 0, w-offset, h);

#ifdef __WXUNIVERSAL__   
	event.Skip();
#endif
} // }}}
// {{{ void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)){
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)){
	Close(TRUE);
} // }}}
// {{{ void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)){
	wxString msg;
	msg.Printf(_T("State Diagram Designer ver. 0.9.2.8\nPower by FHC\nLast update at 06.01.03"));

	wxMessageBox(msg, _T("About MENU"), wxOK | wxICON_INFORMATION, this);
} // }}}
// {{{ void MyFrame::OnNew(wxCommandEvent& event){
void MyFrame::OnNew(wxCommandEvent& event){
	static int count=1;
	wxString title;
	title.Printf(_T("SDD - Unnamed %d"), count);
	SDD *subframe = new SDD(this, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
	_children.Append(subframe);
	subframe->Show(TRUE);
	SetStatusText(wxString::Format(_T("new window: %s"), title.c_str()));
	count++;
} // }}}
// {{{ void MyFrame::OnFileSave(wxCommandEvent &event){
void MyFrame::OnFileSave(wxCommandEvent &event){
	SDD *sdd=static_cast<SDD *>(GetActiveChild());
	if(sdd)
		sdd->saveFile();
} // }}}
// {{{ void MyFrame::OnFileOpen(wxCommandEvent &event){
void MyFrame::OnFileOpen(wxCommandEvent &event){
	wxFileDialog dialog(this, _T("Open SDDL File"), _T("./"), _T(""),
				_T("SDDL files (*.sddl)|*.sddl"));
	if (dialog.ShowModal() == wxID_OK){
		wxString title;
		title.Printf(_T("SDD - %s"), dialog.GetPath().c_str());
		SDD *subframe = new SDD(this, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, string(dialog.GetPath().c_str()));
		_children.Append(subframe);
		subframe->Show(TRUE);
		SetStatusText(wxString::Format(_T("new window: %s"), title.c_str()));
	}
} // }}}
// {{{ void MyFrame::OnRun(wxCommandEvent &event){
void MyFrame::OnRun(wxCommandEvent &event){
	SDD *sdd=static_cast<SDD *>(GetActiveChild());
	if(!sdd)
		return;
	OnFileSave(event);
	wxFileDialog dialog(this, _T("Save SDDL File to..."), _T("./"), _T(""),
			_T("LCDL files (*.lcdl)|*.lcdl"), wxSAVE|wxOVERWRITE_PROMPT);
	dialog.SetFilterIndex(1);
	if (dialog.ShowModal() == wxID_OK && !dialog.GetPath().empty()){
		forward(sdd->fileName, string(dialog.GetPath().c_str()));
		wxDialog *dialogResult=new wxDialog(this, -1, _T("Result"), wxDefaultPosition, wxSize(800, 600));
		wxTextCtrl *textWindow = new wxTextCtrl(dialogResult, -1, _T("A help window"),
				wxDefaultPosition, dialogResult->GetClientSize(), wxTE_MULTILINE | wxSUNKEN_BORDER);
		FILE *fin=fopen(dialog.GetPath().c_str(), "r");
		wxString res;
		if(fin){
			char buf[200];
			while(fgets(buf, 200, fin)){
				res += buf;
			}
		}
		textWindow->SetValue(res);
		dialogResult->ShowModal();
		/*
		string cmd("notepad ");
		cmd += dialog.GetPath().c_str();
		system(cmd.c_str());
		*/
	}
} // }}}
// {{{ void MyFrame::OnClickTool(wxCommandEvent& event){
void MyFrame::OnClickTool(wxCommandEvent& event){
	switch(event.GetId()){
		case SDDTOOL_SEL:
			SetStatusText(_T("Set cursor as selection!"));
			SDDPanel::setCursorType(SDDPanel::SEL);
			break;
		case SDDTOOL_STATE:
			SetStatusText(_T("Set cursor as new state!"));
			SDDPanel::setCursorType(SDDPanel::STATE);
			break;
		case SDDTOOL_TRANS:
			SetStatusText(_T("Set cursor as new transtion!"));
			SDDPanel::setCursorType(SDDPanel::TRANS);
			break;
	}
	switch(event.GetId()){
		case SDDTOOL_SEL:
		case SDDTOOL_STATE:
		case SDDTOOL_TRANS:
			SetStatusText(_T("Set cursor!"));
			for(wxNode *node=_children.GetFirst(); node; node=node->GetNext()){
				// FIXME: can't dynamic casting
				SDD *sdd = static_cast<SDD *>(node->GetData());
				if(sdd)
					sdd->command(event.GetId());
			}
			break;
		case SDDTOOL_MAGP:
		case SDDTOOL_MAGM:
			SDD *sdd=static_cast<SDD *>(GetActiveChild());
			if(sdd)
				sdd->command(event.GetId());
	}
} // }}}
