#pragma once
#include <string>
#include <stdio.h>
using namespace std;
class cJordanMtr
{
	int nLines; //число строк
	int nColumns; //число столбцов
	int* narLnInd; //номера переменных для строк
	int* narClmInd;//номера переменных для столбцов	
	string sNameOfVar; //имя переменной
	double** dmtrBase; //матрица для преобразований
	void fnBsMtrCpy(double*const* _dmtrBase, const int _nLines, const int _nColumns);
	void fnLnIndCpy(const int* _narLnInd, const int _nLines);
	void fnClmIndCpy(const int* _narClmInd, const int _nColumns);	
	cJordanMtr(void);
protected:
	const int get_nLines() const {return nLines;};
	const int get_nColumns() const {return nColumns;};
	const int* get_narLnInd() const {return narLnInd;};
	const int* get_narClmInd() const {return narClmInd;};
	const string get_sNameOfVar() const {return sNameOfVar;};
	double *const* get_dmtrBase() const {return dmtrBase;};
	void fnMakeJordanException(const int r, const int k);
	friend void fnMatrixIntegerBranching(cJordanMtr* out_Floor_brach, cJordanMtr* out_Ceil_branch, 
										  cJordanMtr* Source, int var, double value);
public:
	cJordanMtr(double*const* _dmtrBase, const int _nLines, const int _nColumns, 
		const int* _narLnInd, const int* _narClmInd, const std::string _sNameOfVar);
	cJordanMtr( const cJordanMtr& _in);
	void fnPrint(void) const;
	virtual ~cJordanMtr(void);
};

