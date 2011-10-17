#include "TransTable.h"
#include <iomanip>

ostream &operator<<(ostream &os, const TransTable &trans)
{
	TransTable::RowElement temp;

	//cout << "Input Num: " << trans._nInput << endl;

	list<TransTable::RowElement>::const_iterator iter = trans._trans_table.begin();

	os << "Transition Table: " << endl;
//	os << "columns are (PresentS Inputs NextS Output)" << endl;
	os << "+----------+--------+----------+--------+" << endl;
	os << "| PresentS | Inputs |    NextS | Output |" << endl;
	os << "+----------+--------+----------+--------+" << endl;


		for( ; iter != trans._trans_table.end(); ++iter )
		{
			for( int i = 0; i < (1 << trans._nInput); i++ )
			{
				os << "| ";
				os << setw(8) << (*iter)._present_stateID << " | " << setw(6) << trans._inputs[i] << " | ";
				os << setw(8) << (*iter)._next_stateIDs[i]<< " | " << setw(6) << (*iter)._outputs[i] << " |";
				os << endl;
			}
		}
	os << "+----------+--------+----------+--------+" << endl;

	return os;
}


istream &operator>>(istream &is, TransTable &trans)
{
	string textline;

	getline(is, textline, '\n');

	if( textline == string("<TSD version=\"1.0\">") )
	{

	}
	else
	{
		cerr << "class TransTable: in operator>>, the input stream is not a TSD format" << endl;
		exit(1);
	}


	getline(is, textline, '\n');

	if( textline == string("<head>") )
	{
		while( getline(is, textline, '\n') )
		{
			if( textline == string("</head>") )
				break;
			else if( strncmp(textline.c_str(), "<stateNo>", 9) == 0 )
			{
				trans._nState = atoi(textline.c_str() + 9);	
			}
			else if( strncmp(textline.c_str(), "<transitionNo>", 14) == 0 )
			{
				trans._nTransition = atoi(textline.c_str() + 14);
			}
			else if( strncmp(textline.c_str(), "<inputNo>", 9) == 0 )
			{
				trans._nInput = atoi(textline.c_str() + 9);
			}
			else if( strncmp(textline.c_str(), "<outputNo>", 10) == 0 )
			{
				trans._nOutput = atoi(textline.c_str() + 10);
			}
			else if( strncmp(textline.c_str(), "<type>", 6) == 0 )
			{
				string::size_type pos = textline.find("</type>");  

				if( pos == string::npos )
				{
					cerr << "Error: <type> label does not have a matching </type>\n";
					exit(1);
				}

				string machine_type = textline.substr(6, pos - 6);

				if( machine_type == string("mealy") )
					trans._machine_type = MealyMachine;
				else if( machine_type == string("moore") )
					trans._machine_type = MooreMachine;
				else
				{
					cerr << "Error: Not supported machine type\n";
					exit(1);
				}
			}
			else 
			{
				cerr << "class TransTable: Not supported label in header block" << endl;
				exit(1);
			}
		}
	}
	else
	{
		cerr << "class TransTable: require <head> block in TSD format" << endl;
		exit(1);
	}



	map<string, string> stateID_output_mapping;

	while( getline(is, textline, '\n') )
	{
		if( textline == string("<state>") )
		{
			string stateID;
			string stateName;
			string stateOutput;

			while( getline(is, textline, '\n') )
			{
				if( strncmp(textline.c_str(), "<id>", 4) == 0 )
				{
					string::size_type pos = textline.find("</id>");

					if( pos == string::npos )
					{
						cerr << "Error: <id> label does not have a matching </id>\n";
						exit(1);
					}

					stateID = textline.substr(4, pos - 4);
				}
				else if( strncmp(textline.c_str(), "<name>", 6) == 0 )
				{
					string::size_type pos = textline.find("</name>");  

					if( pos == string::npos )
					{
						cerr << "Error: <name> label does not have a matching </name>\n";
						exit(1);
					}

					stateName = textline.substr(6, pos - 6);
				}
				else if( strncmp(textline.c_str(), "<output>", 8) == 0 )
				{
					string::size_type pos = textline.find("</output>");

					if( pos == string::npos )
					{
						cerr << "Error: <output> label does not have a matching </output>\n";
						exit(1);
					}

					stateOutput = textline.substr(8, pos - 8);
				}
				else if( strncmp(textline.c_str(), "<x>", 3) == 0 )
				{

				}
				else if( strncmp(textline.c_str(), "<y>", 3) == 0 )
				{

				}
				else if( strncmp(textline.c_str(), "<color>", 7) == 0 )
				{

				}
				else if( textline == string("</state>") )
					break;
				else
				{
					cerr << "class TransTable: Not supported label in state block" << endl;
					exit(1);
				}
			}

			trans._stateIDs.push_back(stateID);
			stateID_output_mapping.insert( map<string, string>::value_type(stateID, stateOutput) ); 
		}
		else if( textline == string("<transition>") )
		{
			string transitionFrom;
			string transitionTo;
			string transitionInput;
			string transitionOutput;

			while( getline(is, textline, '\n') )
			{
				if( strncmp(textline.c_str(), "<from>", 6) == 0 )
				{
					string::size_type pos = textline.find("</from>");

					if( pos == string::npos )
					{
						cerr << "Error: <from> label does not have a matching </from>\n";
						exit(1);
					}

					transitionFrom = textline.substr(6, pos - 6);
				}
				else if( strncmp(textline.c_str(), "<to>", 4) == 0 )
				{
					string::size_type pos = textline.find("</to>");

					if( pos == string::npos )
					{
						cerr << "Error: <to> label does not have a matching </to>\n";
						exit(1);
					}

					transitionTo = textline.substr(4, pos - 4);
				}
				else if( strncmp(textline.c_str(), "<input>", 7) == 0 )
				{
					string::size_type pos = textline.find("</input>");

					if( pos == string::npos )
					{
						cerr << "Error: <input> label does not have a matching </input>\n";
						exit(1);
					}

					transitionInput = textline.substr(7, pos - 7);

				}
				else if( strncmp(textline.c_str(), "<output>", 8) == 0 )
				{
					string::size_type pos = textline.find("</output>");

					if( pos == string::npos )
					{
						cerr << "Error: <output> label does not have a matching </output>\n";
						exit(1);
					}

					transitionOutput = textline.substr(8, pos - 8);

				}
				else if( textline == string("</transition>") )
				{
					break;
				}
				else
				{
					cerr << "class TransTable: Not supported label in transition block" << endl;
					exit(1);
				}
			}

			list<TransTable::RowElement>::reverse_iterator riter = trans._trans_table.rbegin();

			for( ; riter != trans._trans_table.rend(); ++riter )
			{
				if( (*riter)._present_stateID == transitionFrom )
				{
					(*riter)._next_stateIDs.push_back(transitionTo);

					if( trans._machine_type == MealyMachine )
						(*riter)._outputs.push_back(transitionOutput);
					else if( trans._machine_type == MooreMachine )
						(*riter)._outputs.push_back(stateID_output_mapping[transitionTo]);

					break;
				}
			}

			if( riter == trans._trans_table.rend() )
			{
				TransTable::RowElement temp;

				temp._present_stateID = transitionFrom;
				temp._next_stateIDs.push_back(transitionTo);

				if( trans._machine_type == MealyMachine )
					temp._outputs.push_back(transitionOutput);
				else if( trans._machine_type == MooreMachine )
					temp._outputs.push_back(stateID_output_mapping[transitionTo]);

				trans._trans_table.push_front(temp);
			}
		}
		else
			cerr << "class TransTable: Not supported label" << endl;

	}

	// Generate inputs
	string bits;

	bits.append(trans._nInput, '0');

	for( int i = 0; i < (1 << trans._nInput); i++ )
	{
		for( int j = 0; j < trans._nInput; j++ )
		{
			if( (i & (1 << j)) != 0)
				bits[j] = '1';
			else
				bits[j] = '0';
		}

		trans._inputs.push_back(bits);
	}


	return is;
}


