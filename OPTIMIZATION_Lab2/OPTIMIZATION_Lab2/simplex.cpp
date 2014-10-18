#include <stdio.h>
#include <limits.h>
#include "simplex.h"

/*
�������������� ��������-�������

n - ����� ��������
m - ����� �����
sim - ��������-�������

� ������� ������������� �� ���� ������ � ���� ������� ������,
��� � �������� ��������-�������
� ������ ������ � ���������� [0,2]-[0, n-1] �������� ������ ��������� ����������
� ������ ������� � ���������� [0,1]-[0, m-2] �������� ������ �������� ����������
*/

ERROR_TYPE fnSimDo ( double** sim, int m, int n )
{
	ERROR_TYPE err;
	bool firstUse = true;
	while ( 1 )
	{
		err = fnSimMin(sim, m, n, firstUse);
		firstUse = false;
		switch (err)
		{
		case NO_ERRORS:
			break;
		case NO_ERROR_OPTIMUM:			
		case NO_DECISION:			
		case UNLIMITED_DECISION:
			return err;
		} //switch
	} //while
}

// ����� ������������ �������
ERROR_TYPE fnSimMin ( double** sim, int m, int n, bool firstUse )
{
	if (firstUse)
	{
		ERROR_TYPE err = fnSimOpornoe(sim, m, n);
		if (err == NO_DECISION)
			return err;
		//fnSimOut(sim, m, n);
	} //if
	// ���� � ������ F �� �������� ������������� ���������, �� ������� ����������
	int k = 0; // ����� �������� ����� �������������� ��������, ���� ����� ����� 
			   // (������������� �����������) �������
	for (int i = 2; i < n; ++i)
	{
		if ( sim[m-1][i] > 0 )
		{
			k = i;
			break;
		} //if
		if ( i == n - 1 )		
			return NO_ERROR_OPTIMUM; // ���� �������. ��� - ����������
	} // for
	// ��������� �������, ��������������� k
	for (int i = 1; i < m - 1 ; ++i)
	{
		if ( sim[i][k] > 0 )		
			break;	
		if ( i == m - 2 )
			return UNLIMITED_DECISION; // ���� �������. ��� - �������������� �������
	} //for
	// ����� ������������ ��������
	int r;
	double min = MY_MAX_DOUBLE, tmp;
	for (int i = 1; i < m - 1 ; i++)
	{
		if ( ( min > ( tmp = sim[i][1] / sim[i][k] ) ) && ( tmp >= 0 ) && ( sim[i][k] > 0 ) )
		{
			min = tmp;
			r = i;
		} //if
	} //for
	// ��������� ���������� ��� ���������� ��������
	fnJordan(sim, m, n, r, k);
	fnSimOut(sim, m, n);
	return NO_ERRORS; // ��������, ���� ����� ����������� �������
}

// ����� �������� �������
ERROR_TYPE fnSimOpornoe ( double** sim, int m, int n )
{
	int r, k;
	ERROR_TYPE err;
	while ( ( ( err = fnGetRazreshElem(sim, m, n, r, k) ) != NO_DECISION ) && \
		!( ( err == NO_ERRORS ) && (!r) && (!k) ) )
	{
		fnJordan(sim, m, n, r, k);	
		fnSimOut(sim, m, n);
	} //while
	return err;
}

/*
����� ������������ �������� ��� ����������� �������� �������
r k - ���������� ������������ ��������
*/
ERROR_TYPE fnGetRazreshElem ( double** sim, int m, int n, int& r, int& k )
{
	r = 0, k = 0;
	for (int i = 1; i < m-1 ; ++i) // ������ �� ������� ��������� ������
	{
		if ( sim[i][1] < 0 ) // ���� ����� �������������
		{
			for (int j = 2; j < n ; ++j) // ������ �� ������ � ������������� ��������� ������
			{
				if ( sim[i][j] < 0 ) // ���� ����� ������������� �������
				{
					k = j; // ����������� ������� ������
					// ���� ����������� ��������� ���������� ����� � ��-�� ������������ �������
					double min = MY_MAX_DOUBLE, tmp;					
					for (int p = 1; p < m - 1 ; ++p )
					{
						if ( min > ( tmp = sim[p][1] / sim[p][k] ) && ( tmp > 0 ) )
						{
							min = tmp;
							r = p;
						} //if
					} // for
					return NO_ERRORS; // ���� ����� �������. ��������� ���� � ������������� �������
									  // � ���� �� ������, �� ������ ���
				} //if
			} //for
			return NO_DECISION; // ���� ����� �������. ��������� ���� � �� ����� ������������� �������
								// � ���� �� ������, �� ������� ���
		} //if
	} //for
	return NO_ERRORS; // �� ����� ������������ ��������� ������ - ������� ������� �������
}

/*
��������� ����������

n - ����� ��������
m - ����� �����
r - ����������� ������
k - ����������� �������
sim - ��������-�������
*/

void fnJordan ( double** sim, int m, int n, int r, int k )
{
	// ������ ������� ������ ����������
	double tmp = sim[0][k]; 
	sim[0][k] = sim[r][0]; 
	sim[r][0] = tmp;
	// �������������� ��������� ��� ����������� ������ � �������
	for (int i = 1;  i < m ; ++i)
	{
		for (int j = 1;  j < n ; ++j)
		{
			if ( (i == r) || ( j == k ) ) continue;
			sim[i][j] -= sim[i][k] * sim[r][j] / sim[r][k];
		} //for
	} //for

	// �������������� ����������� ������
	for (int j = 1; j < n ; ++j)
	{
		if ( j == k ) continue;
		sim[r][j] = sim[r][j] / sim[r][k];
	} //for

	// �������������� ������������ �������
	for (int i = 1;  i < m ; ++i)
	{
		if ( i == r ) continue;
		sim[i][k] = - sim[i][k] / sim[r][k];
	} //for

	// ������ ����������� �������
	sim[r][k] = 1./ ( sim[r][k] );	
}

void fnSimOut ( double** sim, int m, int n )
{
	printf("\t b\t");
	for (int i = 2; i < n; ++i)
	{
		printf("\t Y%d \t", (int)sim[0][i]);
	} //for
	printf("\n");
	for (int i = 1; i < m; ++i)
	{
		if (i != m - 1)
		{
			printf("Y%d \t", (int)sim[i][0]);
		} else
		{
			printf("F \t");
		} // if-else		
		for (int j = 1; j < n; ++j)
		{
			printf( "%8.6f\t", sim[i][j] ) ;
		} //for
		printf("\n");
	} //for
	printf("\n\n");
}
