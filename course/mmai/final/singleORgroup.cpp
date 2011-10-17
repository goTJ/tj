#include <iostream>
#include "gil/gil.h"
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>	
#include <time.h>

using namespace std;
using namespace gil;

int main(int argc, char **argv)
{

	//-- FILE I/O --//
	FILE *infptr,*outfptr1,*outfptr2;
	char outfname1[256];
	char outfname2[256];

	// check
	if(argc < 1){
		printf("parameter error!\n");
		exit(1);
	}

	strcpy(outfname1,argv[1]);
	strcat(outfname1,"_1");
	strcpy(outfname2,argv[1]);
	strcat(outfname2,"_2");
	if( (infptr = fopen(argv[1],"r")) == NULL ){
		printf("listfile %s is not exist!\n",argv[1]);
		exit(1);
	}
	if( (outfptr1 = fopen(outfname1,"w")) == NULL ){
		        printf("listfile %s can't open!\n",outfname1);
			        exit(1);
	}
	if( (outfptr2 = fopen(outfname2,"w")) == NULL ){
		        printf("listfile %s can't open!\n",outfname2);
			        exit(1);
	}

	// for each image
	char line[256];
	char filepath[256];
	char infofilepath[256];
	FILE *tmpfptr;
	while( fgets(line,256,infptr) != NULL ){
		sscanf(line,"%s",&filepath);
		sprintf(infofilepath,"%s.info",filepath);
		
		int faceNum;
		int imgW,imgH;
		int maskX,maskY,maskW,maskH;
		if( (tmpfptr = fopen(infofilepath,"r")) != NULL ){
			fscanf(tmpfptr,"%d\n",&faceNum);
			
			// if mask area big enough, belong to people
			if(faceNum > 1)
				fprintf(outfptr2,"%s\n",filepath);
			else if(faceNum == 1)
				fprintf(outfptr1,"%s\n",filepath);				
		}
		else	// no mask file,  belong to scene
			printf("error!\n");
	
	}// end while
	
	return 0;
}
