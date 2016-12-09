// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#include <Rcpp.h>

#include "civil_time.h"
#include "time_zone.h"
#include "time_zone_if.h"

namespace sc = std::chrono; 	// shorthand

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

    sc::hours d = sc::duration_cast<sc::hours>(tp1-tp2);
    if (verbose) Rcpp::Rcout << "Difference: " << d.count() << std::endl;
    
    return d.count();
}


//' Change from one given timezone to another.
//'
//' Time zone offsets vary by date, and this helper function converts
//' a Datetime object from one given timezone to another.
//'
//' @title Shift datetime object from one timezone to another
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
//' # this redoes the 'Armstrong on the moon in NYC and Sydney' example
//' # note that the default print method will print the return object in _your local time_
//' toTz(ISOdatetime(1969,7,20,22,56,0,tz="UTC"), "America/New_York", "Australia/Sydney", verbose=TRUE)
//' # whereas explicitly formating for Sydney time does the right thing
//' format(toTz(ISOdatetime(1969,7,20,22,56,0,tz="UTC"), 
//'             "America/New_York", "Australia/Sydney", verbose=TRUE), 
//'        tz="Australia/Sydney")
// [[Rcpp::export]]
Rcpp::Datetime toTz(Rcpp::Datetime dt,
                    const std::string tzfrom,
                    const std::string tzto,
                    bool verbose=false) {

    // retain existing sub-second information
    double remainder = dt.getFractionalTimestamp() - std::floor(dt.getFractionalTimestamp());
    
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

    cctz::time_point<cctz::sys_seconds> ntp = cctz::convert(ct, tz2);
    cctz::time_point<cctz::sys_seconds> unix_epoch =
        sc::time_point_cast<cctz::sys_seconds>(sc::system_clock::from_time_t(0));

    // time since epoch, with fractional seconds added back in
    double newdt = (ntp - unix_epoch).count() + remainder;
    
    return Rcpp::Datetime(newdt);
}


//' Format a Datetime object
//'
//' An alternative to \code{format.POSIXct} based on the CCTZ library
//'
//' @title Format a Datetime object as string
//' @param dt A Datetime object to be formatted
//' @param fmt A string with the format, which is based on \code{strftime} with some
//'   extensions; see the CCTZ documentation for details.
//' @param lcltzstr The local timezone object for creation the CCTZ timepoint
//' @param tgttzstr The target timezone for the desired format
//' @return A string with the requested format of the datetime object
//' @author Dirk Eddelbuettel
//' @examples
//' now <- Sys.time()
//' formatDatetime(now)            # current (UTC) time, in full precision RFC3339
//' formatDatetime(now, tgttzstr="America/New_York")  # same but in NY
// [[Rcpp::export]]
std::string formatDatetime(Rcpp::Datetime dt,
                           std::string fmt = "%Y-%m-%dT%H:%M:%E*S%Ez",
                           std::string lcltzstr = "UTC",
                           std::string tgttzstr = "UTC") {
         
    cctz::time_zone tgttz, lcltz;
    load_time_zone(tgttzstr, &tgttz);
    load_time_zone(lcltzstr, &lcltz);

    cctz::time_point<sc::microseconds> tp =
        cctz::convert(cctz::civil_second(dt.getYear(), dt.getMonth(), dt.getDay(),
                                         dt.getHours(), dt.getMinutes(), dt.getSeconds()), lcltz)
        + sc::microseconds(dt.getMicroSeconds());
    
    std::string res = cctz::format(fmt, tp, tgttz);
    return res;
}

//' Parse a Datetime object
//'
//' An alternative to \code{as.POSIXct} based on the CCTZ library
//'
//' @title Parse a Datetime object from a string
//' @param txt A string from which a datetime is to be parsed
//' @param fmt A string with the format, which is based on \code{strftime} with some
//'   extensions; see the CCTZ documentation for details.
//' @param tzstr The local timezone for the desired format
//' @return A Datetime object
//' @author Dirk Eddelbuettel
//' @examples
//' ds <- getOption("digits.secs")
//' options(digits.secs=6) # max value
//' parseDatetime("2016-12-07 10:11:12",        "%Y-%m-%d %H:%M:%S");   # full seconds
//' parseDatetime("2016-12-07 10:11:12.123456", "%Y-%m-%d %H:%M:%E*S"); # fractional seconds
//' parseDatetime("2016-12-07T10:11:12.123456-00:00")  ## default RFC3339 format
//' options(digits.secs=ds)
// [[Rcpp::export]]
Rcpp::Datetime parseDatetime(std::string txt,
                             std::string fmt = "%Y-%m-%dT%H:%M:%E*S%Ez",
                             std::string tzstr = "UTC") {
    cctz::time_zone tz;
    load_time_zone(tzstr, &tz);
    sc::system_clock::time_point tp;
    if (!cctz::parse(fmt, txt, tz, &tp)) return R_NilValue;

    // Rcpp::Rcout << cctz::format(fmt, tp, tz) << std::endl;
    
    cctz::time_point<cctz::sys_seconds> unix_epoch =
        sc::time_point_cast<cctz::sys_seconds>(sc::system_clock::from_time_t(0));

    // time since epoch, with fractional seconds added back in
    double dt = (tp - unix_epoch).count() * 1.0e-9;
    //Rcpp::Rcout << dt << std::endl;
    return Rcpp::Datetime(dt);
}

// [[Rcpp::export]]
std::string formatDouble(double nt,
                         std::string fmt = "%Y-%m-%dT%H:%M:%E*S%Ez",
                         std::string lcltzstr = "UTC",
                         std::string tgttzstr = "UTC") {
         
    cctz::time_zone tgttz, lcltz;
    load_time_zone(tgttzstr, &tgttz);
    load_time_zone(lcltzstr, &lcltz);

    int64_t d = static_cast<int64_t>(nt*1e9);
    cctz::time_point<sc::nanoseconds> tp = sc::system_clock::from_time_t(0);
    tp += sc::nanoseconds(d);
    
    std::string res = cctz::format(fmt, tp, tgttz);
    return res;
}

// [[Rcpp::export]]
double parseDouble(std::string txt,
                   std::string fmt = "%Y-%m-%dT%H:%M:%E*S%Ez",
                   std::string tzstr = "UTC") {
    cctz::time_zone tz;
    load_time_zone(tzstr, &tz);
    sc::system_clock::time_point tp;
    if (!cctz::parse(fmt, txt, tz, &tp)) return 0;

    // Rcpp::Rcout << cctz::format(fmt, tp, tz) << std::endl;

    cctz::time_point<cctz::sys_seconds> unix_epoch =
        sc::time_point_cast<cctz::sys_seconds>(sc::system_clock::from_time_t(0));

    // time since epoch, with fractional seconds added back in
    double dt = (tp - unix_epoch).count() * 1.0e-9;
    return dt;
}
