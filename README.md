## RcppCCTZ [![Build Status](https://travis-ci.org/eddelbuettel/rcppcctz.svg)](https://travis-ci.org/eddelbuettel/rcppcctz) [![License](https://eddelbuettel.github.io/badges/GPL2+.svg)](http://www.gnu.org/licenses/gpl-2.0.html) [![CRAN](http://www.r-pkg.org/badges/version/RcppCCTZ)](https://cran.r-project.org/package=RcppCCTZ) [![Downloads](http://cranlogs.r-pkg.org/badges/RcppCCTZ?color=brightgreen)](http://www.r-pkg.org/pkg/RcppCCTZ)

Rcpp bindings for [CCTZ](https://github.com/google/cctz)

### What is CCTZ?

[CCTZ](https://github.com/google/cctz) (C++ Time Zone) is an excellent (yet small) C++11 library for
translating between absolute times and civil times using the rules defined by a time zone. See its
[repository](https://github.com/google/cctz) (as well as code) for very detailed documentation.
[CCTZ](https://github.com/google/cctz) is being developed by Google but not an officially endorsed product.

### What is RcppCCTZ?

This package wraps CCTZ for use by R via [Rcpp](http://dirk.eddelbuettel.com/code/rcpp.html).

### Examples

#### Difference between Timezones

```r
R> # simple call: difference now
R> tzDiff("America/New_York", "Europe/London", Sys.time())
[1] 5
R> # tabulate difference for every week of the year
R> table(sapply(0:52, function(d) tzDiff("America/New_York", "Europe/London",
+                                        as.POSIXct(as.Date("2016-01-01") + d*7))))

 4  5 
 3 50 
```

#### Shifting Timezone

```r
R> # Given current time in NY what is the time London, UK
R> # (running the code locally in Chicago hence CST text format)    
R> toTz(Sys.time(), "America/New_York", "Europe/London") 	
[1] "2016-12-10 17:15:04.20370 CST"
R> # this redoes the 'Armstrong on the moon in NYC and Sydney' example
R> # note that the default print method will print the return object in _your local time_
R> toTz(ISOdatetime(1969,7,20,22,56,0,tz="UTC"), "America/New_York", "Australia/Sydney", TRUE)
1969-07-20 22:56:00 -0400
1969-07-21 12:56:00 +1000
[1] "1969-07-20 21:56:00 CDT"
R> # whereas explicitly formating for Sydney time does the right thing
R> format(toTz(ISOdatetime(1969,7,20,22,56,0,tz="UTC"), 
+             "America/New_York", "Australia/Sydney", verbose=TRUE), 
+        tz="Australia/Sydney")
1969-07-20 22:56:00 -0400
1969-07-21 12:56:00 +1000
[1] "1969-07-21 12:56:00"
```

#### Parsing and Formatting

```r
R> now <- Sys.time()
R> formatDatetime(now)            # current (UTC) time, in full precision RFC3339
[1] "2016-12-10T18:23:03.327956+00:00"
R> formatDatetime(now, tgttzstr="America/New_York")  # same but in NY
[1] "2016-12-10T13:23:03.327956-05:00"
R> formatDatetime(now + 0:4)	   # vectorised
[1] "2016-12-10T18:23:03.327956+00:00" "2016-12-10T18:23:04.327956+00:00" 
[3] "2016-12-10T18:23:05.327956+00:00" "2016-12-10T18:23:06.327956+00:00"
[5] "2016-12-10T18:23:07.327956+00:00"
R>

R> ds <- getOption("digits.secs")
R> options(digits.secs=6) # max value
R> parseDatetime("2016-12-07 10:11:12",        "%Y-%m-%d %H:%M:%S");   # full seconds
[1] "2016-12-07 04:11:12 CST"
R> parseDatetime("2016-12-07 10:11:12.123456", "%Y-%m-%d %H:%M:%E*S"); # fractional seconds
[1] "2016-12-07 04:11:12.123456 CST"
R> parseDatetime("2016-12-07T10:11:12.123456-00:00")  ## default RFC3339 format
[1] "2016-12-07 04:11:12.123456 CST"
R> now <- trunc(Sys.time())
R> parseDatetime(formatDatetime(now + 0:4))	   			# vectorised
[1] "2016-12-10 12:24:25 CST" "2016-12-10 12:24:26 CST" "2016-12-10 12:24:27 CST"
[4] "2016-12-10 12:24:28 CST" "2016-12-10 12:24:29 CST"
R> options(digits.secs=ds)
    
```
    
### Requirements

The [CCTZ](https://github.com/google/cctz) library depends on timezone files typically found in
`/usr/share/zoneinfo` which requires a Unix-alike OS such as Linux or OS X. Old school Unix variants
may work.

### Status

On [CRAN](https://cran.r-project.org/package=RcppCCTZ), builds and tests cleanly,
and the example functions are accessible from R.

### Author

Dirk Eddelbuettel

### License

GPL (>= 2)


