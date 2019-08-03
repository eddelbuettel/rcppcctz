
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

timepoint <- ISOdatetime(2010,1,2,3,4,5, tz="UTC")
txt <- formatDatetime(timepoint, tgttzstr="UTC")
expect_equal(txt, "2010-01-02T03:04:05+00:00")

txt <- formatDouble(as.numeric(timepoint), 0, tgttzstr="UTC")
expect_equal(txt, "2010-01-02T03:04:05+00:00")

txt <- formatDouble(as.numeric(timepoint), 123456789, tgttzstr="UTC")
expect_equal(txt, "2010-01-02T03:04:05.123456789+00:00")

timepoint <- ISOdatetime(2010,1,2,3,4,5, tz="UTC")
txt <- RcppCCTZ:::formatDouble(rep(timepoint, 3), c(1001, 2002, 3003))
vals <- RcppCCTZ:::parseDouble(txt)
c1 <- rep(1262401445, 3)
c2 <- c(1001, 2002, 3003)
expect_equal(vals[,1], c1)
expect_equal(vals[,2], c2)
