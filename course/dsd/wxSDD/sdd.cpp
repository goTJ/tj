// vim:ft=cpp:foldmethod=marker
#include "sdd.h"
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

// {{{ Element::Element(){
Element::Element(){
	_isSelect = false;
} // }}}
// {{{ void Element::setSelect(bool b){
void Element::setSelect(bool b){
	_isSelect = b;
} // }}}
// {{{ int Element::getSelect(){
bool Element::getSelect(){
	return _isSelect;
} // }}}
int State::_radius=25;
int State::_lineWidth=2;
int State::_selWidth=6;
// {{{ bool State::operator==(State &s){
bool State::operator==(const State &s){
	return _name==s._name && _output==s._output && _isSelect==s._isSelect;
} // }}}
// {{{ State::State(){
State::State(){
	_output = "";
	_name = "";
	_isSelect = false;
	isFrom = false;
} // }}}
// {{{ State::State(int x, int y){
State::State(int x, int y, StateInfo *s, SDDPanel *sp){
	State();
	setPos(x, y);
	stateInfo = s;
	_name = "";
	_output = "";
	_isSelect = false;
	isFrom = false;
	parent = sp;
} // }}}
// {{{ State::State(const State &s){
State::State(const State &s){
	*this = s;
	_name = s._name;
	_output = s._output;
	isFrom = false;
} // }}}
// {{{ State &State::operator=(const State &s){
State &State::operator=(const State &s){
	_pos = s._pos;
	return *this;
} // }}}
// {{{ void State::draw(wxDC &dc, double ratio){
void State::draw(wxDC &dc, double ratio){
	int radius=(int)(_radius*ratio);
	int lineWidth=(int)(_lineWidth*ratio);
	int selWidth=(int)(_selWidth*ratio);
	int x=(int)(_pos.x*ratio);
	int y=(int)(_pos.y*ratio);

	if(isFrom)
		dc.SetPen(wxPen((*wxRED), lineWidth, wxSOLID));
	else
		dc.SetPen(wxPen((*wxBLACK), lineWidth, wxSOLID));
	dc.SetBrush(wxBrush(*wxCYAN, wxSOLID));
	dc.DrawCircle(x, y, radius);
	if(_isSelect){
		dc.SetPen(wxPen((*wxBLACK), 1, wxSOLID));
		dc.SetBrush(wxBrush(*wxBLACK, wxSOLID));
		dc.DrawRectangle(x+(radius-1)-(selWidth>>1), y-(selWidth>>1), selWidth, selWidth);
		dc.DrawRectangle(x-(radius-1)-(selWidth>>1), y-(selWidth>>1), selWidth, selWidth);
		dc.DrawRectangle(x-(selWidth>>1), y+(radius-1)-(selWidth>>1), selWidth, selWidth);
		dc.DrawRectangle(x-(selWidth>>1), y-(radius-1)-(selWidth>>1), selWidth, selWidth);
	}
	dc.SetTextForeground(*wxBLUE);
	dc.DrawText(_name, x-2, y-17);
	if(!_output.empty())
		dc.DrawText(wxString::Format(_T("[%s]"), _output.c_str()), x-2, y+1);
} // }}}
// {{{ bool State::isTouched(int x, int y, double ratio){
bool State::isTouched(int x, int y, double ratio){
	return ((x-_pos.x*ratio)*(x-_pos.x*ratio)+(y-_pos.y*ratio)*(y-_pos.y*ratio) <= _radius*ratio*_radius*ratio);
} // }}}
// {{{ void State::setFrom(bool b){
void State::setFrom(bool b){
	isFrom = b;
} // }}}
// {{{ void State::setPos(int x, int y){
void State::setPos(int x, int y){
	_pos.x = x;
	_pos.y = y;
} // }}}
// {{{ wxPoint State::getPos(){
wxPoint State::getPos(){
	return _pos;
} // }}}

