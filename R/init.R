
.onAttach <- function(libname, pkgname) {
    if (Sys.info()[["sysname"]] == "Windows") {
        if (interactive()) packageStartupMessage("RcppCCTZ setting zoneinfo on Windows")
        Sys.setenv("TZDIR"=file.path(R.home(), "share", "zoneinfo"))
    }
}
