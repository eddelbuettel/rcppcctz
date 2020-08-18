
#ifndef _RcppCCTZ_API_H
#define _RcppCCTZ_API_H

/* number of R header files (possibly listing too many) */
#include <R.h>

#include "cctz/civil_time.h"
#include "cctz/time_zone.h"

#ifdef HAVE_VISIBILITY_ATTRIBUTE
  # define attribute_hidden __attribute__ ((visibility ("hidden")))
#else
  # define attribute_hidden
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* provided the interface for the function exported in
   ../src/init.c via R_RegisterCCallable()		*/

inline int attribute_hidden RcppCCTZ_getOffset_nothrow(std::int_fast64_t s, const char* tzstr, int& offset) {
  typedef int GET_OFFSET_FUN(long long, const char*, int&);
  static GET_OFFSET_FUN *getOffset = (GET_OFFSET_FUN *) R_GetCCallable("RcppCCTZ", "_RcppCCTZ_getOffset_nothrow");
  return getOffset(s, tzstr, offset);
}

inline int attribute_hidden RcppCCTZ_convertToCivilSecond_nothrow(const cctz::time_point<cctz::seconds>& tp, const char* tzstr, cctz::civil_second& cs) {
  typedef int CONVERT_TO_CIVILSECOND(const cctz::time_point<cctz::seconds>&, const char*, cctz::civil_second&);
  static CONVERT_TO_CIVILSECOND *convertToCivilSecond = (CONVERT_TO_CIVILSECOND*) R_GetCCallable("RcppCCTZ", "_RcppCCTZ_convertToCivilSecond_nothrow");
  return convertToCivilSecond(tp, tzstr, cs);
}

inline int attribute_hidden RcppCCTZ_convertToTimePoint_nothrow(const cctz::civil_second& cs, const char* tzstr, cctz::time_point<cctz::seconds>& tp) {
  typedef int CONVERT_TO_TIMEPOINT(const cctz::civil_second&, const char*, cctz::time_point<cctz::seconds>&);
  static CONVERT_TO_TIMEPOINT *convertToTimePoint = (CONVERT_TO_TIMEPOINT*) R_GetCCallable("RcppCCTZ", "_RcppCCTZ_convertToTimePoint_nothrow");
  return convertToTimePoint(cs, tzstr, tp);
}

#ifdef __cplusplus
}

// add a namespace for C++ use
namespace RcppCCTZ {
  inline int getOffset(std::int_fast64_t s, const char* tzstr, int& offset) {
    return RcppCCTZ_getOffset_nothrow(s, tzstr, offset);
  }

  inline int convertToCivilSecond(const cctz::time_point<cctz::seconds>& tp, const char* tzstr, cctz::civil_second& cs) {
    return RcppCCTZ_convertToCivilSecond_nothrow(tp, tzstr, cs);
  }

  inline int convertToTimePoint(const cctz::civil_second& cs, const char* tzstr, cctz::time_point<cctz::seconds>& tp) {
    return RcppCCTZ_convertToTimePoint_nothrow(cs, tzstr, tp);
  }
}

#endif

#endif /* !_RcppCCTZ_API_H */