TransTable::TransTable()
{
	_is_empty = true;
	_nEncodeBits = -1;
}


void TransTable::StateMinimization()
{

}


void TransTable::RowMatching()
{
	list<RowElement>::iterator i, j;
	vector<string> duplicateID;

	for( i = _trans_table.begin(); i != _trans_table.end(); ++i )
	{
		for( j = _trans_table.begin(); j != _trans_table.end(); ++j )
		{
			if( i != j )
			{
				if( *i == *j )
				{
					duplicateID.push_back((*j)._present_stateID);
					_trans_table.erase(j);
				}
			}
		}

		if( !duplicateID.empty() )
		{
			for( unsigned int k = 0; k < duplicateID.size(); k++ )
			{

			}
		}
	}
	return;
}


TransTable::RowElement &TransTable::GetRowElement(int index)
{
	int count = 0;

	for( list<RowElement>::iterator iter = _trans_table.begin(); iter != _trans_table.end(); ++iter )
	{
		if( count == index )
			return *iter;
		else if( count < index )
			count++;
		else
			continue;
	}

	return *_trans_table.end();
}


void TransTable::SetRowElement(int index, RowElement &element)
{


}


void TransTable::SetRowElement(int index, string &presentStateID, vector<string> &nextStateIDs, vector<string> &outputs)
{
	list<RowElement>::iterator iter = _trans_table.begin();
	int count = 0;

	for( ; iter != _trans_table.end(); ++iter )
	{
		if( count == index )
			break;
		else if( count < index )
			count++;
		else
			continue;
	}

	if( iter == _trans_table.end() )
		return;


	(*iter)._present_stateID = presentStateID;

	for( unsigned int i = 0; i < (*iter)._next_stateIDs.size(); i++ )
	{
		(*iter)._next_stateIDs[i] = nextStateIDs[i];
	}

	for( unsigned int i = 0; i < (*iter)._outputs.size(); i++ )
	{
		(*iter)._outputs[i] = outputs[i];
	}

}


