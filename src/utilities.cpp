// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#include <Rcpp.h>

#include "civil_time.h"
#include "time_zone.h"

//' Difference between two given timezones at a specified date.
//'
//' Time zone offsets vary by date, and this helper function computes
//' the difference (in hours) between two time zones for a given date time.
//'
//' @title Return difference between two time zones at a given date.
//' @param tzfrom The first time zone as a character vector.
//' @param tzto The second time zone as a character vector.
//' @param dt A Datetime object specifying when the difference is to be computed.
//' @param verbose A boolean toggle indicating whether more verbose operations
//' are desired, default is \code{FALSE}.
//' @return A numeric value with the difference (in hours) between the first and
//' second time zone at the given date
//' @author Dirk Eddelbuettel
//' @examples
//' # simple call: difference now
//' tzDiff("America/New_York", "Europe/London", Sys.time())
//' # tabulate difference for every week of the year
//' table(sapply(0:52, function(d) tzDiff("America/New_York", "Europe/London",
//'                                       as.POSIXct(as.Date("2016-01-01") + d*7))))
// [[Rcpp::export]]
double tzDiff(const std::string tzfrom,
              const std::string tzto,
              Rcpp::Datetime dt,
              bool verbose=false) {

    cctz::time_zone tz1, tz2;
    if (!cctz::load_time_zone(tzfrom, &tz1)) Rcpp::stop("Bad 'from' timezone");
    if (!cctz::load_time_zone(tzto, &tz2))   Rcpp::stop("Bad 'to' timezone");

    const auto tp1 = cctz::convert(cctz::civil_second(dt.getYear(),
                                                      dt.getMonth(),
                                                      dt.getDay(),
                                                      dt.getHours(),
                                                      dt.getMinutes(),
                                                      dt.getSeconds()),
                                   tz1);
    if (verbose) Rcpp::Rcout << cctz::format("%F %T %z", tp1, tz1) << std::endl;

    const auto tp2 = cctz::convert(cctz::civil_second(dt.getYear(),
                                                      dt.getMonth(),
                                                      dt.getDay(),
                                                      dt.getHours(),
                                                      dt.getMinutes(),
                                                      dt.getSeconds()),
                                   tz2);
    if (verbose) Rcpp::Rcout << cctz::format("%F %T %z", tp2, tz2) << std::endl;

    std::chrono::hours d = std::chrono::duration_cast<std::chrono::hours>(tp1-tp2);
    if (verbose) Rcpp::Rcout << "Difference: " << d.count() << std::endl;
    
    return d.count();
}
