#ifndef FORWARD_H
#define FORWARD_H

#include <string>

using std::string;

void forward(const string &stateFileName, const string &circuitFileName);
void forward(const string &stateFileName, const string &circuitFileName, const string &encodingFile);

#endif