int Trans::_lineWidth=2;
int Trans::_selWidth=6;
// {{{ bool Trans::operator==(Trans &t){
bool Trans::operator==(const Trans &t){
	return from==t.from && to==t.to;
} // }}}
// {{{ Trans::Trans(State *f, State *t){
Trans::Trans(State *f, State *t){
	from = f;
	to = t;
	inOut = "";
} // }}}
// {{{ bool Trans::isTouched(int x, int y, double ratio){
bool Trans::isTouched(int x, int y, double ratio){
	int fx, fy, tx, ty;
	int radius=(int)(State::_radius*ratio);
	fx = (int)(from->getPos().x*ratio);
	fy = (int)(from->getPos().y*ratio);
	tx = (int)(to->getPos().x*ratio);
	ty = (int)(to->getPos().y*ratio);
	if(fx == tx && fy == ty){
		if((x-fx+radius*33/20)*(x-fx+radius*33/20)+(y-fy+radius*33/20)*(y-fy+radius*33/20) <= 30)
			return true;
		else
			return false;
	}else{
		double vx, vy;
		double ox, oy;
		vx = (tx-fx)/sqrt((ty-fy)*(ty-fy)+(tx-fx)*(tx-fx));
		vy = (ty-fy)/sqrt((ty-fy)*(ty-fy)+(tx-fx)*(tx-fx));
		ox = vy;
		oy = -vx;
		fx += (int)((vx+ox)*radius/sqrt(2));
		fy += (int)((vy+oy)*radius/sqrt(2));
		tx -= (int)((vx-ox)*radius/sqrt(2));
		ty -= (int)((vy-oy)*radius/sqrt(2));
		double v, d;
		d = (ty-fy)*(ty-fy)+(tx-fx)*(tx-fx);
		v = fabs((x-fx)*(tx-fx)+(y-fy)*(ty-fy));
		if(v > d)
			return false;
		v = fabs((x-tx)*(fx-tx)+(y-ty)*(fy-ty));
		if(v > d)
			return false;
		if(fabs(((x-fx)*(ty-fy)-(y-fy)*(tx-fx))/sqrt(d)) < 2.5)
			return true;
		return false;
	}
} // }}}
// {{{ void Trans::draw(wxDC &dc, double ratio){
void Trans::draw(wxDC &dc, double ratio){
	int lineWidth=(int)(_lineWidth*ratio);
	int selWidth=(int)(_selWidth*ratio);
	int radius=(int)(State::_radius*ratio);
	int fx, fy, tx, ty;
	fx = (int)(from->getPos().x*ratio);
	fy = (int)(from->getPos().y*ratio);
	tx = (int)(to->getPos().x*ratio);
	ty = (int)(to->getPos().y*ratio);
	if(fx == tx && fy == ty){
		dc.SetPen(wxPen((*wxBLACK), lineWidth, wxSOLID));
		dc.DrawSpline(fx-radius, fy, fx-radius*2, fy-radius*2, fx, fy-radius);
		if(_isSelect){
			dc.SetPen(wxPen((*wxBLACK), 1, wxSOLID));
			dc.SetBrush(wxBrush(*wxBLACK, wxSOLID));
			dc.DrawRectangle(fx-radius-(selWidth>>1), fy-(selWidth>>1), selWidth, selWidth);
			dc.DrawRectangle(fx-radius*33/20-(selWidth>>1), fy-radius*33/20-(selWidth>>1), selWidth, selWidth);
			dc.DrawRectangle(fx-(selWidth>>1), fy-radius-(selWidth>>1), selWidth, selWidth);
		}
		dc.SetTextForeground(*wxBLUE);
		dc.DrawText(wxString::Format(_T("%s"), inOut.c_str()), fx-radius*2, fy-radius*2);
	}else{
		int mx, my;
		double vx, vy;
		double ox, oy;
		double x, y;
		vx = (tx-fx)/sqrt((ty-fy)*(ty-fy)+(tx-fx)*(tx-fx));
		vy = (ty-fy)/sqrt((ty-fy)*(ty-fy)+(tx-fx)*(tx-fx));
		ox = vy;
		oy = -vx;
		fx += (int)((vx+ox)*radius/sqrt(2));
		fy += (int)((vy+oy)*radius/sqrt(2));
		tx -= (int)((vx-ox)*radius/sqrt(2));
		ty -= (int)((vy-oy)*radius/sqrt(2));
		mx = (fx+tx)/2;
		my = (fy+ty)/2;
		x = tx-vx*10*ratio;
		y = ty-vy*10*ratio;
		x += ox*5*ratio;
		y += oy*5*ratio;
		dc.SetPen(wxPen((*wxBLACK), lineWidth, wxSOLID));
		dc.DrawLine(fx, fy, tx, ty);
		dc.DrawLine((int)x, (int)y, tx, ty);
		x -= ox*5*2*ratio;
		y -= oy*5*2*ratio;
		dc.DrawLine((int)x, (int)y, tx, ty);
		if(_isSelect){
			dc.SetPen(wxPen((*wxBLACK), 1, wxSOLID));
			dc.SetBrush(wxBrush(*wxBLACK, wxSOLID));
			dc.DrawRectangle(fx-(selWidth>>1), fy-(selWidth>>1), selWidth, selWidth);
			dc.DrawRectangle(tx-(selWidth>>1), ty-(selWidth>>1), selWidth, selWidth);
			dc.DrawRectangle(mx-(selWidth>>1), my-(selWidth>>1), selWidth, selWidth);
		}
		dc.SetTextForeground(*wxBLUE);
		dc.DrawText(wxString::Format(_T("%s"), inOut.c_str()), mx, my);
	}
} // }}}

