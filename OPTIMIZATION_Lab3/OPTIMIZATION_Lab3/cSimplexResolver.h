#pragma once
#include "cJordanMtr.h"
#include "SimTask.h"

class cSimplexResolver :
	public cJordanMtr
{
	int r; //строка с разрешающим элементом
	int k; //столбец с разрешающим элементом
	ERROR_TYPE LastError;
	bool OPORNOE_RESH_FOUND;
	void fnGetRazreshElem (void);
	void fnOpornoe(void);
	void fnNextMinimizationStep(void);
	cSimplexResolver(const cSimplexResolver&);
	cSimplexResolver(void);
public:
	cSimplexResolver(double *const* _dmtrBase, const int _nLines, const int _nColumns, 
		const int* _narLnInd, const int* _narClmInd, const string _sNameOfVar):
	cJordanMtr(_dmtrBase, _nLines, _nColumns, _narLnInd, _narClmInd, _sNameOfVar){};
	cSimplexResolver(const cJordanMtr& _in):cJordanMtr(_in){};
	ERROR_TYPE fnResolveTypicalSimplexTask(void);
	ERROR_TYPE fnResolveIntegerSimplexTask(void);
	virtual ~cSimplexResolver(void) {};
};

