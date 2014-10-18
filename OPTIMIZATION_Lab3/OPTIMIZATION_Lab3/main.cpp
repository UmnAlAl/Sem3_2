#include "cSimplexResolver.h"

typedef const double * PTR;

void main(void)
{
	const int m = 3, n = 3;	
	double** mtr = new double*[m];
	for (int i = 0; i < m; ++i)
	{
		mtr[i] = new double[n];
	}
	int ClmInd[n-1] = { 1, 2};
	int LnInd[m-1] = {3, 4};
		
	mtr[0][0] = 12; mtr[0][1] = 6; mtr[0][2] = -1; //mtr[0][3] = 1;
	mtr[1][0] = 20; mtr[1][1] = 2; mtr[1][2] = 5; //mtr[1][3] = 0;
	mtr[2][0] = 0; mtr[2][1] = -12; mtr[2][2] = 1; //mtr[2][3] = 4;
	//mtr[3][0] = 0; //mtr[3][1] = 1; //mtr[3][2] = 5; //mtr[3][3] = 5;

	/*
	0,  0,   1,  2,  3,
	4, -1,  -4, -1,  0,
	5, -5,  -1, -4, -0.5,
	6, -5,  -1,  0, -4,
	0,  0,  -5, -7, -6 
	*/
	
	cSimplexResolver Task(mtr, m, n, LnInd, ClmInd, "X");
	ERROR_TYPE err =  Task.fnResolveTypicalSimplexTask();	
	getchar();
}