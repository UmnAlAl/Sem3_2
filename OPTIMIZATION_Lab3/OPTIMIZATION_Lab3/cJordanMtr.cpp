#include "cJordanMtr.h"

cJordanMtr::cJordanMtr(double *const* _dmtrBase, const int _nLines, const int _nColumns, 
		 const int* _narLnInd, const int* _narClmInd, const std::string _sNameOfVar)
{
	nLines = _nLines;
	nColumns = _nColumns;
	sNameOfVar = _sNameOfVar;
	fnBsMtrCpy(_dmtrBase, _nLines, _nColumns);
	fnClmIndCpy(_narClmInd, _nColumns);
	fnLnIndCpy(_narLnInd, _nLines);
}

cJordanMtr::cJordanMtr( const cJordanMtr& _in)
{
	nLines = _in.nLines;
	nColumns = _in.nColumns;
	sNameOfVar = _in.sNameOfVar;
	fnBsMtrCpy(_in.dmtrBase, _in.nLines, _in.nColumns);
	fnClmIndCpy(_in.narClmInd, _in.nColumns);
	fnLnIndCpy(_in.narLnInd, _in.nLines);
}


cJordanMtr::~cJordanMtr(void)
{
	delete narClmInd;
	delete narLnInd;
	for (int i = 0; i < nLines; i++)
	{
		delete dmtrBase[i];
	}
	delete dmtrBase;
}

void cJordanMtr::fnBsMtrCpy(double*const* _dmtrBase, const int _nLines, const int _nColumns)
{
	dmtrBase = new double*[_nLines];
	for (int i = 0; i < _nLines; ++i)
	{
		dmtrBase[i] = new double[_nColumns];
	}
	for (int i = 0; i < _nLines; i++)
	{
		for (int j = 0; j < _nColumns; j++)
		{
			dmtrBase[i][j] = _dmtrBase[i][j];
		}
	}
}

void cJordanMtr::fnClmIndCpy(const int* _narClmInd, const int _nColumns)
{
	narClmInd = new int[_nColumns - 1];
	for (int i = 0; i < _nColumns - 1; i++)
	{
		narClmInd[i] = _narClmInd[i];
	}
}


void cJordanMtr::fnLnIndCpy(const int* _narLnInd, const int _nLines)
{
	narLnInd = new int[_nLines - 1];
	for (int i = 0; i < _nLines - 1; i++)
	{
		narLnInd[i] = _narLnInd[i];
	}
}

void cJordanMtr::fnMakeJordanException(const int r, const int k)
{
	//меняем местами номера переменных
	int tmp = narClmInd[k-1];
	narClmInd[k-1] = narLnInd[r];
	narLnInd[r] = tmp;

	// преобразование элементов вне разрешающей строки и столбца
	for (int i = 0;  i < nColumns ; ++i)
	{
		for (int j = 0;  j < nLines ; ++j)
		{
			if ( (i == r) || ( j == k ) ) continue;
			dmtrBase[i][j] -= dmtrBase[i][k] * dmtrBase[r][j] / dmtrBase[r][k];
		} //for
	} //for

	// преобразование разрешающей строки
	for (int j = 0; j < nColumns ; ++j)
	{
		if ( j == k ) continue;
		dmtrBase[r][j] = dmtrBase[r][j] / dmtrBase[r][k];
	} //for

	// преобразование разрешающего столбца
	for (int i = 0;  i < nLines ; ++i)
	{
		if ( i == r ) continue;
		dmtrBase[i][k] = - dmtrBase[i][k] / dmtrBase[r][k];
	} //for

	// меняем разрешающий элемент
	dmtrBase[r][k] = 1./ ( dmtrBase[r][k] );
}

void fnMatrixIntegerBranching(cJordanMtr* out_Floor_brach, //задача с округлением заданной переменной вниз
							  cJordanMtr* out_Ceil_branch, //задача с округлением заданной переменной вверх
							  cJordanMtr* Source, //исходная задача
							  int var, //номер переменной
							  double value) //значение переменной
{
	double** mtr = new double*[Source->nLines + 1];
	int* narLnInd = new int[Source->nLines];
	
	//меняется количество строк, изменим их нумерацию
	for (int i = 0; i < Source->nLines - 1; i++)
	{
		narLnInd[i] = Source->narLnInd[i];
	}
	narLnInd[Source->nLines - 1] = Source->narLnInd[Source->nLines - 2] - 1;

	for (int i = 0; i < Source->nLines + 1; i++)
	{
		mtr[i] = new double[Source->nColumns];
	}
	//копируем исходную матрицу в новую(расширившуюся), кроме последней строки исходной матрицы
	for (int i = 0; i < Source->nLines - 1; i++)
	{
		for (int j = 0; j < Source->nColumns; j++)
		{
			mtr[i][j] = (Source->dmtrBase)[i][j];
		}
	}
	//заполняем новую строчку
	mtr[Source->nLines - 1][0] = ( var < 0 ) ? ( (int)var - 1 ) : ( (int) var );

}

void cJordanMtr::fnPrint(void) const
{
	printf("\n\n\tb\t");
	for (int i = 0; i < nColumns - 1; i++)
	{
		printf("\t%s%d\t", sNameOfVar.c_str(), narClmInd[i]);	
	}
	
	for (int i = 0; i < nLines; i++)
	{
		printf("\n");
		if (i != nLines - 1)
		{
			printf("%s%d", sNameOfVar.c_str(), narLnInd[i]);
		}
		else
		{
			printf("F");
		}
		for (int j = 0; j < nColumns; j++)
		{
			printf("\t%.6f", dmtrBase[i][j]);
		}
	}
}