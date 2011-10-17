#include "EncodingTable.h"
#include <iomanip>


const EncodingType EncodingTable::OneHot_Method = OneHot;
const EncodingType EncodingTable::Random_Method = SequenceRandom;


ostream &operator<<(ostream &os, const EncodingTable &code)
{
  map<string, string>::const_iterator iter = code._encoding_table.begin();
  map<string, string>::const_iterator iter_end = code._encoding_table.end();

  os << "+----+----------+" << endl;
  os << "| id | encoding |" << endl;
  os << "+----+----------+" << endl;

  for( ;iter != iter_end; iter++)
    os << "| " << setw(2) << (*iter).first << " | " << setw(8) << (*iter).second << " |" << endl;
  os << "+----+----------+" << endl;

  
  return os;
}


istream &operator>>(istream &is, EncodingTable &code)
{
  string stateID;
  string encoded_bits;

  is >> code._nEncodeBits;

  while( is )
  {
    is >> stateID >> encoded_bits;
    code._encoding_table.insert( map<string, string>::value_type( stateID, encoded_bits ) );
  }

  code._is_empty = false;

  return is;
}


EncodingTable::EncodingTable()
{
  _is_empty = true;
  _nEncodeBits = -1;
}


EncodingTable::EncodingTable(map<string, string> &code)
{
  _is_empty = false;
}


void EncodingTable::AddMapping(string &index, string &content)
{
  _encoding_table.insert( map<string, string>::value_type( index, content ) );

  return;
}


void EncodingTable::DeleteMapping(string &index)
{

  return;
}


string EncodingTable::GetMapping(string &index)
{
  return _encoding_table[index];
}


void EncodingTable::BuildMapping(vector<string> &state, const EncodingType &method)
{
  string bits;
  int nBit = 0;
  unsigned int stateNo = state.size();

  switch(method)
  {
    case OneHot:
      bits.append(state.size(), '0');
      bits[0] = '1';
      _encoding_table.insert( map<string, string>::value_type(state[0], bits));	

      for( unsigned int i = 1; i < state.size(); i++ )
      {
	if( (i-1) >= 0 && bits[i-1] == '1' )
	{
	  bits[i-1] = '0';
	  bits[i] = '1';
	}

	_encoding_table.insert( map<string, string>::value_type(state[i], bits));	
      }

      _nEncodeBits = state.size();

      break;
    case SequenceRandom:

      for( nBit = 0; stateNo > 0 ;nBit++, stateNo /= 2 )
	;

      bits.append(nBit, '0'); 

      for( unsigned int j = 0; j < state.size(); j++ )
      {
	for( int k = (bits.size() - 1); (k >= 0); k-- )
	{
	  if( (j & (1 << k)) > 0 )
	    bits[bits.size() - k - 1] = '1';
	  else
	    bits[bits.size() - k - 1] = '0';
	}

	_encoding_table.insert( map<string, string>::value_type(state[j], bits));
      }

      _nEncodeBits = nBit;

      break;
    case SequenceCompact:

      break;
    default:

      break;
  }
}


