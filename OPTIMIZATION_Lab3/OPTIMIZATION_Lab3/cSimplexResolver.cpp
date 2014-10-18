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
	// если в строке F не осталось положительных элементов, то решение оптимально	
	for (int i = 1; i < n; ++i)
	{
		if ( sim[m-1][i] > 0 )
		{
			k = i;//номер положительного элемента, если такой будет 			      
			break;
		} //if
		if ( i == n - 1 )
		{
			LastError = NO_ERROR_OPTIMUM;
			return; // если положит. нет - оптимально
		}
	} // for
	// Проверяем столбец, соответствующий k
	for (int i = 0; i < m - 1 ; ++i)
	{
		if ( sim[i][k] > 0 )		
			break;	
		if ( i == m - 2 )
		{
			LastError = UNLIMITED_DECISION;
			return; // если положит. нет - неограниченное решение
		}
	} //for
	// Поиск разрешающего элемента	
	double min = MY_MAX_DOUBLE, tmp;
	for (int i = 0; i < m - 1 ; i++)
	{
		if ( ( min > ( tmp = sim[i][0] / sim[i][k] ) ) && ( tmp >= 0 ) && ( sim[i][k] > 0 ) )
		{
			min = tmp;
			r = i;
		} //if
	} //for
	// Жорданово исключение для найденного элемента
	fnMakeJordanException(r,k);
	fnPrint();
	LastError = NO_ERRORS;
	return; // Возможно, есть более оптимальное решение
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
	r = -1, k = -1; //несуществующий разрешающий элемент
	for (int i = 0; i < m-1 ; ++i) // проход по столбцу свободных членов
	{
		if ( sim[i][0] < 0 ) // если нашли отрицательный
		{
			for (int j = 1; j < n ; ++j) // проход по строке с отрицательным свободным членом
			{
				if ( sim[i][j] < 0 ) // если нашли отрицательный элемент
				{
					k = j; // разрешающий столбец найден
					// ищем минимальное отношение свободного члена к эл-ту разрешающего столбца
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
					return; // если нашли отрицат. свободный член и отрицательный элемент
									  // в этой же строке, то ошибки нет
				} //if
			} //for
			LastError = NO_DECISION;
			return; // если нашли отрицат. свободный член и не нашли отрицательный элемент
					// в этой же строке, то решения нет
		} //if
	} //for
	// не нашли орицательных свободных членов - опорное решение найдено
}