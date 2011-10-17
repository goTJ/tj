#include "HW3.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void MatrixGeneration( double **A, double *b, int n );
double ErrorEstimation( double **A, double *b, double *x, int n );

int main()
{
	int i, n;
	double **A, *b, *x;

	n=1000;

	x = new double [n];
	b = new double [n];
	A = new double *[n];
	for( i=0; i<n ;i++ )
		A[i] = new double [n];
	
//---------- Timer Start -----------------
	clock_t c1,c2;
	double time_cost;
	c1 = clock();
//----------------------------------------

	MatrixGeneration( A, b, n );
	SolveLinearSystem( A, b, x, n );
	printf( "%40.30lf\n", ErrorEstimation( A, b, x, n ) );

//---------- Timer End -------------------
	c2 = clock();
	time_cost = 1.0*(c2-c1)/CLOCKS_PER_SEC;
	printf("time = %.3f sec\n", time_cost );
//----------------------------------------

	delete b;
	delete x;
	for( i=0; i<n; i++ )
		delete A[i];
	delete A;
	return 0;
}

void MatrixGeneration( double **A, double *b, int n )
{
	int i,j;
	double *u, *v, temp, u_len, v_len;

	u = new double [n];
	v = new double [n];

	for( i=0; i<n; i++ )
	for( j=0; j<n; j++ )
		A[i][j] = 0.0;

	u_len = 0.0;
	v_len = 0.0;

	srand( time(0) );
	for( i=0; i<n; i++ )
	{
		u[i]    = (double) rand() / RAND_MAX
			;
		v[i]    = (double) rand() / RAND_MAX;
		b[i]    = (double) rand() / RAND_MAX;
		A[i][i] = (double) rand() / RAND_MAX;

		u_len += u[i]*u[i];
		v_len += v[i]*v[i];

		b[i]    = 1 + 99.0*b[i];
		A[i][i] = 1 + 99.0*A[i][i];
	}

	u_len = sqrt( u_len );
	v_len = sqrt( v_len );
	for( i=0; i<n; i++ )
	{
		u[i] /= u_len;
		v[i] /= v_len;
	}

	temp = 0.0;
	for( i=0; i<n; i++ )
		temp += u[i]*A[i][i]*v[i];

	for( i=0; i<n; i++ )
	for( j=0; j<n; j++ )
		A[i][j] = A[i][j] - 2*u[i]*u[j]*A[j][j] - 2*v[i]*v[j]*A[i][i] + 4*temp*u[i]*v[j];

	delete u;
	delete v;
}

double ErrorEstimation( double **A, double *b, double *x, int n )
{
	double b1, error;

	error = 0.0;
	for( int i=0; i<n; i++ )
	{
		b1 = 0;
		for( int j=0; j<n; j++ )
			b1 += A[i][j]*x[j];
		error += (b[i] - b1) * (b[i] - b1) ;
	}


	return log(error)/log(10);
}
