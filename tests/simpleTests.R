
library(RcppCCTZ)

ord <- "America/Chicago"
lax <- "America/Los_Angeles"
lga <- "America/New_York"
htr <- "Europe/London"
utc <- "UTC"

now <- ISOdatetime(2016, 11, 29, 12, 00, 00, tz="UTC")

format(toTz(now, utc, lax), tz=lax)
format(toTz(now, utc, ord), tz=ord)
format(toTz(now, utc, lga), tz=lga)
format(toTz(now, utc, htr), tz=htr)

format(toTz(ISOdatetime(1969,7,20,22,56,0,tz="UTC"),
            "America/New_York", "Australia/Sydney", verbose=TRUE),
       tz="Australia/Sydney")


## RcppCCTZ:::example0()	# uses localtime, hard to compare against
RcppCCTZ:::example1()
RcppCCTZ:::example2()
## RcppCCTZ:::example3()     # idem
## RcppCCTZ:::example4()     # idem
RcppCCTZ:::helloMoon()
RcppCCTZ:::exampleFormat()

tzDiff("America/New_York", "Europe/London", now)
