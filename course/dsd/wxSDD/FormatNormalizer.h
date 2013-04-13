#ifndef _FORMAT_NORMALIZER_
#define _FORMAT_NORMALIZER_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "MachineType.h"

using namespace std;

class FormatNormalizer
{
  protected:
    class RowElement
    {
      public:
	RowElement();
	RowElement(const RowElement &element);
	bool Empty();
	bool operator==(const RowElement &row) const;  //Check for the smae nextstate & outputs 
	bool operator<(const RowElement &row) const;
      public:
	bool _is_empty;
	string _present_stateID;
	string _input;
	string _next_stateID;
	string _output;
    };

    class State
    {
      public:
	State();
	State(const State &state);
	bool operator==(const State &state) const;
	bool operator<(const State &state) const;
      public:
	string _ID;
	string _name;
	string _output;
    };
    
    friend istream &operator>>(istream &is, FormatNormalizer &format);
    friend ostream &operator<<(ostream &os, FormatNormalizer &format);
    friend void ExpandDontCare(vector<string> &result, string &s, unsigned int index);

  private:
    bool _is_empty;
    int _nInput;
    int _nOutput;
    int _nState;
    int _nTransition;
    MachineType _machine_type;

    vector<State> _state;
    vector<RowElement> _transition;
    
};

#endif

