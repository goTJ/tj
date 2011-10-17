#include "TRIModel.h"
#include <cctype>
#include <cstdio>


bool TRIModel::loadFromFile(const char* fileName){
	char buf[4][200];
	char str[200];
	int buf_idx=0;
	double max[3]={0.0, 0.0, 0.0};
	double min[3]={0.0, 0.0, 0.0};

	FILE* inFile = fopen(fileName, "r");
	if(!inFile){
        printf("Can't open object file %s\n", fileName);
		return false;
	}

    printf("Loading \"%s\"\n", fileName);
	while(1){
        char *result;
        if((result=fgets(str, 200, inFile)) != NULL){
            char *p=str;
    		while(isspace(*p))
                p++;
    		if(!isalpha(*p)){
                if(buf_idx < 4)
           			strcpy(buf[buf_idx++], p);
           		continue;
            }
		}
		if(buf_idx < 3){
			buf_idx = 0;
			continue;
		}
		int tmp_int[6] = { 255, 127, 127, 63, 63, 63 };
		double tmp_double[6];
		int p=0;
		if(buf_idx >= 4)
			sscanf(buf[p++],"%d %d %d %d %d %d",&tmp_int[0],&tmp_int[1], &tmp_int[2], &tmp_int[3], &tmp_int[4], &tmp_int[5]);
		Triangle triangle(tmp_int);
		for(int i=0; i<3; i++){
			sscanf(buf[p++],"%lf %lf %lf %lf %lf %lf",&tmp_double[0],&tmp_double[1], &tmp_double[2], &tmp_double[3], &tmp_double[4], &tmp_double[5]);
			for(int j = 0; j < 3; j++){
				if(tmp_double[j] < min[j]){
					min[j] = tmp_double[j];
				}
				if(tmp_double[j] > max[j]){
					max[j] = tmp_double[j];
				}
			}
			triangle.loadVertex(i, tmp_double);
		}
		triangleList.push_back(triangle);
		buf_idx = 0;
		if(result == NULL)
            break;
	}
	for(int i = 0; i < 3; i++){
		center[i] = (min[i] + max[i]) / 2;
	}
	return true;
}
void TRIModel::copy(TRIModel * t){
	for(int i = 0; i < 3; i++){
		center[i] = t->center[i];
	}
	triangleList = t->triangleList;
}

TRIModel::TRIModel(){
}

TRIModel::~TRIModel(){
}
