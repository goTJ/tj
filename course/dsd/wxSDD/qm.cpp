// vim:ft=cpp:foldmethod=marker
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "Set.h"
#include "Pattern.h"
#include "qm.h"

using namespace std;

// {{{ struct PI{
struct PI{
	Set set;
	Pattern ptn;
	bool used;

	PI &operator=(const PI &pi){ set=pi.set; ptn=pi.ptn; used=pi.used; return *this;};
	PI(int n):set(n){ used=false; };
	PI(int n, const Pattern &p, const int &pos, const Set &s):set(s),ptn(p,pos){ used=false; };
}; // }}}
// {{{ class PICmp{
class PICmp{
public:
	bool operator()(const PI &p1, const PI &p2) const { return p1.ptn < p2.ptn; };
private:
}; // }}}
typedef set<PI, PICmp> PIset;

// {{{ inline void swap(int &a, int &b)
inline void swap(int &a, int &b)
{
	int tmp;

	tmp = a;
	a = b;
	b = tmp;
} // }}}
// {{{ void rotate_set(const vector<PI> &pis, Set *cols, const int &n)
void rotate_set(const vector<PI> &pis, Set *cols[], const int &n)
{
////	cout << "====== go rotate_set(" << pis.size() << " PIs) ======" << endl;
////	cout << "=== final PIs ===" << endl;
////	for(unsigned int i=0; i<pis.size(); i++){
////		cout << pis[i].ptn << ", " << pis[i].set << endl;
////	}
////	cout << "=== END ===" << endl;
	for(int i=0; i<n; i++)
		cols[i] = new Set(pis.size());
	for(unsigned int i=0; i<pis.size(); i++){
		for(unsigned int j=0; j<pis[i].set.size(); j++){
			if(pis[i].set[j])
				cols[j]->setElement(i, true);
		}
	}
} // }}}
// {{{ void delPI(vector<PI> &pis, Set *cols[], int &n, const unsigned int &idx)
inline void delPI(vector<PI> &pis, Set *cols[], int &n, const unsigned int &idx)
{
	if(idx != pis.size()-1)
		pis[idx] = pis.back();
	pis.pop_back();
	for(int j=0; j<n; j++)
		cols[j]->del(idx);
} // }}}
// {{{ inline void delCol(vector<PI> &pis, Set *cols[], int &n, const int &idx)
inline void delCol(vector<PI> &pis, Set *cols[], int &n, const int &idx)
{
	n--;
	for(unsigned int j=0; j<pis.size(); j++)
		pis[j].set.del(idx);
	delete cols[idx];
	if(idx != n)
		cols[idx] = cols[n];
} // }}}
// {{{ void pickPI(vector<Pattern> &ansPIs, vector<PI> &pis, Set *cols[], int &n, const int &idx)
void pickPI(list<string> &ansPIs, vector<PI> &pis, Set *cols[], int &n, const unsigned int &idx)
{
	int pos;

	ansPIs.push_back(pis[idx].ptn.getPtn());
	while((pos=pis[idx].set.front()) != -1)
		delCol(pis, cols, n, pos);
	delPI(pis, cols, n, idx);
	// erase empty PIs
	for(unsigned int i=0; i<pis.size(); i++){
		if(!pis[i].set.count())
			delPI(pis, cols, n, i);
	}
////	cout << "=== pis ===" << endl;
////	for(unsigned int i=0; i<pis.size(); i++)
////		cout << pis[i].set << ", " << pis[i].set.count() << endl;
////	cout << "= cols =" << endl;
////	for(int i=0; i<n; i++)
////		cout << *cols[i] << ", " << cols[i]->count() << endl;
//////	cout << "= ans =" << endl;
//////	for(unsigned int i=0; i<ansPIs.size(); i++)
//////		cout << ansPIs[i] << endl;
////	cout << "=== END ===" << endl;
} // }}}

