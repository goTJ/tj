// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cstring>
#include <string>
#include <set>
#include <map>

#define TIMES_THRESHOLD 0.01

using namespace std;

struct TermInfo{
	string name;
	double sceneProb[50];
};

string myBase64(char *p, int n){ // {{{
	char buf[32];

	for(int i=0; i<n; i++){
		char c=*(p+i);
		int hex=(c&0xf0)>>4;
		buf[(i<<1)] = (hex < 10) ? '0'+hex : 'a'+hex-10;
		hex = (c&0x0f);
		buf[(i<<1)+1] = (hex < 10) ? '0'+hex : 'a'+hex-10;
	}
	buf[(n<<1)] = '\0';
	return buf;
} // }}}

int main(int argv, char *argc[])
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

	set<string> allTerms;
	map<string, double> allTermsTimes;
	set<string> terms[50][302];
	char buf[65536];
	int sceneSize=-1;
	int dataSize[50]={0};
	int contentId=0;
	while(fgets(buf, 65536, fin) != NULL){
		if(buf[0] == '='){
			sceneSize++;
		}else{
			dataSize[sceneSize]++;
		}
	}
	sceneSize = -1;
	rewind(fin);
	while(fgets(buf, 65536, fin) != NULL){
		buf[strlen(buf)-1] = '\0';
		if(buf[0] == '='){
			printf("%s\n", buf);
			fprintf(stderr, "%s\n", buf);
			contentId = 0;
			sceneSize++;
		}else{
			for(char *p=buf; *p; p+=2){
				char b[16];
				strncpy(b, p, 8);
				b[8] = '\0';
				if(strlen(b) == 8){
					terms[sceneSize][contentId].insert(b);
					allTermsTimes[b] += 1.0/dataSize[sceneSize];
					if(allTermsTimes[b] > TIMES_THRESHOLD){
						allTerms.insert(b);
					}
				}
				b[6] = '\0';
				if(strlen(b) == 6){
					terms[sceneSize][contentId].insert(b);
					allTermsTimes[b] += 1.0/dataSize[sceneSize];
					if(allTermsTimes[b] > TIMES_THRESHOLD){
						allTerms.insert(b);
					}
				}
				b[4] = '\0';
				if(strlen(b) == 4){
					terms[sceneSize][contentId].insert(b);
					allTermsTimes[b] += 1.0/dataSize[sceneSize];
					if(allTermsTimes[b] > TIMES_THRESHOLD){
						allTerms.insert(b);
					}
				}
			}
			contentId++;
		}
	}
	printf("%d\n", allTerms.size());
	int count=0;
	// for(map<string, int>::iterator iter=allTerms.begin(); iter != allTerms.end(); iter++){
	for(set<string>::iterator iter=allTerms.begin(); iter != allTerms.end(); iter++){
		fprintf(stderr, "%d / %d\n", count++, allTerms.size());
		TermInfo termInfo;
		termInfo.name = *iter;
		for(int sid=0; sid<sceneSize; sid++){
			int total=0, has=0;
			for(int cid=0; !terms[sid][cid].empty(); cid++){
				if(terms[sid][cid].find(*iter) != terms[sid][cid].end())
					has++;
				total++;
			}
			termInfo.sceneProb[sid] = (double)has/total;
		}
		printf("<<<%s>>>", termInfo.name.c_str());
		for(int sid=0; sid<sceneSize; sid++)
			printf(" %.4lf", termInfo.sceneProb[sid]);
		printf("\n");
	}
	return 0;
}
