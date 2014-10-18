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
	FILE* in, * out; // входной и выходной файл
	char sTmp[SIZE_OF_STR_BUF]; // временная строка для считывания и записи
	unsigned long *mtr = 0; // матрица смежности
	int szmtr = 0; // число строк матрицы смежности

	setlocale( LC_ALL, "" );

	if( argc != 3 )
	{
		printf("Неверное число аргументов.");
		getchar(); /* ждать нажатия клавиши */
		return 1;
	}

	if ( !(in = fopen(argv[1], "r")) )
	{
		printf("Невозможно открыть входной файл. %s\n", argv[1]);
		getchar(); /* ждать нажатия клавиши */
		return 1;
	}
	
	if ( !(out = fopen(argv[2], "w")) )
	{
		printf("Невозможно открыть выходной файл. %s\n", argv[1]);
		getchar(); /* ждать нажатия клавиши */
		return 1;
	}

	// Первая строка
	fgets( sTmp, SIZE_OF_STR_BUF, in );
	fputs( sTmp, out );

	// Вторая строка
	fgets( sTmp, SIZE_OF_STR_BUF, in );
	fputs( sTmp, out );
	szmtr = atoi( sTmp );

	if ( szmtr <= 0 )
	{
		printf("Неверный размер матрицы смежности.\n");
		getchar(); /* ждать нажатия клавиши */
		return 1;
	}

	if ( szmtr > sizeof( unsigned long) * 8 )
	{
		printf("Слишком большой размер матрицы смежности.\n");
		getchar(); /* ждать нажатия клавиши */
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
			printf("Неверное количество элементов в строке в номером %d.\n", i + 1);
			getchar(); /* ждать нажатия клавиши */
			return 1;
		}

	} //for

	/* Определение связности графа */
	unsigned long U = (1 << szmtr) - 1; // представление множества всех вершин графа
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
		fputs( "\n\nГраф связный.", out );
	}
	else
	{
		fputs( "\n\nГраф несвязный.", out );
	}

	/* Нахождение компонет связности графа. */
	unsigned long SK = 0; // текущее покрытие компонентами связности
	int nk = 0; // число компонент связности
	int s; // номер непокрытой вершины

	while ( SK != U )
	{
		// Ищем непокрытую вершину
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
		// Строим компоненту от текущей вершины
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

		SK |= M2; //текущая компонета добавлена к покрытию
		++nk;

		//Вывод текущей компоненты
		fprintf( out, "\n\nКомпонента, содержащая вершину %d:\n", s );
		for (int i = 0; i < szmtr; ++i)
		{
			if ( M2 & (1 << i) )
			{
				fprintf( out, " %d ", i );
			} //for
		} //if

	}//while

	fprintf( out, "\n\nГраф имеет %d компонент(у) связности.", nk );

	fclose( in );
	fclose( out );
	free( mtr );
}//main