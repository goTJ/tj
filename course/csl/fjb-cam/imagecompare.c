#include<stdio.h>
#include<math.h>
#include "inc.h"


int imagecompare(unsigned char[][],unsigned char[][], int threshold, unsigned char[][]);
int imagecompare(unsigned char image1[WIDTH][HEIGHT],unsigned char image2[WIDTH][HEIGHT], int threshold, unsigned char mask[WIDTH][HEIGHT]){
	int i,j,k,l;
	
	int counter=0;
	int counter2=0;
	int temp;
	const int t_minus = threshold/WIDTH/HEIGHT;
	
	fprintf(stderr, "t_minus = %d\n",t_minus);

	for(i=0;i<WIDTH;i++){
		for(j=0;j<HEIGHT;j++){
			temp=(image1[i][j]-image2[i][j]);
			counter+=temp*temp*mask[i][j];
			threshold-=((mask[i][j]+1)%2)*t_minus;
			counter2+=(mask[i][j]+1)%2;

		}
	}
	fprintf(stderr, "threshold = %d\n",threshold);
	fprintf(stderr, "numberof1inmask = %d\n",counter2);
	if(counter>threshold)
		return 1;
	else
		return 0;
}

