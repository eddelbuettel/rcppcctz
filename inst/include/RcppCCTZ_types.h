
#include "cctz/time_zone.h"

// export these functions so they can be called at the C level by other packages:
template <typename D>
using time_point = std::chrono::time_point<std::chrono::system_clock, D>;
using seconds    = std::chrono::duration<std::int_fast64_t>;

// declarations of exported functions
int                             _RcppCCTZ_getOffset(std::int_fast64_t s, const char* tzstr);
cctz::civil_second              _RcppCCTZ_convertToCivilSecond(const cctz::time_point<cctz::seconds>& tp, const char* tzstr);
cctz::time_point<cctz::seconds> _RcppCCTZ_convertToTimePoint(const cctz::civil_second& cs, const char* tzstr);

int _RcppCCTZ_getOffset_nothrow(std::int_fast64_t s, const char* tzstr, int& offset);
int _RcppCCTZ_convertToCivilSecond_nothrow(const cctz::time_point<cctz::seconds>& tp, const char* tzstr, cctz::civil_second& cs);
int _RcppCCTZ_convertToTimePoint_nothrow(const cctz::civil_second& cs, const char* tzstr, cctz::time_point<cctz::seconds>& tp);
