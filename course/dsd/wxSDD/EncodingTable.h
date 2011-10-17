#ifndef __ENCODING_TABLE__
#define __ENCODING_TABLE__

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum EncodingType { OneHot, SequenceRandom, SequenceCompact, UserSpecify };

class EncodingTable
{
  friend ostream &operator<<(ostream &os, const EncodingTable &code);
  friend istream &operator>>(istream &is, EncodingTable &code);

  public:
    EncodingTable();
    EncodingTable(map<string, string> &code);
    bool IsEmpty();
    void AddMapping(string &index, string &content);    
    void DeleteMapping(string &index);
    string GetMapping(string &index);
    int GetNEncodeBits() { return _nEncodeBits; };
    void BuildMapping(vector<string> &state, const EncodingType &method = OneHot);
        
    static const EncodingType OneHot_Method;
    static const EncodingType Random_Method;
    
  private:
    bool _is_empty;
    int _nEncodeBits;
    map<string, string> _encoding_table;    
    static EncodingType _method;

};


#endif