SDDPanel::CursorType SDDPanel::_cursorType=SDDPanel::SEL;
wxSize SDDPanel::_defaultVirtualSize(1500, 1000);
// {{{ event table of SDDPanel
// Note that MDI_NEW_WINDOW and MDI_ABOUT commands get passed
// to the parent window for processing, so no need to
// duplicate event handlers here.
BEGIN_EVENT_TABLE(SDDPanel, wxScrolledWindow)
	EVT_LEFT_UP(SDDPanel::OnLeftUp)
	EVT_LEFT_DOWN(SDDPanel::OnLeftDown)
	EVT_LEFT_DCLICK(SDDPanel::OnLeftDClick)
	EVT_MOTION(SDDPanel::OnMotion)
	EVT_KEY_UP(SDDPanel::OnKeyUp)
END_EVENT_TABLE() // }}}
// {{{ SDDPanel::SDDPanel(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style)
SDDPanel::SDDPanel(wxWindow *parent, const wxPoint& pos, const wxSize& size)
	: wxScrolledWindow(parent, -1, pos, size, wxSUNKEN_BORDER | wxNO_FULL_REPAINT_ON_RESIZE | wxVSCROLL | wxHSCROLL)
{
	this->parent = static_cast<SDD *>(parent);
	ratio = 1.0;
	_selState = NULL;
	_isClick = false;
	_selTrans = NULL;
	_fromState = NULL;
	SetBackgroundColour(*wxWHITE);
	SetSizeHints(300, 200);
	SetVirtualSize(_defaultVirtualSize);
        SetScrollRate(10, 10);
	switch(_cursorType){
		case SEL:
			SetCursor(wxCursor(wxCURSOR_ARROW));
			break;
		case STATE:
			SetCursor(wxCursor(wxCURSOR_BULLSEYE));
			break;
		case TRANS:
			SetCursor(wxCursor(wxCURSOR_POINT_LEFT));
			break;
	}
} // }}}
// {{{ SDDPanel::~SDDPanel(){
SDDPanel::~SDDPanel(){
	_state.clear();
	_trans.clear();
} // }}}
// {{{ void SDDPanel::OnLeftDown(wxMouseEvent &event){
void SDDPanel::OnLeftDown(wxMouseEvent &event){
	_isClick = true;
	int x, y;
	CalcUnscrolledPosition(event.GetPosition().x, event.GetPosition().y, &x, &y);
	if(_cursorType == SEL){
		bool noSel=true;
		for(list<State>::iterator iter=_state.begin(); iter!=_state.end(); iter++){
			if(iter->isTouched(x, y, ratio)){
				if(_selState)
					_selState->setSelect(false);
				iter->setSelect(!(iter->getSelect()));
				_selState = &*iter;
				noSel = false;
			}
		}
		if(noSel){
			if(_selState)
				_selState->setSelect(false);
			_selState = NULL;
			for(list<Trans>::iterator iter=_trans.begin(); iter!=_trans.end(); iter++){
				if(iter->isTouched(x, y, ratio)){
					if(_selTrans)
						_selTrans->setSelect(false);
					iter->setSelect(!(iter->getSelect()));
					_selTrans = &*iter;
					noSel = false;
				}
			}
			if(noSel){
				if(_selTrans)
					_selTrans->setSelect(false);
				_selTrans = NULL;
			}
		}else{
			if(_selTrans)
				_selTrans->setSelect(false);
			_selTrans = NULL;
			_gap.x = (int)(_selState->getPos().x*ratio-x);
			_gap.y = (int)(_selState->getPos().y*ratio-y);
		}
		OnDraw();
	}
} // }}}
// {{{ void SDDPanel::OnLeftUp(wxMouseEvent &event){
void SDDPanel::OnLeftUp(wxMouseEvent &event){
	_isClick = false;
	int x, y;
	CalcUnscrolledPosition(event.GetPosition().x, event.GetPosition().y, &x, &y);
	if(_cursorType == STATE){
		_state.push_back(State((int)(x/ratio), (int)(y/ratio), stateInfo, this));
		if(_selState)
			_selState->setSelect(false);
		_selState = &_state.back();
		setState(*_selState);
		_selState->setSelect(true);
		if(_selTrans)
			_selTrans->setSelect(false);
		_selTrans = NULL;
		parent->SetStatusText(_T("Add state"));
		OnDraw();
	}else if(_cursorType == TRANS){
		if(_fromState){
			bool noSel=true;
			State *toState=NULL;
			for(list<State>::iterator iter=_state.begin(); iter!=_state.end(); iter++){
				if(iter->isTouched(x, y, ratio)){
					toState = &*iter;
					noSel = false;
				}
			}
			if(!noSel){
				Trans trans(_fromState, toState);
				bool isExist=false;
				for(list<Trans>::iterator iter=_trans.begin(); iter!=_trans.end(); iter++){
					if(*iter == trans)
						isExist = true;
				}
				if(!isExist){
					if(_selTrans)
						_selTrans->setSelect(false);
					_trans.push_back(Trans(_fromState, toState));
					_selTrans = &_trans.back();
					_selTrans->setSelect(true);
					if(_selState)
						_selState->setSelect(false);
					_selState = NULL;
					setTrans(*_selTrans);
					parent->SetStatusText(_T("Add transition"));
					OnDraw();
				}
			}
			_fromState->setFrom(false);
			_fromState = NULL;
			OnDraw();
		}else{
			bool noSel=true;
			for(list<State>::iterator iter=_state.begin(); iter!=_state.end(); iter++){
				if(iter->isTouched(x, y, ratio)){
					if(_fromState)
						_fromState->setFrom(false);
					iter->setFrom(true);
					_fromState = &*iter;
					noSel = false;
				}
			}
			if(noSel){
				if(_fromState)
					_fromState->setFrom(false);
				_fromState = NULL;
			}
			OnDraw();
		}
	}
} // }}}
// {{{ void SDDPanel::OnMotion(wxMouseEvent &event){
void SDDPanel::OnMotion(wxMouseEvent &event){
	static int control=1;
	int x, y;
	CalcUnscrolledPosition(event.GetPosition().x, event.GetPosition().y, &x, &y);
	if(!control && _cursorType == SEL && _isClick && _selState){
		_selState->setPos((int)((x+_gap.x)/ratio), (int)((y+_gap.y)/ratio));
		OnDraw();
	}
	control = (control+1)&3;
} // }}}
// {{{ void SDDPanel::OnLeftDClick(wxMouseEvent &event){
void SDDPanel::OnLeftDClick(wxMouseEvent &event){
	if(_selState && _cursorType == SEL){
		setState(*_selState);
		OnDraw();
	}else if(_selTrans && _cursorType == SEL){
		setTrans(*_selTrans);
		OnDraw();
	}
} // }}}
// {{{ void SDDPanel::OnKeyUp(wxKeyEvent& event){
void SDDPanel::OnKeyUp(wxKeyEvent& event){
	if(event.GetKeyCode() == WXK_DELETE){
		if(_selState){
			for(list<Trans>::iterator iter=_trans.begin(); iter!=_trans.end(); iter++){
				if(iter->from == _selState || iter->to == _selState)
					_trans.remove(*iter);
			}
			_state.remove(*_selState);
			_selState = NULL;
			parent->SetStatusText(_T("Delete state"));
			OnDraw();
		}else if(_selTrans){
			_trans.remove(*_selTrans);
			parent->SetStatusText(_T("Delete transition"));
			OnDraw();
		}
	}
} // }}}
// {{{ void SDDPanel::OnDraw(){
void SDDPanel::OnDraw(){
	wxClientDC dc(this);
	PrepareDC(dc);
	dc.BeginDrawing();
	OnDraw(dc);
	dc.EndDrawing();
} // }}}
// {{{ void SDDPanel::OnDraw(wxDC &dc){
void SDDPanel::OnDraw(wxDC &dc){
	dc.Clear();
	for(list<State>::iterator iter=_state.begin(); iter!=_state.end(); iter++)
		iter->draw(dc, ratio);
	if(_selState)
		_selState->draw(dc, ratio);
	for(list<Trans>::iterator iter=_trans.begin(); iter!=_trans.end(); iter++)
		iter->draw(dc, ratio);
} // }}}
// {{{ void SDDPanel::setState(State &s){
void SDDPanel::setState(State &s){
	wxTextEntryDialog dialog(parent, _T("Please enter state name"), _T("State Property - Name"), s._name, wxOK | wxCANCEL);
	if (dialog.ShowModal() == wxID_OK){
		s._name = dialog.GetValue();
	}
	if(stateInfo->mType == StateInfo::MOORE){
		wxTextEntryDialog dialog2(parent, _T("Please enter state output(0 or 1). (For Moore machine only.)\nEx: '0', '01', '1001'"), _T("State Property - Output"), s._output, wxOK | wxCANCEL);
		if (dialog2.ShowModal() == wxID_OK){
			s._output = dialog2.GetValue();
		}
	}
} // }}}
// {{{ void SDDPanel::setTrans(Trans &t){
void SDDPanel::setTrans(Trans &t){
	wxTextEntryDialog dialog(parent, _T("Please enter input pattern for Moore machine.\nEx: '01,10,00', '00d,101'\nPlease enter input/output pattern for Mealy machine.\nEx: '10/0,01/1', '10d/11,000/01'"), _T("Transition Property - Input/Output"), _selTrans->inOut, wxOK | wxCANCEL);
	if (dialog.ShowModal() == wxID_OK){
		_selTrans->inOut = dialog.GetValue();
	}
} // }}}
// {{{ void SDDPanel::removeStateOutput(){
void SDDPanel::removeStateOutput(){
	for(list<State>::iterator iter=_state.begin(); iter!=_state.end(); iter++)
		iter->_output = "";
	OnDraw();
} // }}}
// {{{ void SDDPanel::saveFile(const string &fileName){
void SDDPanel::saveFile(const string &fileName){
	FILE *fout=fopen(fileName.c_str(), "w");
	if(fout){
		fprintf(fout, "<TSD version=\"1.0\">\n<head>\n<type>%s</type>\n", (stateInfo->mType==StateInfo::MOORE)?"moore":"mealy");
		int transNum=0;
		for(list<Trans>::iterator iter=_trans.begin(); iter!=_trans.end(); iter++){
			transNum++;
			for(char *p=(char *)iter->inOut.c_str(); *p; p++){
				if(*p == ',')
					transNum++;
			}
		}
		fprintf(fout, "<stateNo>%d</stateNo>\n<transitionNo>%d</transitionNo>\n", _state.size(), transNum);
		fprintf(fout, "<inputNo>%d<inputNo>\n<outputNo>%d</outputNo>\n</head>\n", stateInfo->inNum, stateInfo->outNum);
		int i=0;
		for(list<State>::iterator iter=_state.begin(); iter!=_state.end(); iter++){
			iter->id = i;
			fprintf(fout, "<state>\n<id>%d</id>\n<name>%s</name>\n", i++, iter->_name.c_str());
			if(stateInfo->mType == StateInfo::MOORE)
				fprintf(fout, "<output>%s</output>\n", iter->_output.c_str());
			fprintf(fout, "<x>%d</x>\n<y>%d</y>\n</state>\n", iter->getPos().x, iter->getPos().y);
		}
		i = 0;
		for(list<Trans>::iterator iter=_trans.begin(); iter!=_trans.end(); iter++){
			string s=iter->inOut.c_str();
			char *p=(char *)s.c_str();
			p = strtok(p, ",");
			while(p){
				fprintf(fout, "<transition>\n<from>%d</from>\n<to>%d</to>\n", iter->from->id, iter->to->id);
				char *pp;
				for(pp=p; *pp&&*pp!='/'; pp++)
					;
				if(*pp){
					*pp = '\0';
					pp++;
					fprintf(fout, "<input>%s</input>\n", p);
					if(stateInfo->mType == StateInfo::MEALY)
						fprintf(fout, "<output>%s</output>\n", pp);
				}else{
					fprintf(fout, "<input>%s</input>\n", p);
				}
				fprintf(fout, "</transition>\n");
				p = strtok(NULL, ",");
			}
		}
		fclose(fout);
	}
} // }}}
// {{{ void SDDPanel::parseSDDLFile(const string &fileName){
void SDDPanel::parseSDDLFile(const string &fileName){
	FILE *fin=fopen(fileName.c_str(), "r");
	char buf[100];
	if(fin){
		int stateNum=0;
		{ // {{{ parse head
			fgets(buf, 100, fin);	// skip <TSD version="1.0">
			fgets(buf, 100, fin);	// skip <head>
			while(fgets(buf, 100, fin)){
				char *p=strtok(buf, ">");
				if(!strcmp(p, "</head")){
					break;
				}else if(!strcmp(p, "<stateNo")){
					p = strtok(NULL, "<");
					sscanf(p, "%d", &stateNum);
				}
			}
		} // }}}
		{ // {{{ parse state
			int cx=300, cy=150;
			int r=100;
			int dx=cx+r, dy=cy;
			int count=1;
			int x, y;
			int id;
			string name, output;
			while(fgets(buf, 100, fin)){
				char *p=strtok(buf, ">");
				if(!strcmp(p, "<transition")){
					break;
				}else if(!strcmp(p, "<state")){
					x = dx;
					y = dy;
					id = -1;
					name = "";
					output = "";
				}else if(!strcmp(p, "</state")){
					_state.push_back(State(x, y, stateInfo, this));
					_state.back()._name = name.c_str();
					_state.back()._output = output.c_str();
					_state.back().id = id;
					if(x==dx && y==dy){
						dx = (int)(cx+r*cos(2*3.1415*count/stateNum));
						dy = (int)(cy+r*sin(2*3.1415*count/stateNum));
						count++;
					}
				}else if(!strcmp(p, "<id")){
					p = strtok(NULL, "<");
					if(*p == '/'){
						id = -1;
					}else{
						sscanf(p, "%d", &id);
					}
				}else if(!strcmp(p, "<x")){
					p = strtok(NULL, "<");
					if(*p == '/'){
						x = dx;
					}else{
						sscanf(p, "%d", &x);
					}
				}else if(!strcmp(p, "<y")){
					p = strtok(NULL, "<");
					if(*p == '/'){
						y = dy;
					}else{
						sscanf(p, "%d", &y);
					}
				}else if(!strcmp(p, "<name")){
					p = strtok(NULL, "<");
					if(*p == '/'){
						name = "";
					}else{
						name = p;
					}
				}else if(!strcmp(p, "<output") && stateInfo->mType==StateInfo::MOORE){
					p = strtok(NULL, "<");
					if(*p == '/'){
						output = "";
					}else{
						output = p;
					}
				}
			}
		} // }}}
		{ // {{{ parse trans
			State *from=NULL, *to=NULL;
			string input, output;
			while(fgets(buf, 100, fin)){
				char *p=strtok(buf, ">");
				if(!strcmp(p, "<transition")){
					from = to = NULL;
					input = output = "";
				}else if(!strcmp(p, "</transition")){
					bool isExist=false;
					if(!from || !to)
						break;
					for(list<Trans>::iterator iter=_trans.begin(); iter!=_trans.end(); iter++){
						if(from==iter->from && to==iter->to){
							isExist = true;
							iter->inOut += ",";
							iter->inOut += input.c_str();
							if(stateInfo->mType==StateInfo::MEALY){
								iter->inOut += "/";
								iter->inOut += output.c_str();
							}
							break;
						}
					}
					if(!isExist){
						_trans.push_back(Trans(from, to));
						_trans.back().inOut = input.c_str();
						if(stateInfo->mType==StateInfo::MEALY){
							_trans.back().inOut += "/";
							_trans.back().inOut += output.c_str();
						}
					}
				}else if(!strcmp(p, "<from")){
					int id;
					p = strtok(NULL, "<");
					if(*p == '/'){
						id = -1;
					}else{
						sscanf(p, "%d", &id);
					}
					for(list<State>::iterator iter=_state.begin(); iter!=_state.end(); iter++){
						if(id == iter->id){
							from = &*iter;
							break;
						}
					}
				}else if(!strcmp(p, "<to")){
					int id;
					p = strtok(NULL, "<");
					if(*p == '/'){
						id = -1;
					}else{
						sscanf(p, "%d", &id);
					}
					for(list<State>::iterator iter=_state.begin(); iter!=_state.end(); iter++){
						if(id == iter->id){
							to = &*iter;
							break;
						}
					}
				}else if(!strcmp(p, "<input")){
					p = strtok(NULL, "<");
					if(*p == '/'){
						input = "";
					}else{
						input = p;
					}
				}else if(!strcmp(p, "<output") && stateInfo->mType==StateInfo::MEALY){
					p = strtok(NULL, "<");
					if(*p == '/'){
						output = "";
					}else{
						output = p;
					}
				}
			}
		}// }}}
		fclose(fin);
		OnDraw();
	}
} // }}}

