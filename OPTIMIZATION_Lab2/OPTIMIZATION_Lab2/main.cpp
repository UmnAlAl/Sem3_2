#include "simplex.h"
#include <stdio.h>

void main (void)
{
	
	int m = 5, n = 5;
	double** sim = new double*[m];
	for (int i = 0; i < m; ++i)
	{
		sim[i] = new double[n];
	}

	/*
	0,  0,   1,  2,  3,
	4, -1,  -4, -1,  0,
	5, -5,  -1, -4, -0.5,
	6, -5,  -1,  0, -4,
	0,  0,  -5, -7, -6 
	*/
	sim[0][0] = 0; sim[0][1] = 0; sim[0][2] = 1; sim[0][3] = 2; sim[0][4] = 3;
	sim[1][0] = 4; sim[1][1] = -1; sim[1][2] = -4; sim[1][3] = -1; sim[1][4] = 0;
	sim[2][0] = 5; sim[2][1] = -5; sim[2][2] = -1; sim[2][3] = -4; sim[2][4] = -0.5;
	sim[3][0] = 6; sim[3][1] = -5; sim[3][2] = -1; sim[3][3] = 0; sim[3][4] = -4;
	sim[4][0] = 0; sim[4][1] = 0; sim[4][2] = -5; sim[4][3] = -7; sim[4][4] = -6;		
	
	fnSimDo(sim, m, n);
	for (int i = 0; i < m; ++i)
	{
		delete sim[i];
	}
	delete sim;
	getchar();
}