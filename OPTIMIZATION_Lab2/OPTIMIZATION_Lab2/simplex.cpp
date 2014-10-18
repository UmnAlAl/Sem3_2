#include <stdio.h>
#include <limits.h>
#include "simplex.h"

/*
Преобразование симплекс-таблицы

n - число столбцов
m - число строк
sim - симплекс-таблица

В матрице представления на одну строку и один столбец больше,
чем в исходной симплекс-таблице
В первой строке в промежутке [0,2]-[0, n-1] хранятся номера свободных переменных
В первом столбце в промежутке [0,1]-[0, m-2] хранятся номера базисных переменных
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

// Поиск оптимального решения
ERROR_TYPE fnSimMin ( double** sim, int m, int n, bool firstUse )
{
	if (firstUse)
	{
		ERROR_TYPE err = fnSimOpornoe(sim, m, n);
		if (err == NO_DECISION)
			return err;
		//fnSimOut(sim, m, n);
	} //if
	// если в строке F не осталось положительных элементов, то решение оптимально
	int k = 0; // здесь сохраним номер положительного элемента, если такой будет 
			   // (потенциальный разрешающий) столбец
	for (int i = 2; i < n; ++i)
	{
		if ( sim[m-1][i] > 0 )
		{
			k = i;
			break;
		} //if
		if ( i == n - 1 )		
			return NO_ERROR_OPTIMUM; // если положит. нет - оптимально
	} // for
	// Проверяем столбец, соответствующий k
	for (int i = 1; i < m - 1 ; ++i)
	{
		if ( sim[i][k] > 0 )		
			break;	
		if ( i == m - 2 )
			return UNLIMITED_DECISION; // если положит. нет - неограниченное решение
	} //for
	// Поиск разрешающего элемента
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
	// Жорданово исключение для найденного элемента
	fnJordan(sim, m, n, r, k);
	fnSimOut(sim, m, n);
	return NO_ERRORS; // Возможно, есть более оптимальное решение
}

// Поиск опорного решения
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
Поиск разрешающего элемента при нахожждении опорного решения
r k - координаты разрешающего элемента
*/
ERROR_TYPE fnGetRazreshElem ( double** sim, int m, int n, int& r, int& k )
{
	r = 0, k = 0;
	for (int i = 1; i < m-1 ; ++i) // проход по столбцу свободных членов
	{
		if ( sim[i][1] < 0 ) // если нашли отрицательный
		{
			for (int j = 2; j < n ; ++j) // проход по строке с отрицательным свободным членом
			{
				if ( sim[i][j] < 0 ) // если нашли отрицательный элемент
				{
					k = j; // разрешающий столбец найден
					// ищем минимальное отношение свободного члена к эл-ту разрешающего столбца
					double min = MY_MAX_DOUBLE, tmp;					
					for (int p = 1; p < m - 1 ; ++p )
					{
						if ( min > ( tmp = sim[p][1] / sim[p][k] ) && ( tmp > 0 ) )
						{
							min = tmp;
							r = p;
						} //if
					} // for
					return NO_ERRORS; // если нашли отрицат. свободный член и отрицательный элемент
									  // в этой же строке, то ошибки нет
				} //if
			} //for
			return NO_DECISION; // если нашли отрицат. свободный член и не нашли отрицательный элемент
								// в этой же строке, то решения нет
		} //if
	} //for
	return NO_ERRORS; // не нашли орицательных свободных членов - опорное решение найдено
}

/*
Жордановы исключения

n - число столбцов
m - число строк
r - разрешающая строка
k - разрешающий столбец
sim - симплекс-таблица
*/

void fnJordan ( double** sim, int m, int n, int r, int k )
{
	// меняем местами номера переменных
	double tmp = sim[0][k]; 
	sim[0][k] = sim[r][0]; 
	sim[r][0] = tmp;
	// преобразование элементов вне разрешающей строки и столбца
	for (int i = 1;  i < m ; ++i)
	{
		for (int j = 1;  j < n ; ++j)
		{
			if ( (i == r) || ( j == k ) ) continue;
			sim[i][j] -= sim[i][k] * sim[r][j] / sim[r][k];
		} //for
	} //for

	// преобразование разрешающей строки
	for (int j = 1; j < n ; ++j)
	{
		if ( j == k ) continue;
		sim[r][j] = sim[r][j] / sim[r][k];
	} //for

	// преобразование разрешающего столбца
	for (int i = 1;  i < m ; ++i)
	{
		if ( i == r ) continue;
		sim[i][k] = - sim[i][k] / sim[r][k];
	} //for

	// меняем разрешающий элемент
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
