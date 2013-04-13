// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <gil/gil.h>
#include <gil/dip.h>

using namespace std;
using namespace gil;

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Usage: %s list_file\n", argv[0]);
		return 0;
	}
	const char *listfile=argv[1];
	FILE *fin=fopen(listfile, "r");

	if(fin == NULL){
		fprintf(stderr, "Can't open file %s\n", listfile);
		return 1;
	}
	char imagename[100];
	while(fscanf(fin, "%s", imagename) == 1){
		printf("imagename: %s\n", imagename);
		ByteImage3 image;
		read(image, imagename);
		Convert<RgbToGray> convert;
		FloatImage1 gray_image = convert(image);
		int color_hist[128];
		for(int y=0, y<image.height(); y++){
			for(int x=0; x<image.width(); x++){
			}
		}
	}
	return 0;
}
