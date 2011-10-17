// vim:ft=c:foldmethod=marker
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// {{{ void checkLU(double **A, double **lu, int n)
void checkLU(double **A, double **lu, int n)
{
	double tmp;
	int i, j, k;
	int min;
	double **B=(double **)malloc(sizeof(double*)*n);
	int flag=1;

	for(i=0; i<n; i++)
		B[i] = (double *)malloc(sizeof(double)*n);
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			min = (i<j) ? i : j;
			tmp = 0;
			for(k=0; k<min; k++)
				tmp += lu[i][k]*lu[k][j];
			if(j >= i)
				tmp += lu[i][j];
			else
				tmp += lu[i][k]*lu[k][j];
			B[i][j] = tmp;
			if(A[i][j]-tmp > 0.00001 || A[i][j]-tmp < -0.00001)
				flag = 0;
		}
	}
	/*
	for(i=0; i<n; i++){
		for(j=0; j<n; j++)
			printf("%10.3lf ", B[i][j]);
		printf("\n");
	}
	*/
	printf("check done: %d\n", flag);
} // }}}

void SolveLinearSystem(double **A, double *b, double *x, int n)
{
	int i, j, k;
	int *pi=(int *)malloc(sizeof(int)*n);
	double *y;
	double **lu;
	double *taj;
	double **tlu, **tlu_end, *tlui, *tlui_begin, *tluj, *tluj_end, tluji, tluii;
	double *tp;
	double tmp;
	int pii;

// {{{ LUP Decomposition
	// {{{ allocate memory
	lu = (double**)malloc(sizeof(double*)*n);
	for(i=0; i<n; i++)
		lu[i] = (double*)malloc(sizeof(double)*n); // }}}
	// {{{ do i=0 especially for save A to lu
		// {{{ find A[j][0]!=0 from j=0 to n-1, set pi[0]=j, and lu[0][0..n-1] = A[j][0..n-1]
		for(j=0; j<n; j++){
			if(A[j][0])
				break;
		}
		pi[0] = j;
		memmove(lu[0], A[j], sizeof(double)*n); // }}}
		// {{{ arrange matrix to be more LU
		tlui = lu[0];
		tluii = tlui[0];
		for(j=1; j<n; j++){
			lu[j][0] = A[j][0]/tluii;
			tluj = lu[j];
			tluji = tluj[0];
			taj = A[j]+1;
			for(tluj++,k=1; k<n; k++, tluj++, taj++)
				*tluj = *taj-tluji*tlui[k];
		}
		// }}}
	// }}}
	for(i=1; i<n; i++){
		// {{{ find lu[j][i]!=0 from j=i to n-1, set pi[i]=j, and lu[i] <-> lu[j]
		tmp = 0.0;
		for(j=i; j<n; j++){
			if(lu[j][i] > tmp){
				tmp = lu[j][i];
				pii = j;
			}
		}
		pi[i] = pii;
		if(pii != i){
			tp = lu[j];
			lu[j] = lu[i];
			lu[i] = tp;
		} // }}}
		// {{{ arrange matrix to be more LU
		tlui = lu[i];
		tluii = tlui[i];
		tlui_begin = tlui+i+1;
		for(tlu=lu+i+1,tlu_end=lu+n; tlu<tlu_end; tlu++){
			tluj = *tlu+i;
			tluj_end = tluj+n-i;
			*tluj /= tluii;
			tluji = *tluj;
			for(tluj++,tlui=tlui_begin; tluj<tluj_end; tluj++,tlui++){
				*tluj -= tluji**tlui;
			}
		}
	} // }}}
// }}}
// {{{ calculate x
	y = (double *)malloc(sizeof(double)*n);
	for(i=0; i<n; i++){
		tmp = b[pi[i]];
		for(j=0; j<i; j++)
			tmp -= lu[i][j]*y[j];
		y[i] = tmp;
	}
	for(i=n-1; i>=0; i--){
		tmp = y[i];
		for(j=i+1; j<n; j++)
			tmp -= lu[i][j]*x[j];
		x[i] = tmp/lu[i][i];
	}
	// }}}
}
/*
	// {{{ display
	printf("=== matrix A ===\n");
	for(i=0; i<n; i++){
		for(j=0; j<n; j++)
			printf("%10.3lf ", A[i][j]);
		printf("\n");
		}
	printf("=== matrix lu ===\n");
	for(i=0; i<n; i++){
		for(j=0; j<n; j++)
			printf("%10.3lf ", lu[i][j]);
		printf("\n");
	}
	printf("=== matrix b ===\n");
	for(i=0; i<n; i++)
		printf("%10.3lf ", b[i]);
	printf("\n");
	printf("=== matrix pi ===\n");
	for(i=0; i<n; i++)
		printf("%d ", pi[i]);
	printf("\n");
	printf("================\n");
	checkLU(A, lu, n); // }}}
*/
