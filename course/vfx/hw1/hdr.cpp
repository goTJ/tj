#include <cmath>
#include <gsl/gsl_linalg.h>
#include <gil.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <values.h>

using namespace Gil;
using std::vector;
using std::pair;

#define MAX_FILENAME_LENGTH 80

extern char* optarg;
extern int optind, opterr;

inline void usage(char* program) { // {{{
	printf("Usage:\t%s [-n number_of_samples] [-w output_filename] [-l lamda] -c config_file\n"
			"\t%s -h\n",
			program, program);
} // }}}

inline void help() { // {{{
	printf("-c config_file\n"
			"\t\tThe format is FILENAME SHUTTER_SPEED per single line.\n");
} // }}}

int main(int argc, char** argv) {
	char config_filename[MAX_FILENAME_LENGTH + 1] = "";
	vector<pair<ByteImage*, float> > imageList;
	int imageWidth, imageHeight, imageChannel;
	int num_of_samples = 50;
	char output_filename[MAX_FILENAME_LENGTH + 1] = "test.hdr";
	float lamda = 100;

	srandom(time(NULL));

	//  Arguments list setting {{{
	{
		opterr = 0;

		char c;							
		while ((c = getopt(argc, argv, "l:w:n:hc:")) != -1) {
			switch (c) {
				case 'l':
					lamda = atof(optarg);
					break;
				case 'n':
					num_of_samples = atoi(optarg);
					break;
				case 'h':
					help();
					exit(0);
					break;

				case 'c':
					strncpy(config_filename, optarg, MAX_FILENAME_LENGTH);
					config_filename[MAX_FILENAME_LENGTH] = '\0';
					break;

				case 'w':
					strncpy(output_filename, optarg, MAX_FILENAME_LENGTH);
					config_filename[MAX_FILENAME_LENGTH] = '\0';
					break;
					
				default:
					usage(argv[0]);
					exit(0);
					break;
			}
		}

		if (strlen(config_filename) == 0) {
			usage(argv[0]);
			exit(0);
		}
		printf("%-30s%s\n", "Using configuration file: ", config_filename);
	}
	printf("%-30s%d\n", "The number of samples:", num_of_samples);
	// }}}

	// Read configuration file and Build _imageList_. (imageList) {{{
	{
		FILE* config_file = fopen(config_filename, "r");
		char filename[MAX_FILENAME_LENGTH+1];
		float time;
		while (fscanf(config_file, "%s %f", filename, &time) > 0) {
			ByteImage* image = new ByteImage;
			image->readFile(filename);
			printf("%-30s%-s\n", "Reading file: ", filename);
			imageList.push_back(pair<ByteImage *, float>(image, time));
		}

		printf("%-30s%d\n", "The size of image list: ", imageList.size());
	}
	// }}}

	// Check images and determine _imageWidth_, _imageHeight_, and _imageChannel (imageWidth, imageHeight, imageChannel) {{{
	{
		if (imageList.size() <= 0) {
			fprintf(stderr, "ERROR: You should at least provide one correct filename of image in configuration file");
			exit(0);
		}

		ByteImage* image = imageList[0].first;
		imageWidth = image->width();
		imageHeight = image->height();
		imageChannel = image->channels();
		for (int imageCnt = 1; imageCnt < imageList.size(); ++imageCnt) {
			image = imageList[imageCnt].first;
			if (image->width() != imageWidth || image->height() != imageHeight
					|| image->channels() != imageChannel) {
				fprintf(stderr, "ERROR: The %d-th image has different size to the first image.",
						imageCnt);
				exit(0);
			}
		}
		printf("%-30s%d\n", "Image width:", imageWidth);
		printf("%-30s%d\n", "Image height:", imageHeight);
	}
	// }}}

	vector<double> colorScale(3);
	colorScale[0] = 0;
	colorScale[1] = 0;
	colorScale[2] = 0;
	vector<pair<int, int> > samplePositions;

	// Randomly sample _num_of_samples_ samples from image. (samplePositions) {{{
	{
		// Check if image has more than _num_of_samples_ pixels. {{{
		if (imageWidth * imageHeight < num_of_samples) {
			fprintf(stderr, "ERROR: You should choose bigger images (with more than %d pixels)!",
					num_of_samples);
			exit(0);
		}
		// }}}

		// Sample by discarding {{{
		{
			printf("Generating samples...");
			while (samplePositions.size() < num_of_samples) {
				int x, y;
				x = random()%imageHeight;
				y = random()%imageWidth;
				samplePositions.push_back(pair<int, int>(x, y));
				printf(".");
				fflush(stdout);
			}
			printf("\n");
		}
		// }}}
	}
	// }}}

	FloatImage outputImage(imageWidth, imageHeight, imageChannel);
	for (int channel = 0; channel < imageChannel; ++channel) {
		printf("\n--- Processing channel #%d ---\n", channel);
		int zmax  = -1, zmin = 256;
		int zmid;

		// Find the values of _zmax_ and _zmin_ (zmax, zmin) {{{
		{
			printf("Calculating Zmax and Zmin...");
			for (int imageCnt = 0; imageCnt < imageList.size(); ++imageCnt) {
				ByteImage& image = *imageList[imageCnt].first;
				for (int row = 0; row < imageHeight; ++row) {	// Gil uses column first.
					for (int col = 0; col < imageWidth; ++col) {
						if (image(col, row, channel) > zmax) {
							zmax = image(col, row, channel);
						}
						if (image(col, row, channel) < zmin) {
							zmin = image(col, row, channel);
						}
					}
				}
				printf(".");
				fflush(stdout);
			}
			printf("\n");

			zmid = (zmax+zmin)/2;
			zmin = zmin + (zmax - zmin)/10;
			zmax = zmax - (zmax - zmin)/10;

			printf("%-30s%d\n", "Zmax:", zmax);
			printf("%-30s%d\n", "Zmin:", zmin);
			printf("%-30s%d\n", "Zmid:", zmid);
		}
		// }}}

		const int num_of_shots		= imageList.size();
		const int num_of_variables	= num_of_samples + 256;

		/* Number of Equations = Data Terms + Smooth Terms + Additional Constraint */
		const int num_of_equations = (num_of_shots * num_of_samples) + (zmax - zmin - 1) + 1;


		printf("Callocating matrices and vectors...");
		fflush(stdout);
		gsl_matrix* A = gsl_matrix_calloc(num_of_equations, num_of_variables);
		gsl_matrix* V = gsl_matrix_calloc(num_of_variables, num_of_variables);
		gsl_vector* S = gsl_vector_calloc(num_of_variables);
		gsl_vector* work = gsl_vector_calloc(num_of_variables);
		gsl_vector* b = gsl_vector_calloc(num_of_equations);
		gsl_vector* x = gsl_vector_calloc(num_of_variables);
		printf("\n");

		printf("Filling in the matrix _A_ and vector _b_..."); // {{{
		{
			int eqCnt = 0;
#define WEIGHTING(Z) ((Z <= zmin || Z >= zmax) ? 0 : ( Z<=zmid ? (Z - zmin) : (zmax - Z)))

			// Data Terms (A, b) {{{
			{
				for (int imageCnt = 0; imageCnt < imageList.size(); ++imageCnt) {
					ByteImage& image = *imageList[imageCnt].first;
					const double shutter_speed = imageList[imageCnt].second;

					for (int sampleCnt = 0; sampleCnt < num_of_samples; ++sampleCnt, ++eqCnt) {
						const pair<int, int>& samplePosition = samplePositions[sampleCnt];
						int pixelValue = image(samplePosition.second, samplePosition.first);
						gsl_matrix_set(A, eqCnt, pixelValue, 1*WEIGHTING(pixelValue));
						gsl_matrix_set(A, eqCnt, 256 + sampleCnt, -1*WEIGHTING(pixelValue));
						gsl_vector_set(b, eqCnt, log(shutter_speed)*WEIGHTING(pixelValue));
					}
				}
			} // }}}

			// Smooth Terms (A, b) {{{
			{
				for (int z = zmin + 1; z <= zmax - 1; ++z, ++eqCnt) {
					gsl_matrix_set(A, eqCnt, z - 1,	lamda * WEIGHTING(z));
					gsl_matrix_set(A, eqCnt, z,		-2 * lamda * WEIGHTING(z));
					gsl_matrix_set(A, eqCnt, z + 1,	lamda * WEIGHTING(z));
					gsl_vector_set(b, eqCnt, 0);
				}
			}
			// }}}

			// Constraint (A, b) {{{
			{
				const static float LARGE_FLOAT = 1000000000;
				gsl_matrix_set(A, eqCnt, zmid, LARGE_FLOAT);
				gsl_vector_set(b, eqCnt, LARGE_FLOAT * colorScale[channel]);
			}
			// }}}

			printf("%s\n%30s%d\n", "The number of Equations in matrix A:", "",eqCnt + 1);
			assert(eqCnt == (num_of_shots * num_of_samples + zmax - zmin - 1));

			/*
			   for (int cnt1 = 0; cnt1 < num_of_equations; ++cnt1) {
			   for (int cnt2 = 0; cnt2 < num_of_variables; ++cnt2) {
			   printf("%f ", gsl_matrix_get(A, cnt1, cnt2));
			   }
			   printf("\n");
			   }
			 */


		} // }}}

		gsl_linalg_SV_decomp (A, V, S, work);
		gsl_linalg_SV_solve(A, V, S, b, x);

		for (int cnt = 0; cnt < num_of_variables; ++cnt) {
			printf("%d %f\n", cnt, gsl_vector_get(x, cnt));
		}
		printf("\n");

		// Construct the new image {{{
		{

			printf("Reconstructing HDR image...");
			for (int row = 0; row < imageHeight; ++row) {	// Gil uses column first.
				for (int col = 0; col < imageWidth; ++col) {
					// Use weighting sum {{{
					{
						double weightingSum = 0;
						double weightedSum = 0;

						for (int imageCnt = 0; imageCnt < imageList.size(); ++imageCnt) {
							ByteImage& image = *imageList[imageCnt].first;
							double delT = imageList[imageCnt].second;
							int z = image(col, row, channel);

#define G(z) gsl_vector_get(x, z)
							weightingSum += WEIGHTING(z);
							weightedSum += WEIGHTING(z) * (G(z) - log(delT));
						}

						weightedSum /= weightingSum;
						outputImage(col, row, channel) = exp(weightedSum);
					}
					// }}}
					// Use Median Filter {{{
					/*{
					  vector<double> v;
					  for (int imageCnt = 0; imageCnt < imageList.size(); ++imageCnt) {
					  ByteImage& image = *imageList[imageCnt].first;
					  double delT = imageList[imageCnt].second;
					  int z = image(col, row, channel);
					  v.push_back(G(z) - log(delT));
					  }
					  sort(v.begin(), v.end());
					  outputImage(col, row, channel) = exp(v[v.size()/2]);
					  }*/
					// }}}
				}
				printf(".");
				fflush(stdout);
			}
		}
		printf("\n");
		// }}}

		printf("%-30s%f:", "Zmid's value:", gsl_vector_get(x, zmid));
		fflush(stdout);
	}
	outputImage.writeFile(output_filename);
}
