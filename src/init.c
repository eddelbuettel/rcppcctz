#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP RcppCCTZ_example0();
extern SEXP RcppCCTZ_example1();
extern SEXP RcppCCTZ_example2();
extern SEXP RcppCCTZ_example3();
extern SEXP RcppCCTZ_example4();
extern SEXP RcppCCTZ_exampleFormat();
extern SEXP RcppCCTZ_formatDatetime(SEXP, SEXP, SEXP, SEXP);
extern SEXP RcppCCTZ_formatDouble(SEXP, SEXP, SEXP, SEXP);
extern SEXP RcppCCTZ_helloMoon(SEXP);
extern SEXP RcppCCTZ_now();
extern SEXP RcppCCTZ_parseDatetime(SEXP, SEXP, SEXP);
extern SEXP RcppCCTZ_parseDouble(SEXP, SEXP, SEXP);
extern SEXP RcppCCTZ_toTz(SEXP, SEXP, SEXP, SEXP);
extern SEXP RcppCCTZ_tzDiff(SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"RcppCCTZ_example0",       (DL_FUNC) &RcppCCTZ_example0,       0},
    {"RcppCCTZ_example1",       (DL_FUNC) &RcppCCTZ_example1,       0},
    {"RcppCCTZ_example2",       (DL_FUNC) &RcppCCTZ_example2,       0},
    {"RcppCCTZ_example3",       (DL_FUNC) &RcppCCTZ_example3,       0},
    {"RcppCCTZ_example4",       (DL_FUNC) &RcppCCTZ_example4,       0},
    {"RcppCCTZ_exampleFormat",  (DL_FUNC) &RcppCCTZ_exampleFormat,  0},
    {"RcppCCTZ_formatDatetime", (DL_FUNC) &RcppCCTZ_formatDatetime, 4},
    {"RcppCCTZ_formatDouble",   (DL_FUNC) &RcppCCTZ_formatDouble,   4},
    {"RcppCCTZ_helloMoon",      (DL_FUNC) &RcppCCTZ_helloMoon,      1},
    {"RcppCCTZ_now",            (DL_FUNC) &RcppCCTZ_now,            0},
    {"RcppCCTZ_parseDatetime",  (DL_FUNC) &RcppCCTZ_parseDatetime,  3},
    {"RcppCCTZ_parseDouble",    (DL_FUNC) &RcppCCTZ_parseDouble,    3},
    {"RcppCCTZ_toTz",           (DL_FUNC) &RcppCCTZ_toTz,           4},
    {"RcppCCTZ_tzDiff",         (DL_FUNC) &RcppCCTZ_tzDiff,         4},
    {NULL, NULL, 0}
};

void R_init_RcppCCTZ(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