// {{{ event table of SDD
// Note that MDI_NEW_WINDOW and MDI_ABOUT commands get passed
// to the parent window for processing, so no need to
// duplicate event handlers here.
BEGIN_EVENT_TABLE(SDD, wxMDIChildFrame)
	EVT_SIZE(SDD::OnSize)
	EVT_COMBOBOX(SDDPANEL_MTYPE, SDD::OnChangeMType)
	EVT_COMBOBOX(SDDPANEL_INUM, SDD::OnChangeInputNum)
	EVT_COMBOBOX(SDDPANEL_ONUM, SDD::OnChangeOutputNum)
END_EVENT_TABLE() // }}}
// {{{ SDD::SDD(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style)
SDD::SDD(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style)
       : wxMDIChildFrame(parent, -1, title, pos, size, style | wxNO_FULL_REPAINT_ON_RESIZE | wxRESIZE_BORDER)
{
	fileName = "";
	stateInfo = new StateInfo();
	stateInfo->mType = StateInfo::MOORE;
	stateInfo->inNum = stateInfo->outNum = 1;
	wxString mType[]={"Moore", "Mealy"};
	wxString inOutputNum[]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
	CreateStatusBar();
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *optionsizer = new wxBoxSizer(wxHORIZONTAL);
	optionsizer->Add(new wxStaticText(this, -1, _T("Machine Type: "), wxDefaultPosition), 0, wxALL, 6);
	mTypeCombo = new wxComboBox(this, SDDPANEL_MTYPE, _T("Moore"), wxDefaultPosition, wxSize(100, -1), 2, mType, wxCB_READONLY);
	optionsizer->Add(mTypeCombo, 0, wxALL, 5);
	optionsizer->Add(new wxStaticText(this, -1, _T("Input Number: "), wxDefaultPosition), 0, wxALL, 6);
	inNumCombo = new wxComboBox(this, SDDPANEL_INUM, _T("1"), wxDefaultPosition, wxSize(40, -1), 11, inOutputNum, wxCB_READONLY);
	optionsizer->Add(inNumCombo, 0, wxALL, 5);
	optionsizer->Add(new wxStaticText(this, -1, _T("Output Number: "), wxDefaultPosition), 0, wxALL, 6);
	outNumCombo = new wxComboBox(this, SDDPANEL_ONUM, _T("1"), wxDefaultPosition, wxSize(40, -1), 11, inOutputNum, wxCB_READONLY);
	optionsizer->Add(outNumCombo, 0, wxALL, 5);
	topsizer->Add(optionsizer, 0, wxALL, 2);
	_SDDPanel = new SDDPanel(this, wxPoint(-1, -1), wxDefaultSize);
	_SDDPanel->stateInfo = stateInfo;
	topsizer->Add(_SDDPanel, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_LEFT, 2);
	topsizer->SetMinSize(GetClientSize());
	topsizer->Layout();
	SetSizer(topsizer);
	_SDDPanel->SetSize(GetClientSize().x, GetClientSize().y-33);
//	topsizer->SetSizeHints(this);
	/*
	SetStatusText(wxString::Format(_T("%d %d"), _SDDPanel->GetSize().x, _SDDPanel->GetSize().y));
	SetStatusText(wxString::Format(_T("%d %d"), GetPosition().x, GetPosition().y), 1);
	SetStatusText(wxString::Format(_T("%d %d"), topsizer->GetMinSize().x, topsizer->GetMinSize().y), 2);
	*/
} // }}}
// {{{ SDD::SDD(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style)
SDD::SDD(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style, const string &fileName)
       : wxMDIChildFrame(parent, -1, title, pos, size, style | wxNO_FULL_REPAINT_ON_RESIZE | wxRESIZE_BORDER)
{
	this->fileName = fileName;
	stateInfo = new StateInfo();
	stateInfo->mType = StateInfo::MOORE;
	stateInfo->inNum = stateInfo->outNum = 1;
	wxString mType[]={"Moore", "Mealy"};
	wxString inOutputNum[]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
	CreateStatusBar();
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *optionsizer = new wxBoxSizer(wxHORIZONTAL);
	optionsizer->Add(new wxStaticText(this, -1, _T("Machine Type: "), wxDefaultPosition), 0, wxALL, 6);
	mTypeCombo = new wxComboBox(this, SDDPANEL_MTYPE, _T("Moore"), wxDefaultPosition, wxSize(100, -1), 2, mType, wxCB_READONLY);
	optionsizer->Add(mTypeCombo, 0, wxALL, 5);
	optionsizer->Add(new wxStaticText(this, -1, _T("Input Number: "), wxDefaultPosition), 0, wxALL, 6);
	inNumCombo = new wxComboBox(this, SDDPANEL_INUM, _T("1"), wxDefaultPosition, wxSize(40, -1), 11, inOutputNum, wxCB_READONLY);
	optionsizer->Add(inNumCombo, 0, wxALL, 5);
	optionsizer->Add(new wxStaticText(this, -1, _T("Output Number: "), wxDefaultPosition), 0, wxALL, 6);
	outNumCombo = new wxComboBox(this, SDDPANEL_ONUM, _T("1"), wxDefaultPosition, wxSize(40, -1), 11, inOutputNum, wxCB_READONLY);
	optionsizer->Add(outNumCombo, 0, wxALL, 5);
	parseSDDLFile(fileName);
	topsizer->Add(optionsizer, 0, wxALL, 2);
	_SDDPanel = new SDDPanel(this, wxPoint(-1, -1), wxDefaultSize);
	_SDDPanel->stateInfo = stateInfo;
	_SDDPanel->parseSDDLFile(fileName);
	topsizer->Add(_SDDPanel, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_LEFT, 2);
	topsizer->SetMinSize(GetClientSize());
	topsizer->Layout();
	SetSizer(topsizer);
	_SDDPanel->SetSize(GetClientSize().x, GetClientSize().y-33);
//	topsizer->SetSizeHints(this);
	/*
	SetStatusText(wxString::Format(_T("%d %d"), _SDDPanel->GetSize().x, _SDDPanel->GetSize().y));
	SetStatusText(wxString::Format(_T("%d %d"), GetPosition().x, GetPosition().y), 1);
	SetStatusText(wxString::Format(_T("%d %d"), topsizer->GetMinSize().x, topsizer->GetMinSize().y), 2);
	*/
} // }}}
// {{{ SDD::~SDD(){
SDD::~SDD(){
	delete _SDDPanel;
} // }}}
// {{{ void SDD::OnSize(wxSizeEvent& event){
void SDD::OnSize(wxSizeEvent& event){
	if(_SDDPanel)
		_SDDPanel->SetSize(GetClientSize().x, GetClientSize().y-33);
} // }}}
// {{{ void SDD::OnChangeMType(wxCommandEvent& event){
void SDD::OnChangeMType(wxCommandEvent& event){
	if(event.GetInt() == 0){
		stateInfo->mType = StateInfo::MOORE;
	}else{
		stateInfo->mType = StateInfo::MEALY;
		_SDDPanel->removeStateOutput();
	}
	SetStatusText(wxString::Format(_T("Change machine type: %s"), (event.GetInt()==0)?_T("Moore"):_T("Mealy")));
} // }}}
// {{{ void SDD::OnChangeInputNum(wxCommandEvent& event){
void SDD::OnChangeInputNum(wxCommandEvent& event){
	stateInfo->inNum = event.GetInt();
	SetStatusText(wxString::Format(_T("Change input number: %d"), (int)event.GetInt()));
} // }}}
// {{{ void SDD::OnChangeOutputNum(wxCommandEvent& event){
void SDD::OnChangeOutputNum(wxCommandEvent& event){
	stateInfo->outNum = event.GetInt();
	SetStatusText(wxString::Format(_T("Change Output number: %d"), (int)event.GetInt()));
} // }}}
// {{{ void SDD::command(int id){
void SDD::command(int id){
	switch(id){
		case SDDTOOL_SEL:
			_SDDPanel->SetCursor(wxCursor(wxCURSOR_ARROW));
			break;
		case SDDTOOL_STATE:
			_SDDPanel->SetCursor(wxCursor(wxCURSOR_BULLSEYE));
			break;
		case SDDTOOL_TRANS:
			_SDDPanel->SetCursor(wxCursor(wxCURSOR_POINT_LEFT));
			break;
		case SDDTOOL_MAGP:
			_SDDPanel->ratio += 0.125;
			_SDDPanel->SetVirtualSize((int)(SDDPanel::_defaultVirtualSize.x*_SDDPanel->ratio),(int)(SDDPanel::_defaultVirtualSize.x*_SDDPanel->ratio));
			_SDDPanel->OnDraw();
			break;
		case SDDTOOL_MAGM:
			_SDDPanel->ratio -= 0.125;
			_SDDPanel->SetVirtualSize((int)(SDDPanel::_defaultVirtualSize.x*_SDDPanel->ratio),(int)(SDDPanel::_defaultVirtualSize.x*_SDDPanel->ratio));
			_SDDPanel->OnDraw();
			break;
	}
} // }}}
// {{{ void SDD::saveFile(){
void SDD::saveFile(){
	if(fileName.empty()){
		wxFileDialog dialog(this, _T("Save SDDL File to..."), _T("./"), _T(""),
				_T("SDDL files (*.sddl)|*.sddl"), wxSAVE|wxOVERWRITE_PROMPT);
		dialog.SetFilterIndex(1);
		if (dialog.ShowModal() == wxID_OK){
			fileName = dialog.GetPath().c_str();
		}
	}
	if(!fileName.empty()){
		SetStatusText(_T("Save File"));
		SetTitle(wxString::Format(_T("SDD - %s"), fileName.c_str()));
		_SDDPanel->saveFile(fileName);
	}
} // }}}
// {{{ void SDD::parseSDDLFile(const string &fileName){
void SDD::parseSDDLFile(const string &fileName){
	FILE *fin=fopen(fileName.c_str(), "r");
	char buf[100];
	if(fin){
		fgets(buf, 100, fin);	// skip <TSD version="1.0">
		fgets(buf, 100, fin);	// skip <head>
		while(fgets(buf, 100, fin)){
			char *p=strtok(buf, ">");
			if(!strcmp(p, "</head")){
				break;
			}else if(!strcmp(p, "<type")){
				p = strtok(NULL, "<");
				if(!strcmp(p, "moore")){
					stateInfo->mType = StateInfo::MOORE;
					mTypeCombo->SetSelection(StateInfo::MOORE);
				}else if(!strcmp(p, "mealy")){
					stateInfo->mType = StateInfo::MEALY;
					mTypeCombo->SetSelection(StateInfo::MEALY);
				}
			}else if(!strcmp(p, "<inputNo")){
				p = strtok(NULL, "<");
				int n;
				sscanf(p, "%d", &n);
				stateInfo->inNum = n;
				inNumCombo->SetSelection(n);
			}else if(!strcmp(p, "<outputNo")){
				p = strtok(NULL, "<");
				int n;
				sscanf(p, "%d", &n);
				stateInfo->outNum = n;
				outNumCombo->SetSelection(n);
			}
		}
		fclose(fin);
	}
} // }}}
