#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#define SIZE_OF_STR_BUF 80

void fnOutL (unsigned long x )
{
	int n = sizeof( unsigned long) * 8;
	for (int i = n - 1; i >= 0; --i)
	{
		printf("%d", (x >> i) & 1 );
	}
}

void fnMtrBinOut(unsigned long* mtr, int size )
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d", ( mtr[i] >> j ) & 1 );
		}
		printf("\n");
	}
}

int main( int argc, char** argv )
{
	FILE* in, * out; // ������� � �������� ����
	char sTmp[SIZE_OF_STR_BUF]; // ��������� ������ ��� ���������� � ������
	unsigned long *mtr = 0; // ������� ���������
	int szmtr = 0; // ����� ����� ������� ���������

	setlocale( LC_ALL, "" );

	if( argc != 3 )
	{
		printf("�������� ����� ����������.");
		getchar(); /* ����� ������� ������� */
		return 1;
	}

	if ( !(in = fopen(argv[1], "r")) )
	{
		printf("���������� ������� ������� ����. %s\n", argv[1]);
		getchar(); /* ����� ������� ������� */
		return 1;
	}
	
	if ( !(out = fopen(argv[2], "w")) )
	{
		printf("���������� ������� �������� ����. %s\n", argv[1]);
		getchar(); /* ����� ������� ������� */
		return 1;
	}

	// ������ ������
	fgets( sTmp, SIZE_OF_STR_BUF, in );
	fputs( sTmp, out );

	// ������ ������
	fgets( sTmp, SIZE_OF_STR_BUF, in );
	fputs( sTmp, out );
	szmtr = atoi( sTmp );

	if ( szmtr <= 0 )
	{
		printf("�������� ������ ������� ���������.\n");
		getchar(); /* ����� ������� ������� */
		return 1;
	}

	if ( szmtr > sizeof( unsigned long) * 8 )
	{
		printf("������� ������� ������ ������� ���������.\n");
		getchar(); /* ����� ������� ������� */
		return 1;
	}

	mtr = ( unsigned long* ) malloc( sizeof(unsigned long) * szmtr );

	for (int i = 0, j = 0; i < szmtr; ++i)
	{
		fgets( sTmp, SIZE_OF_STR_BUF, in );
		fputs( sTmp, out );	
		int nRead = 0;
		mtr[i] = 0;
		for (j = 0; j < szmtr; ++j)
		{
			if ( sTmp[j] == '0' )
			{
				++nRead;
			} 
			else if (sTmp[j] == '1')
			{
				mtr[i] |= (1 << j);
				++nRead;
			}
		} //for			
		if (nRead != szmtr)
		{
			free( mtr );
			printf("�������� ���������� ��������� � ������ � ������� %d.\n", i + 1);
			getchar(); /* ����� ������� ������� */
			return 1;
		}

	} //for

	/* ����������� ��������� ����� */
	unsigned long U = (1 << szmtr) - 1; // ������������� ��������� ���� ������ �����
	unsigned long M1 = 0, M2 = 1;	
	while ( ( M1 != M2 ) && (M2 != U) )
	{		
		/*printf("\n");
		fnOutL(M1);
		printf("\n");
		fnOutL(M2);
		printf("\n");*/
		M1 = M2;
		for (int i = 0; i < szmtr; ++i)
		{
			if ( M1 & (1 << i) )
			{
				M2 |= mtr[i];
			}
		}//for
	}//while

	if (U == M2)
	{
		fputs( "\n\n���� �������.", out );
	}
	else
	{
		fputs( "\n\n���� ���������.", out );
	}

	/* ���������� �������� ��������� �����. */
	unsigned long SK = 0; // ������� �������� ������������ ���������
	int nk = 0; // ����� ��������� ���������
	int s; // ����� ���������� �������

	while ( SK != U )
	{
		// ���� ���������� �������
		for (int i = 0; i < szmtr; ++i)
		{
			if (!(SK & (1 << i)))
			{
				s = i;
				break;
			}//if
		}//for

		M1 = 0;
		M2 = (1 << s);
		// ������ ���������� �� ������� �������
		while (M1 != M2)
		{
			M1 = M2;
			for (int i = 0; i < szmtr; ++i)
			{
				if (M1 & (1 << i))
				{
					M2 |= mtr[i];
				}//if
			} //for			
		}//while

		SK |= M2; //������� ��������� ��������� � ��������
		++nk;

		//����� ������� ����������
		fprintf( out, "\n\n����������, ���������� ������� %d:\n", s );
		for (int i = 0; i < szmtr; ++i)
		{
			if ( M2 & (1 << i) )
			{
				fprintf( out, " %d ", i );
			} //for
		} //if

	}//while

	fprintf( out, "\n\n���� ����� %d ���������(�) ���������.", nk );

	fclose( in );
	fclose( out );
	free( mtr );
}//main