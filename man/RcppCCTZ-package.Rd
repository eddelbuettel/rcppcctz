\name{RcppCCTZ-package}
\alias{RcppCCTZ-package}
\alias{RcppCCTZ}
\alias{example0}
\alias{example1}
\alias{example2}
\alias{example3}
\alias{example4}
\alias{helloMoon}
\alias{exampleFormat}
\docType{package}
\title{
  A Simple Wrapper to the CCTZ Library for Time Zone Calculations
}
\description{
  CCTZ contains two underlying libraries which build on the C++11
  library \code{chrono}.  The first covers \emph{civil time} for
  computing with human-scale time such as dates and time. It is
  header-only. The second covers time zones and allow translation
  between absolute time and civil time.

  RcppCCTZ brings CCTZ to R by means of Rcpp.
}
\details{
  CCTZ requires a valid timezone library as well as recent-enough
  compiler to cope with C++11.  This means that Windows is currently not
  supported..
}
\author{
  Dirk Eddelbuettel wrote the package; Bradley White and Greg Miller
  wrote the underlying CCTZ library,

  Maintainer: Dirk Eddelbuettel <edd@debian.org>
}
\references{
  The CCZT repository at \url{https://github.com/google/cctz} has additional information.
}
\keyword{ package }
\examples{
  helloMoon()
}
