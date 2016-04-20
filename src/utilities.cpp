// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#include <Rcpp.h>

#include "civil_time.h"
#include "time_zone.h"

// [[Rcpp::export]]
double tzDiff(const std::string tzfrom, const std::string tzto, Rcpp::Datetime dt) {

    cctz::time_zone tza, tzb;
    if (!cctz::load_time_zone(tzfrom, &tza)) Rcpp::stop("Bad 'from' timezone");
    if (!cctz::load_time_zone(tzto, &tzb))   Rcpp::stop("Bad 'to' timezone");

    const auto tp1 = cctz::convert(cctz::civil_second(dt.getYear(),
                                                      dt.getMonth(),
                                                      dt.getDay(),
                                                      dt.getHours(),
                                                      dt.getMinutes(),
                                                      dt.getSeconds()),
                                   tza);
    
    const std::string s = cctz::format("%F %T %z", tp1, tza);
    //Rcpp::Rcout << s << "\n";

    const auto tp2 = cctz::convert(cctz::civil_second(dt.getYear(),
                                                      dt.getMonth(),
                                                      dt.getDay(),
                                                      dt.getHours(),
                                                      dt.getMinutes(),
                                                      dt.getSeconds()),
                                   tzb);
    const std::string s2 = cctz::format("%F %T %z", tp2, tzb);
    //Rcpp::Rcout << s2 << "\n";
    std::chrono::hours d = std::chrono::duration_cast<std::chrono::hours>(tp1-tp2);
    //Rcpp::Rcout << id.count() << "\n";
    
    return d.count();
}
