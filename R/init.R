
## On Windows, set TZDIR to the zoneinfo shipped with R
.onLoad <- function(libname, pkgname) {
    if (Sys.info()[["sysname"]] == "Windows" && Sys.getenv("TZDIR") == "") {
        Sys.setenv("TZDIR"=file.path(R.home(), "share", "zoneinfo"))
    }
}