void TransTable::EncodeTransitionTable(EncodingTable &encoding)
{
	for( list<RowElement>::iterator iter = _trans_table.begin(); iter != _trans_table.end(); ++iter )
	{
		(*iter)._present_stateID = encoding.GetMapping((*iter)._present_stateID);

		for( unsigned int i = 0; i < (*iter)._next_stateIDs.size(); i++ )
		{
			(*iter)._next_stateIDs[i] = encoding.GetMapping((*iter)._next_stateIDs[i]);
		}
	}

	_nEncodeBits = encoding.GetNEncodeBits();

	return;
}


void TransTable::GetAssertedRow(vector< vector<string> > &combination)
{
	for(list<RowElement>::iterator iter = _trans_table.begin(); iter != _trans_table.end(); ++iter)
	{
		for( int i = 0; i < _nEncodeBits; i++ )
		{
			for( int j = 0; j < (1 << _nInput); j++ )
			{
				if( (*iter)._next_stateIDs[j][i] == '1' )
					combination[i].push_back( (*iter)._present_stateID + _inputs[j] );
			}
		}
	}

	for(list<RowElement>::iterator iter = _trans_table.begin(); iter != _trans_table.end(); ++iter)
	{
		for( int i = 0; i < _nOutput; i++ )
		{
			for( int j = 0; j < (1 << _nInput); j++ )
			{
				if( (*iter)._outputs[j][i] == '1' )
					combination[i+_nEncodeBits].push_back( (*iter)._present_stateID + _inputs[j] );
			}
		}
	}

	return;
}


void TransTable::GetDontCareRow(vector< vector<string> > &combination)
{
	for(list<RowElement>::iterator iter = _trans_table.begin(); iter != _trans_table.end(); ++iter)
	{
		for( int i = 0; i < _nEncodeBits; i++ )
		{
			for( int j = 0; j < (1 << _nInput); j++ )
			{
				if( (*iter)._next_stateIDs[j][i] == 'd' )
					combination[i].push_back( (*iter)._present_stateID + _inputs[j] );
			}
		}
	}

	for(list<RowElement>::iterator iter = _trans_table.begin(); iter != _trans_table.end(); ++iter)
	{
		for( int i = 0; i < _nOutput; i++ )
		{
			for( int j = 0; j < (1 << _nInput); j++ )
			{
				if( (*iter)._outputs[j][i] == 'd' )
					combination[i+_nEncodeBits].push_back( (*iter)._present_stateID + _inputs[j] );
			}
		}
	}

	return;
}


TransTable::RowElement::RowElement()
{
	_is_empty = true;
}


TransTable::RowElement::RowElement(const RowElement &element)
{
	_present_stateID = element._present_stateID;

	for( unsigned int i = 0; i < element._next_stateIDs.size(); i++ )
	{
		_next_stateIDs.push_back(element._next_stateIDs[i]);
	}

	for( unsigned int i = 0; i < element._outputs.size(); i++ )
	{
		_outputs.push_back(element._outputs[i]);
	}

	_is_empty = false;
}


bool TransTable::RowElement::operator==(const RowElement &row) const
{
	if( _next_stateIDs.size() != row._next_stateIDs.size() )
		return false;

	if( _outputs.size() != row._outputs.size() )
		return false;

	//if( _present_stateID != row._present_stateID )
	//  return false;

	for( unsigned int i = 0; i < _next_stateIDs.size(); i++ )
		if( _next_stateIDs[i] != row._next_stateIDs[i] )
			return false;

	for( unsigned int i = 0; i < _outputs.size(); i++ )
		if( _outputs[i] != row._outputs[i] )
			return false;

	return true;
}


