// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#include <Rcpp.h>

#include "civil_time.h"
#include "time_zone.h"
#include "time_zone_if.h"


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


// double toDouble(cctz::time_point<cctz::sys_seconds> tp) {
//     cctz::time_point<cctz::sys_seconds> unix_epoch =
//         std::chrono::time_point_cast<cctz::sys_seconds>(std::chrono::system_clock::from_time_t(0));
//     return(tp - unix_epoch).count();
// }
                                                                                 

//' Change from one given timezone to another.
//'
//' Time zone offsets vary by date, and this helper function converts
//' a Datetime object from one given timezone to another.
//'
//' @title Return difference between two time zones at a given date.
//' @param dt A Datetime object specifying when the difference is to be computed.
//' @param tzfrom The first time zone as a character vector.
//' @param tzto The second time zone as a character vector.
//' @param verbose A boolean toggle indicating whether more verbose operations
//' are desired, default is \code{FALSE}.
//' @return A Datetime object with the given (civil time) determined by the
//' incoming object (and its timezone) shifted to the target timezone.
//' @author Dirk Eddelbuettel
//' @examples
//' toTz(Sys.time(), "America/New_York", "Europe/London")
// [[Rcpp::export]]
Rcpp::Datetime toTz(Rcpp::Datetime dt,
                    const std::string tzfrom,
                    const std::string tzto,
                    bool verbose=false) {

    cctz::time_zone tz1, tz2;   // two time zone objects
    if (!cctz::load_time_zone(tzfrom, &tz1)) Rcpp::stop("Bad 'from' timezone");
    if (!cctz::load_time_zone(tzto, &tz2))   Rcpp::stop("Bad 'to' timezone");

    // incoming time-point object given civil-time and timezone
    const auto tp = cctz::convert(cctz::civil_second(dt.getYear(),
                                                     dt.getMonth(),
                                                     dt.getDay(),
                                                     dt.getHours(),
                                                     dt.getMinutes(),
                                                     dt.getSeconds()),
                                  tz1);
    if (verbose) Rcpp::Rcout << cctz::format("%F %T %z", tp, tz1) << std::endl;
    if (verbose) Rcpp::Rcout << cctz::format("%F %T %z", tp, tz2) << std::endl;

    // create a civil-time object from time-point and new timezone
    const auto ct = cctz::convert(tp, tz2);

    // retain existing sub-second information
    double remainder = dt.getFractionalTimestamp() - std::floor(dt.getFractionalTimestamp());

    namespace sc = std::chrono; 	// shorthand
    cctz::time_point<cctz::sys_seconds> ntp = cctz::convert(ct, tz2);
    cctz::time_point<cctz::sys_seconds> unix_epoch =
        sc::time_point_cast<cctz::sys_seconds>(sc::system_clock::from_time_t(0));

    // time since epoch, with fractional seconds added back in
    double newdt = (ntp - unix_epoch).count() + remainder;
    
    return Rcpp::Datetime(newdt);
}


