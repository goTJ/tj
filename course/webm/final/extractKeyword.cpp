// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cmath>
#include <string>
#include <list>
#include <algorithm>
#include <set>

#define MAX_SCENE_SIZE 50

using namespace std;

string theString;

bool isStr(const string &s){ // {{{
	if(s == theString)
		return true;
	return false;
} // }}}
bool isSuperstr(const string &s){ // {{{
	size_t found=s.find(theString);
	if(s.size() == theString.size())
		return false;
	if(found!=string::npos)
		return true;
	return false;
} // }}}
bool isSubstr(const string &s){ // {{{
	size_t found=theString.find(s);
	if(s.size() == theString.size())
		return false;
	if(found!=string::npos)
		return true;
	return false;
} // }}}
double sqr(double a){ // {{{
	return a*a;
} // }}}
bool canSkip(const string &s){ // {{{
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�A") != string::npos) return true;
	if(s.find("�C") != string::npos) return true;
	if(s.find("�B") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�I") != string::npos) return true;
	if(s.find("�H") != string::npos) return true;
	if(s.find("�G") != string::npos) return true;
	if(s.find("�u") != string::npos) return true;
	if(s.find("�]") != string::npos) return true;
	if(s.find("�^") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�@") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�h") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�F") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�b") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�W") != string::npos) return true;
	if(s.find("�U") != string::npos) return true;
	if(s.find("�H") != string::npos) return true;
	if(s.find("�C") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�S") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�o") != string::npos) return true;
	if(s.find("�O") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("��") != string::npos) return true;
	if(s.find("�~") != string::npos) return true;
	if(s.find("�]") != string::npos) return true;
	if(s.find("�[��") != string::npos) return true;
	if(s.find("�j�a") != string::npos) return true;
	return false;
} // }}}
int main(int argv, char *argc[]) // {{{
{
	if(argv < 2){
		printf("Usage: %s data_set\n", argc[0]);
		return 0;
	}
	const char *dataFilename=argc[1];
	FILE *fin=fopen(dataFilename, "rb");
	if(fin == NULL){
		fprintf(stderr, "Can't open file %s\n", dataFilename);
		return 1;
	}

	char buf[128];
	string sceneName[MAX_SCENE_SIZE];
	list<string> keywords[MAX_SCENE_SIZE];
	list<string> general;
	//set<string> stopList;
	int sceneSize=0;
	while(fgets(buf, 2048, fin) != NULL){
		if(buf[0] == '='){
			buf[strlen(buf)-5] = '\0';
			sceneName[sceneSize++] = buf+4;
			//stopList.insert(buf+4);
		}else{
			break;
		}
	}
	while(fscanf(fin, "%s", buf) == 1){
		//printf("%s\n", buf);
		buf[strlen(buf)-3] = '\0';
		char *keyword=buf+3;
		double ratio[MAX_SCENE_SIZE];
		for(int i=0; i<sceneSize; i++)
			fscanf(fin, "%lf", &ratio[i]);
		for(int sid=0; sid<sceneSize; sid++){
			if(ratio[sid] > 0.2){
				double rest=0.0;
				for(int i=0; i<sceneSize; i++){
					if(i != sid)
						rest += ratio[i];
				}
				rest /= sceneSize-1;
				if(rest < 0.00000001 || ratio[sid]/rest > 2.5){
					keywords[sid].push_back(keyword);
				}
			}
		}
		double avg=0.0, var=0.0;
		for(int sid=0; sid<sceneSize; sid++)
			avg += ratio[sid];
		avg /= sceneSize;
		for(int sid=0; sid<sceneSize; sid++)
			var += sqr(ratio[sid]-avg);
		var = sqrt(var/(sceneSize-1));
		if(avg > 0.1 && var < 0.08)
			general.push_back(keyword);
	}
	for(int sid=0; sid<sceneSize; sid++){
		list<string> t=keywords[sid];
		for(list<string>::iterator iter=t.begin(); iter!=t.end(); iter++){
			theString = *iter;
			keywords[sid].remove_if(isSubstr);
		}
		keywords[sid].remove_if(canSkip);
		if(!keywords[sid].empty()){
			printf("%s:", sceneName[sid].c_str());
			for(list<string>::iterator iter=keywords[sid].begin(); iter!=keywords[sid].end(); iter++){
				//if(stopList.find(*iter) == stopList.end())
				if(*iter != sceneName[sid])
					printf(" %s", iter->c_str());
			}
			printf("\n");
		}
	}
	general.remove_if(canSkip);
	printf("General:");
	for(list<string>::iterator iter=general.begin(); iter!=general.end(); iter++)
		printf(" %s", iter->c_str());
	printf("\n");
	/*
	printf("stop:");
	for(set<string>::iterator iter=stopList.begin(); iter!=stopList.end(); iter++)
		printf(" %s", iter->c_str());
	printf("\n");
	*/
	return 0;
} // }}}
