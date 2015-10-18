// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#include <Rcpp.h>

#include "src/cctz.h"

// from examples/classic.cc
// 
std::string Format(const std::string& fmt, const std::tm& tm) {
  char buf[100];
  std::strftime(buf, sizeof(buf), fmt.c_str(), &tm);
  return buf;
}

// [[Rcpp::export]]
void example0() {
    const std::time_t now = std::time(nullptr);

    std::tm tm_utc;
    gmtime_r(&now, &tm_utc);
    Rcpp::Rcout << Format("UTC: %F %T\n", tm_utc);

    std::tm tm_local;
    localtime_r(&now, &tm_local);
    Rcpp::Rcout << Format("Local: %F %T\n", tm_local);
}

// from examples/hello.cc
// 
// [[Rcpp::export]]
int helloMoon() {
    cctz::TimeZone syd;
    if (!cctz::LoadTimeZone("Australia/Sydney", &syd)) return -1;

    // Neil Armstrong first walks on the moon
    const auto tp1 = cctz::MakeTime(1969, 7, 21, 12, 56, 0, syd);

    const std::string s = cctz::Format("%F %T %z", tp1, syd);
    Rcpp::Rcout << s << "\n";

    cctz::TimeZone nyc;
    cctz::LoadTimeZone("America/New_York", &nyc);

    const auto tp2 = cctz::MakeTime(1969, 7, 20, 22, 56, 0, nyc);
    return tp2 == tp1 ? 0 : 1;
}


// from examples/example1.cc to examples/example4.cc

// [[Rcpp::export]]
void example1() {
    cctz::TimeZone lax;
    LoadTimeZone("America/Los_Angeles", &lax);
    
    // Time Programming Fundamentals @cppcon
    const auto tp = cctz::MakeTime(2015, 9, 22, 9, 0, 0, lax);

    cctz::TimeZone nyc;
    LoadTimeZone("America/New_York", &nyc);

    Rcpp::Rcout << cctz::Format("Talk starts at %T %z (%Z)\n", tp, lax);
    Rcpp::Rcout << cctz::Format("Talk starts at %T %z (%Z)\n", tp, nyc);

}

// [[Rcpp::export]]
int example2() {
    const std::string civil_string = "2015-09-22 09:35:00";

    cctz::TimeZone lax;
    LoadTimeZone("America/Los_Angeles", &lax);
    std::chrono::system_clock::time_point tp;
    const bool ok = cctz::Parse("%Y-%m-%d %H:%M:%S", civil_string, lax, &tp);
    if (!ok) return -1;

    const auto now = std::chrono::system_clock::now();
    const std::string s = now > tp ? "running long!" : "on time!";
    Rcpp::Rcout << "Talk " << s << "\n";
    return 0;
}

// [[Rcpp::export]]
void example3() {
    cctz::TimeZone lax;
    LoadTimeZone("America/Los_Angeles", &lax);

    const auto now = std::chrono::system_clock::now();
    const cctz::Breakdown bd = cctz::BreakTime(now, lax);
    
    // First day of month, 6 months from now.
    const auto then = cctz::MakeTime(bd.year, bd.month + 6, 1, 0, 0, 0, lax);
    
    Rcpp::Rcout << cctz::Format("Now: %F %T %z\n", now, lax);
    Rcpp::Rcout << cctz::Format("6mo: %F %T %z\n", then, lax);
}



template <typename D>
cctz::time_point<cctz::seconds64> FloorDay(cctz::time_point<D> tp,
                                           cctz::TimeZone tz) {
    const cctz::Breakdown bd = cctz::BreakTime(tp, tz);
    const cctz::TimeInfo ti =
        cctz::MakeTimeInfo(bd.year, bd.month, bd.day, 0, 0, 0, tz);
    return ti.kind == cctz::TimeInfo::Kind::SKIPPED ? ti.trans : ti.pre;
}

// [[Rcpp::export]]
void example4() {
    cctz::TimeZone lax;
    LoadTimeZone("America/Los_Angeles", &lax);
    const auto now = std::chrono::system_clock::now();
    const auto day = FloorDay(now, lax);
    Rcpp::Rcout << cctz::Format("Now: %F %T %z\n", now, lax);
    Rcpp::Rcout << cctz::Format("Day: %F %T %z\n", day, lax);
}
