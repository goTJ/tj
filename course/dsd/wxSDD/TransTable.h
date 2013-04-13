/* Author: MnO2
 * Email: b92120@csie.ntu.edu.tw
 */

#ifndef __TRANS_TABLE__
#define __TRANS_TABLE__

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <cstdlib>
#include <cstring>
#include "EncodingTable.h"
#include "MachineType.h"

using namespace std;


class TransTable
{
  protected:
    class RowElement
    {
      public:
	RowElement();
	RowElement(const RowElement &element);
	bool Empty();
	bool operator==(const RowElement& row) const;  //Check for the smae nextstate & outputs

      public:
	bool _is_empty;
	string _present_stateID;
	vector<string> _next_stateIDs;
	vector<string> _outputs;
    };
  
  friend ostream &operator<<(ostream &os, const TransTable &trans);
  friend istream &operator>>(istream &is, TransTable &trans);
  friend class list<RowElement>;
  
  public:
    TransTable();
    bool IsEmpty() { return _is_empty; }
    bool IsEncode() { return _is_encoded; }
    void StateMinimization();
    void RowMatching();
    int GetNInput() { return _nInput; };
    int GetNEncodeBits() { return _nEncodeBits; };
    int GetNOutput() { return _nOutput; };
    vector<string> &GetStateIDs() { return _stateIDs; };
    RowElement &GetRowElement(int index);
    void SetRowElement(int index, RowElement &element);
    void SetRowElement(int index, string &presentStateID, vector<string> &nextStateIDs, vector<string> &outputs);
    void EncodeTransitionTable(EncodingTable &encoding);
    void GetAssertedRow(vector< vector<string> > &combination);
    void GetDontCareRow(vector< vector<string> > &combination);
    
  private:
    bool _is_encoded;
    bool _is_empty;
    int _nInput;
    int _nOutput;
    int _nState;
    int _nTransition;
    int _nEncodeBits;
    MachineType _machine_type;
    
    list<RowElement> _trans_table;
    vector<string> _inputs;
    vector<string> _stateIDs;
    vector< list<int> > _adj_state_list;
};


#endif


