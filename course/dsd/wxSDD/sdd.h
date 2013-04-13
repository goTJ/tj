// vim:ft=cpp:foldmethod=marker
#ifndef SDD_H
#define SDD_H
#define SDD_EVENT_OFFSEST 100

#include "events.h"
#include <list>
#include <string>

using std::list;
using std::string;

class SDDPanel;
class SDD;
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
// {{{ struct StateInfo{
struct StateInfo{
	enum MTYPE{MOORE, MEALY};
	MTYPE mType;
	int inNum;
	int outNum;
}; // }}}
// {{{ class Element{
class Element{
public:
	Element();
	virtual ~Element(){};
	virtual void draw(wxDC &dc, double ratio)=0;
	virtual bool isTouched(int x, int y, double ratio)=0;
	void setSelect(bool);
	bool getSelect();
protected:
	bool _isSelect;
}; // }}}
// {{{ class State: public Element{
class State: public Element{
public:
	bool operator==(const State &s);
	State();
	State(int x, int y, StateInfo *, SDDPanel *);
	State(const State &s);
	State &operator=(const State &s);
	void draw(wxDC &dc, double ratio);
	bool isTouched(int x, int y, double ratio);

	void setFrom(bool);
	void setPos(int x, int y);
	wxPoint getPos();
	StateInfo *stateInfo;
	SDDPanel *parent;
	wxString _name;
	wxString _output;
	static int _radius;
	int id;
private:
	bool isFrom;
	wxPoint _pos;
	static int _lineWidth;
	static int _selWidth;
}; // }}}
// {{{ class Trans: public Element{
class Trans: public Element{
public:
	bool operator==(const Trans &t);
	Trans(State *from, State *to);
	void draw(wxDC &dc, double ratio);
	bool isTouched(int x, int y, double ratio);
	wxString inOut;
	State *from;
	State *to;
private:
	static int _lineWidth;
	static int _selWidth;
}; // }}}
// {{{ class Transmission: public Element{
/*
class Transmission: public Element{
public:
	void draw();
	bool isTouched();
private:
};*/ // }}}
// {{{ class SDDPanel: public wxScrolledWindow{
class SDDPanel: public wxScrolledWindow{
public:
	enum CursorType{SEL, STATE, TRANS};
	SDDPanel(wxWindow *parent, const wxPoint& pos, const wxSize& size);
	~SDDPanel();

	void OnDraw();
	void OnDraw(wxDC &);
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnLeftDClick(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);
	void OnKeyUp(wxKeyEvent& event);

	void setState(State &);
	void setTrans(Trans &);
	void removeStateOutput();
	void saveFile(const string &fileName);
	void parseSDDLFile(const string &);
	static void setCursorType(CursorType c){
		_cursorType = c;
	}
	double ratio;
	static wxSize _defaultVirtualSize;
	StateInfo *stateInfo;
	SDD *parent;
private:
	// for move state
	bool _isClick;
	wxPoint _gap;
	State *_selState;
	// for add trans
	State *_fromState;
	Trans *_selTrans;

	static CursorType _cursorType;
	list<State> _state;
	list<Trans> _trans;
	DECLARE_EVENT_TABLE();
}; // }}}
// {{{ class SDD: public wxMDIChildFrame{
class SDD: public wxMDIChildFrame{
public:
	SDD(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style);
	SDD(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style, const string &fileName);
	~SDD();

	void OnSize(wxSizeEvent& event);
	void OnChangeMType(wxCommandEvent& event);
	void OnChangeInputNum(wxCommandEvent& event);
	void OnChangeOutputNum(wxCommandEvent& event);

	void saveFile();
	void command(int);
	StateInfo *stateInfo;
	string fileName;
private:
	void parseSDDLFile(const string &);
	SDDPanel *_SDDPanel;
	wxComboBox *mTypeCombo, *inNumCombo, *outNumCombo;
	DECLARE_EVENT_TABLE();
}; // }}}

#endif
