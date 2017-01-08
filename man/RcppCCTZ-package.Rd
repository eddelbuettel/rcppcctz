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
  compiler to cope with C++11.

  Windows is supported since version 0.2.0 via the \code{g++-4.9}
  compiler, but note that it provides an \emph{incomplete} C++11
  library. The \code{std::get_time} function was ported from libc++ of
  the LLVM to enable this. However, string formatting is more limited as
  the libc++ library used by \code{g++-4.9} does not provide complete
  C++11 semantics. As one example, CCTZ frequently uses \code{"\%F \%T"}
  which do not work on Windows; one has to use
  \code{"\%Y-\%m-\%d \%H:\%M:\%S"}.
  
}
\author{
  Dirk Eddelbuettel wrote the package; Dan Dillon ported \code{std::get_time}
  from LLVM's libc++; Bradley White and Greg Miller wrote the underlying CCTZ library.

  Maintainer: Dirk Eddelbuettel <edd@debian.org>
}
\references{
  The CCZT repository at \url{https://github.com/google/cctz} has additional information.
}
\keyword{ package }
\examples{
  helloMoon()
}
