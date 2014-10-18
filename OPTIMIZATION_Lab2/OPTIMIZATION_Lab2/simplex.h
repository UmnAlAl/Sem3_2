#ifndef _SIMPLEX_H_
#define _SIMPLEX_H_

#define MY_MAX_DOUBLE 100000000

const enum ERROR_TYPE {NO_ERRORS, NO_ERROR_OPTIMUM, NO_DECISION, UNLIMITED_DECISION};

ERROR_TYPE fnSimDo ( double** sim, int m, int n );
ERROR_TYPE fnSimMin ( double** sim, int m, int n, bool firstUse );
ERROR_TYPE fnSimOpornoe ( double** sim, int m, int n );
ERROR_TYPE fnGetRazreshElem ( double** sim, int m, int n, int& r, int& k );
void fnJordan ( double** sim, int m, int n, int r, int k );
void fnSimOut ( double** sim, int m, int n );

#endif // !_SIMPLEX_H_
