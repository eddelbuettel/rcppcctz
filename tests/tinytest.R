
# RcppCCTZ: Rcpp-based R bindings for CCTZ
#
# Copyright (C) 2019         Dirk Eddelbuettel
#
# This file is part of RcppCCTZ.
#
# RcppCCTZ is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 2 of the
# License, or (at your option) any later version.
#
# RcppCCTZ is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with RcppCCTZ.  If not, see <http://www.gnu.org/licenses/>.

if (requireNamespace("tinytest", quietly=TRUE)) {

    ## Set a seed to make the test deterministic
    set.seed(42)

    ## there are several more granular ways to test files in a
    ## tinytest directory, see its package vignette; tests can also
    ## run once the package is installed using the same command

    tinytest::test_package("RcppCCTZ")

}
