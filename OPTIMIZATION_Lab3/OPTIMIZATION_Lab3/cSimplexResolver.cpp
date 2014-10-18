#include "cSimplexResolver.h"

ERROR_TYPE cSimplexResolver::fnResolveIntegerSimplexTask(void)
{
}


ERROR_TYPE cSimplexResolver::fnResolveTypicalSimplexTask(void)
{
	OPORNOE_RESH_FOUND = false;
	LastError = NO_ERRORS;
	fnPrint();
	while ( 1 )
	{
		fnNextMinimizationStep();		
		switch (LastError)
		{
		case NO_ERRORS:
			break;
		case NO_ERROR_OPTIMUM:			
		case NO_DECISION:			
		case UNLIMITED_DECISION:
			return LastError;
		} //switch
	} //while
}

void cSimplexResolver::fnNextMinimizationStep(void)
{
	if (!OPORNOE_RESH_FOUND)
	{
		fnOpornoe();		
		//fnPrint();
		return;
	} //if
	double*const* sim = get_dmtrBase();
	int m = get_nLines();
	int n = get_nColumns();
	// ���� � ������ F �� �������� ������������� ���������, �� ������� ����������	
	for (int i = 1; i < n; ++i)
	{
		if ( sim[m-1][i] > 0 )
		{
			k = i;//����� �������������� ��������, ���� ����� ����� 			      
			break;
		} //if
		if ( i == n - 1 )
		{
			LastError = NO_ERROR_OPTIMUM;
			return; // ���� �������. ��� - ����������
		}
	} // for
	// ��������� �������, ��������������� k
	for (int i = 0; i < m - 1 ; ++i)
	{
		if ( sim[i][k] > 0 )		
			break;	
		if ( i == m - 2 )
		{
			LastError = UNLIMITED_DECISION;
			return; // ���� �������. ��� - �������������� �������
		}
	} //for
	// ����� ������������ ��������	
	double min = MY_MAX_DOUBLE, tmp;
	for (int i = 0; i < m - 1 ; i++)
	{
		if ( ( min > ( tmp = sim[i][0] / sim[i][k] ) ) && ( tmp >= 0 ) && ( sim[i][k] > 0 ) )
		{
			min = tmp;
			r = i;
		} //if
	} //for
	// ��������� ���������� ��� ���������� ��������
	fnMakeJordanException(r,k);
	fnPrint();
	LastError = NO_ERRORS;
	return; // ��������, ���� ����� ����������� �������
}

void cSimplexResolver::fnOpornoe(void)
{
	fnGetRazreshElem();
	while ( ( LastError != NO_DECISION ) && \
		!( ( LastError == NO_ERRORS ) && (r == -1) && (k == -1) ) )
	{		
		fnMakeJordanException(r, k);
		fnPrint();
		fnGetRazreshElem();
	} //while
	if (LastError != NO_DECISION)
		OPORNOE_RESH_FOUND = true;
}

void cSimplexResolver::fnGetRazreshElem()
{
	int m = get_nLines();
	int n = get_nColumns();
	double*const* sim = get_dmtrBase();
	r = -1, k = -1; //�������������� ����������� �������
	for (int i = 0; i < m-1 ; ++i) // ������ �� ������� ��������� ������
	{
		if ( sim[i][0] < 0 ) // ���� ����� �������������
		{
			for (int j = 1; j < n ; ++j) // ������ �� ������ � ������������� ��������� ������
			{
				if ( sim[i][j] < 0 ) // ���� ����� ������������� �������
				{
					k = j; // ����������� ������� ������
					// ���� ����������� ��������� ���������� ����� � ��-�� ������������ �������
					double min = MY_MAX_DOUBLE, tmp;					
					for (int p = 0; p < m - 1 ; ++p )
					{
						if ( min > ( tmp = sim[p][0] / sim[p][k] ) && ( tmp > 0 ) )
						{
							min = tmp;
							r = p;
						} //if
					} // for
					LastError = NO_ERRORS;
					return; // ���� ����� �������. ��������� ���� � ������������� �������
									  // � ���� �� ������, �� ������ ���
				} //if
			} //for
			LastError = NO_DECISION;
			return; // ���� ����� �������. ��������� ���� � �� ����� ������������� �������
					// � ���� �� ������, �� ������� ���
		} //if
	} //for
	// �� ����� ������������ ��������� ������ - ������� ������� �������
}