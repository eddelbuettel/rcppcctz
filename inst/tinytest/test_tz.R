
## Copyright (C) 2018 - 2019  Dirk Eddelbuettel
##
## This file is part of RcppCCTZ.
##
## RcppCCTZ is free software: you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation, either version 2 of the
## License, or (at your option) any later version.
##
## RcppCCTZ is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with RcppCCTZ.  If not, see <http://www.gnu.org/licenses/>.

library(RcppCCTZ)

isSolaris <- Sys.info()[["sysname"]] == "SunOS"

timepoint <- ISOdatetime(2010,1,2,3,4,5)
if (!isSolaris) {
    nyc2lon <- toTz(timepoint, "America/New_York", "Europe/London")
    h <- as.integer(difftime(nyc2lon, timepoint, units="hour"))
    expect_equal(h, 5L)
}

moonland <- ISOdatetime(1969,7,20,22,56,0,tz="UTC")
if (!isSolaris) {
    oz <- toTz(moonland, "America/New_York", "Australia/Sydney")
    txt <- format(oz, tz="Australia/Sydney")
    expect_equal(txt, "1969-07-21 12:56:00")
}

timepoint <- ISOdatetime(2010,1,2,3,4,5)
if (!isSolaris) {
    chi2lon <- tzDiff("America/Chicago", "Europe/London", timepoint)
    expect_equal(chi2lon, 6L)

    nyc2lon <- tzDiff("America/New_York", "Europe/London", timepoint)
    expect_equal(nyc2lon, 5L)
}
