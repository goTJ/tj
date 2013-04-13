// vim:ft=cpp:foldmethod=marker
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

bool ok(string a, string b)
{
	for(unsigned int i=0; i<a.size(); i++){
		if(a[i]!=b[i] && b[i]!='-')
			return false;
	}
	return true;
}

int main()
{
	vector<string> ptns, ans;
	vector<unsigned int> count;
	string s;
	int numPI;
	int numTruePtn;
	int tmp;

	cin >> tmp >> numTruePtn >> tmp;
	while(getline(cin, s)){
		if(s == "=== Patterns ==="){
			while(getline(cin, s)){
				if(s == "=== END ===")
					break;
				ptns.push_back(s);
			}
		}
		if(s.find("covering problem") != string::npos){
			numPI = atoi(s.substr(s.find("(")+1, s.find(")")-s.find("(")-1).c_str());
		}
		if(s == "=== Ans ==="){
			getline(cin, s);	// ignore total line
			while(getline(cin, s)){
				if(s == "=== END ===")
					break;
				ans.push_back(s);
				count.push_back(0);
			}
		}
	}
	cout << "number of Patterns: " << ptns.size() << endl;
	cout << "number of True Patterns: " << numTruePtn << endl;
	cout << "number of PIs: " << numPI << endl;
	cout << "number of ans PIs: " << ans.size() << endl;
	for(int i=0; i<numTruePtn; i++){
		bool isOK=false;
		for(unsigned int j=0; j<ans.size(); j++){
			if(ok(ptns[i], ans[j])){
				isOK = true;
				count[j]++;
			}
		}
		if(!isOK){
			cout << "Incorrect! Can't include all patterns." << endl;
			return 0;
		}
	}
	for(unsigned int i=numTruePtn; i<ptns.size(); i++){
		for(unsigned int j=0; j<ans.size(); j++){
			if(ok(ptns[i], ans[j]))
				count[j]++;
		}
	}
	for(unsigned int i=0; i<ans.size(); i++){
		unsigned int sum=1;
		for(unsigned int j=0; j<ans[i].size(); j++){
			if(ans[i][j] == '-')
				sum <<= 1;
		}
		if(count[i] != sum){
			cout << "Incorrect! Wrong patterns." << endl;
			return 0;
		}
	}
	cout << "Correct!" << endl;
	return 0;
}
