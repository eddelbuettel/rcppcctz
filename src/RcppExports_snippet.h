/* The following needs to be preserved in the call to R_init_RcppCCTZ:
    R_RegisterCCallable("RcppCCTZ", "_RcppCCTZ_getOffset", (DL_FUNC) &_RcppCCTZ_getOffset);
    R_RegisterCCallable("RcppCCTZ", "_RcppCCTZ_convertToCivilSecond", (DL_FUNC) &_RcppCCTZ_convertToCivilSecond);
    R_RegisterCCallable("RcppCCTZ", "_RcppCCTZ_convertToTimePoint", (DL_FUNC) &_RcppCCTZ_convertToTimePoint);
    R_RegisterCCallable("RcppCCTZ", "_RcppCCTZ_getOffset_nothrow", (DL_FUNC) &_RcppCCTZ_getOffset_nothrow);
    R_RegisterCCallable("RcppCCTZ", "_RcppCCTZ_convertToCivilSecond_nothrow", (DL_FUNC) &_RcppCCTZ_convertToCivilSecond_nothrow);
    R_RegisterCCallable("RcppCCTZ", "_RcppCCTZ_convertToTimePoint_nothrow", (DL_FUNC) &_RcppCCTZ_convertToTimePoint_nothrow);
*/