list<string> doQM(list<string> &ones, list <string> &dc)
{
	// {{{ declaration
	int num, n;
	PIset *table[2];
	vector<PI> pis;
	list<string> ansPIs;
	Set **cols;
	int now=0, next=1;
	// }}}
	// {{{ initialize
	if(ones.size() == 0)
		return *(new list<string>);
	num = ones.begin()->size();
	n = ones.size();
	table[0] = new PIset[num+1];
	table[1] = new PIset[num+1];
	// }}}
//	cout << "=== Patterns ===" << endl;
	{ // {{{ input
		int i=0;
		for(list<string>::iterator iter=ones.begin(); iter!=ones.end(); iter++, i++){
//			cout << *iter << endl;
			PI *p=new PI(n);
			p->set.setElement(i, true);
			p->ptn.setPtn(*iter);
			table[now][p->ptn.countOne()].insert(*p);
		}
		for(list<string>::iterator iter=dc.begin(); iter!=dc.end(); iter++){
//			cout << *iter << endl;
			PI *p=new PI(n);
			p->ptn.setPtn(*iter);
			table[now][p->ptn.countOne()].insert(*p);
		}
	} // }}}
//	cout << "=== END ===" << endl;
//	cout << "====== merge PIs(" << n << ") ======" << endl;
	// {{{ merge PIs
	for(int i=num; i>=0; i--){
////		cout << "=== " << num-i+1 << " times. ===" << endl;
////		for(int ii=0; ii<=i; ii++){
////			cout <<  ii << ": ";
////			for(PIset::iterator iter=table[now][ii].begin(); iter!=table[now][ii].end(); iter++){
////				cout << iter->ptn << ", ";
////			}
////			cout << endl;
////		}
		bool hasMerged=false;
		// {{{ find mergable PI pair and merge them
		for(int j=1; j<=i; j++){
			for(PIset::iterator iter=table[now][j].begin(); iter!=table[now][j].end(); iter++){
				for(PIset::iterator iter2=table[now][j-1].begin(); iter2!=table[now][j-1].end(); iter2++){
					int t_pos;
					if((t_pos=iter->ptn.similar(iter2->ptn)) >= 0){
						hasMerged = true;
						((PI *)&(*iter))->used = ((PI *)&(*iter2))->used  = true;
						PI *p=new PI(n, iter->ptn, t_pos, (iter->set)|(iter2->set));
						table[next][j-1].insert(*p);
////						cout << "merge: " << iter->ptn << " & " << iter2->ptn << " => " << p->ptn << endl;
////						cout << "union: " << iter->set << " | " << iter2->set << " => " << p->set << endl;
						delete p;
					}
				}
			}
		} // }}}
		// {{{ put unused to final PIs and clear table
		for(int j=0; j<=i; j++){
			for(PIset::iterator iter=table[now][j].begin(); iter!=table[now][j].end(); iter++){
				if(!iter->used && ((PI *)&*iter)->set.count()){
////					cout << "put: " << iter->ptn << ", " << iter->set << endl;
					pis.push_back(*iter);
				}
			}
			table[now][j].clear();
		} // }}}
		if(!hasMerged)
			break;
		swap(now, next);
	} // }}}
	// {{{ delete something for qm & generate something for covering problem
	delete[] table[0];
	delete[] table[1];
	cols = new Set*[n];
	rotate_set(pis, cols, n);
	// }}}
//	cout << "====== covering problem(" << pis.size() << ") ======" << endl;
	// {{{ covering problem
	while(n && pis.size()){
		// {{{ display info
////		cout << "=== pis ===" << endl;
////		for(unsigned int i=0; i<pis.size(); i++)
////			cout << pis[i].set << ", " << pis[i].set.count() << endl;
////		cout << "= cols " << n << " =" << endl;
////		for(int i=0; i<n; i++)
////			cout << *cols[i] << ", " << cols[i]->count() << endl;
////	//	cout << "= ans =" << endl;
////	//	for(unsigned int i=0; i<ansPIs.size(); i++)
////	//		cout << ansPIs[i] << endl;
////		cout << "=== END ===" << endl;
		// }}}
		bool isChange=false;
		// {{{ find essential PIs
		for(int i=0; i<n; i++){
			if(cols[i]->count() == 1){
				int t_idx=cols[i]->front();
////				cout << "essential PI idx: " << t_idx << ", " << *cols[i] << endl;
				pickPI(ansPIs, pis, cols, n, t_idx);
				isChange = true;
				i = -1;
			}
		} // }}}
		for(int i=1; i<n; i++){
			for(int j=0; j<i&&i<n; j++){
				int t_res=cols[j]->cmpSet(*cols[i]);
				if(t_res){
					unsigned int t_idx=(t_res==1)?j:i;
					delCol(pis, cols, n, t_idx);
					isChange = true;
				}
			}
		}
		// {{{ find redundant PIs
		for(unsigned int i=1; i<pis.size(); i++){
			for(unsigned int j=0; j<i&&i<pis.size(); j++){
				int t_res=pis[j].set.cmpSet(pis[i].set);
				if(t_res){
					unsigned int t_idx=(t_res==1)?i:j;

////					cout << "compare sets: " << pis[j].set << endl << "              " << pis[i].set << " => " << pis[t_idx].set << endl;
////					cout << "=== pis ===" << endl;
////					for(unsigned int i=0; i<pis.size(); i++)
////						cout << pis[i].set << ", " << pis[i].set.count() << endl;
////					cout << "= cols " << n << " =" << endl;
////					for(int i=0; i<n; i++)
////						cout << *cols[i] << ", " << cols[i]->count() << endl;
					delPI(pis, cols, n, t_idx);
					isChange = true;
				}
			}
		} // }}}
		// {{{ choose less element PI
		if(n && !isChange){
			int t_min_idx=0;
			for(unsigned int i=1; i<pis.size(); i++){
				if(pis[i].set.count() < pis[t_min_idx].set.count())
					t_min_idx = i;
			}
			pickPI(ansPIs, pis, cols, n, t_min_idx);
		} // }}}
	} // }}}
	for(int i=0; i<n; i++)
		delete cols[i];
	delete[] cols;
	return ansPIs;
}
