#include "FormatNormalizer.h"


ostream &operator<<(ostream &os, FormatNormalizer &norm)
{
  os << "<TSD version=\"1.0\">\n";

  os << "<head>\n";
  os << "<stateNo>" << norm._nState << "</stateNo>\n";
  os << "<transitionNo>" << norm._nTransition << "</transitionNo>\n";
  os << "<inputNo>" << norm._nInput << "</inputNo>\n";
  os << "<outputNo>" << norm._nOutput << "</outputNo>\n";
  os << "<type>" << ((norm._machine_type == MealyMachine) ? "mealy" : "moore") << "</type>\n";
  os << "</head>\n";

  for( unsigned int i = 0; i < norm._state.size(); i++ )
  {
    os << "<state>\n";
    os << "<id>" << norm._state[i]._ID << "</id>\n";
    os << "<name>" << norm._state[i]._name << "</name>\n";
    os << "<output>" << norm._state[i]._output << "</output>\n";
    os << "</state>\n";
  }

  for( unsigned int i = 0; i < norm._transition.size(); i++ )
  {
    os << "<transition>\n";
    os << "<from>" << norm._transition[i]._present_stateID << "</from>\n";
    os << "<to>" << norm._transition[i]._next_stateID << "</to>\n";
    os << "<input>" << norm._transition[i]._input << "</input>\n";
    os << "<output>" << norm._transition[i]._output << "</output>\n";
    os << "</transition>\n";
  }

  return os;
}


istream &operator>>(istream &is, FormatNormalizer &norm)
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
	norm._nState = atoi(textline.c_str() + 9);
      }
      else if( strncmp(textline.c_str(), "<transitionNo>", 14) == 0 )

      {
	norm._nTransition = atoi(textline.c_str() + 14);
      }
      else if( strncmp(textline.c_str(), "<inputNo>", 9) == 0 )
      {
	norm._nInput = atoi(textline.c_str() + 9);
      }
      else if( strncmp(textline.c_str(), "<outputNo>", 10) == 0 )
      {
	norm._nOutput = atoi(textline.c_str() + 10);
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
	  norm._machine_type = MealyMachine;
	else if( machine_type == string("moore") )
	  norm._machine_type = MooreMachine;
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

      FormatNormalizer::State temp;

      temp._ID = stateID;
      temp._name = stateName;
      temp._output = stateOutput;

      norm._state.push_back(temp);
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


      FormatNormalizer::RowElement temp;
      
      temp._present_stateID = transitionFrom;
      temp._next_stateID = transitionTo;
      temp._output = transitionOutput;

      if( transitionInput.find('d') != string::npos )
      {
	vector<string> result;
	ExpandDontCare(result, transitionInput, 0);

	for( unsigned int i = 0; i < result.size(); i++ )
	{
	  temp._input = result[i];

	  norm._transition.push_back(temp);
	}
      }
      else
      {
	temp._input = transitionInput;
	norm._transition.push_back(temp);
      }
    }
    else
      cerr << "class TransTable: Not supported label" << endl;
  }

  sort(norm._state.begin(), norm._state.end());
  sort(norm._transition.begin(), norm._transition.end());

  vector<FormatNormalizer::RowElement> temp;
  vector<string> bitCombo;

  for( int i = 0; i < (1 << norm._nInput); i++ )
  {
    string str;

    str.assign(norm._nInput, '0');

    for( int j = (norm._nInput-1); j >= 0 ; j-- )
    {
      if( (i & (1 << j)) > 0 )
	str[norm._nInput - j - 1] = '1';
      else
	str[norm._nInput - j - 1] = '0';
    }

    bitCombo.push_back(str);
  }
 
  //for( unsigned int i = 0; i < bitCombo.size(); i++ )
  //{
  //  cout << bitCombo[i] << endl;
  //}
  
  for( unsigned int i = 0, j = 0; i < norm._transition.size(); i++, j++ )
  {
    if( bitCombo[j % (1 << norm._nInput)] == norm._transition[i]._input )
    {
      temp.push_back( norm._transition[i] );
    }
    else if( bitCombo[j % (1 << norm._nInput)] < norm._transition[i]._input )
    {
      FormatNormalizer::RowElement row;

      row._present_stateID = norm._transition[i]._present_stateID;
      row._next_stateID = norm._transition[i]._next_stateID;
      row._input = bitCombo[i % (1 << norm._nInput)];
      row._output = string().assign(norm._nOutput, 'd');
      
      temp.push_back( row );
      i--;
    }
    else if( bitCombo[j % (1 << norm._nInput)] > norm._transition[i]._input )
    {
      FormatNormalizer::RowElement row;

      row._present_stateID = norm._transition[i-1]._present_stateID;
      row._next_stateID = norm._transition[i-1]._next_stateID;
      row._input = bitCombo[i % (1 << norm._nInput)];
      row._output = string().assign(norm._nOutput, 'd');

      temp.push_back( row );
      i--;
    }
  }

  norm._transition.clear();

  for( unsigned int i = 0; i < temp.size(); i++ )
  {
    norm._transition.push_back(temp[i]);
  }
  
  return is;
}


void ExpandDontCare(vector<string> &result, string &s, unsigned int index)
{
  if( index == s.size() )
  {
    result.push_back(s); 
    return;
  }

  if( s[index] == 'd' )
  {
    s[index] = '0';
    ExpandDontCare(result, s, index+1);
    s[index] = '1';
    ExpandDontCare(result, s, index+1);
    s[index] = 'd';
  }
  else
  {
    ExpandDontCare(result, s, index+1);
  }
}


FormatNormalizer::RowElement::RowElement()
{
  _is_empty = true;
}


FormatNormalizer::RowElement::RowElement(const RowElement &element)
{
  _present_stateID = element._present_stateID;
  _input = element._input;
  _next_stateID = element._next_stateID;
  _output = element._output;

  _is_empty = false;
}


bool FormatNormalizer::RowElement::operator<(const RowElement &element) const
{
  int a = atoi(_present_stateID.c_str());
  int b = atoi(element._present_stateID.c_str());
  int c = atoi(_next_stateID.c_str());
  int d = atoi(element._next_stateID.c_str());

  if( a < b )
    return true;
  else if( a == b )
  {
    if( c < d )
      return true;
    else if( c == d )
    {
      if( _input < element._input )
	return true;
      else
	return false;
    }
    else
      return false;
  }
  else
    return false;
}


FormatNormalizer::State::State()
{

}


FormatNormalizer::State::State(const State &state)
{
  _ID = state._ID;
  _name = state._name;
  _output = state._output;

}



bool FormatNormalizer::State::operator<(const State &state) const
{
  int a = atoi(_ID.c_str());
  int b = atoi(state._ID.c_str());

  if( a < b )
    return true;
  else 
    return false;

}

