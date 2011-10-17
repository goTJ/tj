#include "forward.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include "TransTable.h"
#include "EncodingTable.h"
#include "FormatNormalizer.h"
#include "qm.h"

using namespace std;

void forward(const string &stateFileName, const string &circuitFileName, const string &encodingFileName="")
{
  ifstream inFile;
  ofstream outFile(circuitFileName.c_str());
  stringstream sFile(stringstream::in | stringstream::out);
  TransTable trans_table;
  EncodingTable codebook;


  if( encodingFileName.empty() )
  {
    FormatNormalizer norm;

    inFile.open(stateFileName.c_str());
    inFile >> norm;
    sFile << norm;
    inFile.close();

    sFile >> trans_table;

    codebook.BuildMapping(trans_table.GetStateIDs(), EncodingTable::OneHot_Method);

    outFile << "Encoding: " << endl;
    outFile << codebook << endl;
  }
  else 
  {
    FormatNormalizer norm;

    inFile.open(encodingFileName.c_str());
    inFile >> codebook;
    outFile << "Encoding: " << endl;
    outFile << codebook << endl;
    inFile.close();

    inFile.open(stateFileName.c_str());
    inFile >> norm;
    inFile.close();

    sFile << norm;
    sFile >> trans_table;
  }

  vector< vector<string> > asserted(trans_table.GetNInput() + codebook.GetNEncodeBits());
  vector< vector<string> > dontcared(trans_table.GetNInput() + codebook.GetNEncodeBits());

  trans_table.EncodeTransitionTable(codebook);
  outFile << trans_table << endl;
  trans_table.GetAssertedRow(asserted);
  trans_table.GetDontCareRow(dontcared);


  for( unsigned int i = 0; i < asserted.size(); i++ )
  {
    list<string> ones;
    list<string> dont_care;
    list<string> ans;

    if( i < (unsigned int)codebook.GetNEncodeBits() )
      outFile << "=== Q" << i << " ===" << endl;
    else
      outFile << "=== Output " << (i-codebook.GetNEncodeBits()+1) << " ===" << endl;


    for( unsigned int j = 0; j < asserted[i].size(); j++ )
    {
      ones.push_back(asserted[i][j]);
      //outFile << asserted[i][j] << endl;
    }

    for( unsigned int j = 0; j < dontcared[i].size(); j++ )
    {
      dont_care.push_back(dontcared[i][j]);
      //outFile << dontcared[i][j] << endl;
    }

    if( ones.size() > 0 )
    {
      ans = doQM(ones, dont_care);

      for( list<string>::iterator iter = ans.begin(); iter != ans.end(); ++iter )
	outFile << *iter << endl;
    }
    else
      outFile << '0' << endl;
  }

  outFile << endl;

}
void forward(const string &stateFileName, const string &circuitFileName){
  forward(stateFileName, circuitFileName, "");
}

