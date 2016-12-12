\name{NEWS}
\title{News for Package \pkg{RcppCCTZ}}

\section{Changes in version 0.1.0 (2016-12-11)}{
  \itemize{
    \item Synchronized with \code{CCTZ} upstream.
    \item New parsing and formating helpers for Datetime vectors
    \item New parsing and formating helpers for (two) \code{double}
    vectors representing full \code{std::chrono} nanosecond resolutions
    \item Updated documentation and examples.
  }
}

\section{Changes in version 0.0.5 (2016-07-09)}{
  \itemize{
    \item New utility example functions \code{toTz()} and \code{tzDiff}
    \item Synchronized with small upstream change for additional
    \code{#ifdef} for compiler differentiation
  }
}

\section{Changes in version 0.0.4 (2016-04-17)}{
  \itemize{
    \item Synchronized with \code{CCTZ} v2 upstream.
    \item Updated \code{examples.cpp} accordingly
  }
}

\section{Changes in version 0.0.3 (2016-01-17)}{
  \itemize{
    \item Synchronized with \code{CCTZ} upstream.
  }
}

\section{Changes in version 0.0.2 (2015-12-02)}{
  \itemize{
    \item Additional \code{#ifdef} statements suggested by Bradley White
    in \code{CCTZ} ticket #5 permitting compilation on Solaris -- with thanks
    to Jeroen for testing our branch.
  }
}

\section{Changes in version 0.0.1 (2015-12-01)}{
  \itemize{
    \item Initial CRAN upload.
    \item Package is functional and provides examples.
  }
}
