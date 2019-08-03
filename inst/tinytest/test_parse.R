
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

ds <- getOption("digits.secs")
options(digits.secs=6) # max value

timepoint <- ISOdatetime(2010,1,2,3,4,5, tz="UTC")
pt <- parseDatetime("2010-01-02 03:04:05", "%Y-%m-%d %H:%M:%S", "UTC")
expect_equal(timepoint, pt)

pt <- parseDouble("2010-01-02 03:04:05", "%Y-%m-%d %H:%M:%S")
expect_equal(as.numeric(timepoint), pt[1,1])


timepoint <- ISOdatetime(2010,1,2,3,4,5.123456, tz="UTC")
pt <- parseDatetime("2010-01-02 03:04:05.123456", "%Y-%m-%d %H:%M:%E*S", "UTC")
expect_equal(timepoint, pt)

pt <- parseDouble("2010-01-02 03:04:05.123456", "%Y-%m-%d %H:%M:%E*S")
expect_equal(as.numeric(timepoint), pt[1,1])


timepoints <- ISOdatetime(2010,1,2,3,4,5, tz="UTC") + 0:9
tpvec <- format(timepoints)
ptvec <- parseDatetime(tpvec, "%Y-%m-%d %H:%M:%S", "UTC")
expect_equal(timepoints, ptvec)

options(digits.secs=ds)
