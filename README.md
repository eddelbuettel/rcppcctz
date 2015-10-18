## RcppCCTZ [![Build Status](https://travis-ci.org/eddelbuettel/rcppcctz.svg)](https://travis-ci.org/eddelbuettel/rcpptoml) [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html) 

Rcpp bindings for [CCTZ](https://github.com/google/cctz)

### What is CCTZ?

[CCTZ](https://github.com/google/cctz) (C++ Time Zone) is a library for
translating between absolute times and civil times using the rules
defined by a time zone. 

This package wraps CCTZ for use by R via
[Rcpp](http://dirk.eddelbuettel.com/code/rcpp.html).

### Requirements

Due to internal architectural choices only builds on 64-bit operating
systems, and it also depends on timezone files typically found in
`/usr/share/zoneinfo` which requires a Unix-alike OS such as Linux or OS
X. Old school Unix variants may work.

### Status

Preliminary. It builds, and the example functions are accessible from R.

### Author

Dirk Eddelbuettel

### License

GPL (>= 2)


