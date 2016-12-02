// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#include <Rcpp.h>

#include "civil_time.h"
#include "time_zone.h"

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
Rcpp::CharacterVector helloMoon(bool verbose=false) {
    cctz::time_zone syd, nyc;
    cctz::load_time_zone("Australia/Sydney", &syd);
    cctz::load_time_zone("America/New_York", &nyc);

    // Neil Armstrong first walks on the moon
    const auto tp = cctz::convert(cctz::civil_second(1969, 7, 20, 22, 56, 0), nyc);
    const std::string s1 = cctz::format("%F %T %z", tp, nyc);
    if (verbose) Rcpp::Rcout << s1 << "\n";

    // That time in Sydney
    const std::string s2 = cctz::format("%F %T %z", tp, syd);
    if (verbose) Rcpp::Rcout << s2 << "\n";
    
    return Rcpp::CharacterVector::create(Rcpp::Named("New_York") = s1,
                                         Rcpp::Named("Sydney") = s2);
}


// from examples/example1.cc to examples/example4.cc

// [[Rcpp::export]]
void example1() {
    cctz::time_zone lax;
    load_time_zone("America/Los_Angeles", &lax);
    
    // Time Programming Fundamentals @cppcon
    const auto tp = cctz::convert(cctz::civil_second(2015, 9, 22, 9, 0, 0), lax);

    cctz::time_zone nyc;
    load_time_zone("America/New_York", &nyc);

    Rcpp::Rcout << cctz::format("Talk starts at %T %z (%Z)\n", tp, lax);
    Rcpp::Rcout << cctz::format("Talk starts at %T %z (%Z)\n", tp, nyc);

}

// [[Rcpp::export]]
int example2() {
    const std::string civil_string = "2015-09-22 09:35:00";

    cctz::time_zone lax;
    load_time_zone("America/Los_Angeles", &lax);
    std::chrono::system_clock::time_point tp;
    const bool ok = cctz::parse("%Y-%m-%d %H:%M:%S", civil_string, lax, &tp);
    if (!ok) return -1;

    const auto now = std::chrono::system_clock::now();
    const std::string s = now > tp ? "running long!" : "on time!";
    Rcpp::Rcout << "Talk " << s << "\n";
    return 0;
}

// [[Rcpp::export]]
void example3() {
    cctz::time_zone lax;
    load_time_zone("America/Los_Angeles", &lax);

    const auto now = std::chrono::system_clock::now();
    const cctz::civil_second cs = cctz::convert(now, lax);
    
    // First day of month, 6 months from now.
    const auto then = cctz::convert(cctz::civil_month(cs) + 6, lax);
    
    Rcpp::Rcout << cctz::format("Now: %F %T %z\n", now, lax);
    Rcpp::Rcout << cctz::format("6mo: %F %T %z\n", then, lax);
}


template <typename D>
cctz::time_point<cctz::sys_seconds> FloorDay(cctz::time_point<D> tp,
                                             cctz::time_zone tz) {
  return cctz::convert(cctz::civil_day(cctz::convert(tp, tz)), tz);
}

// [[Rcpp::export]]
void example4() {
    cctz::time_zone lax;
    load_time_zone("America/Los_Angeles", &lax);
    const auto now = std::chrono::system_clock::now();
    const auto day = FloorDay(now, lax);
    Rcpp::Rcout << cctz::format("Now: %F %T %z\n", now, lax);
    Rcpp::Rcout << cctz::format("Day: %F %T %z\n", day, lax);
}


// from time_zone_format_test.cc
//
// [[Rcpp::export]]
void exampleFormat() {

    const cctz::time_zone tz = cctz::utc_time_zone();
    cctz::time_point<std::chrono::nanoseconds> tp = std::chrono::system_clock::from_time_t(0);
    tp += std::chrono::hours(3) + std::chrono::minutes(4) + std::chrono::seconds(5);
    tp += std::chrono::milliseconds(6) + std::chrono::microseconds(7) +
        std::chrono::nanoseconds(8);

    std::string txt = format("%H:%M:%E15S", tp, tz);

    Rcpp::Rcout << "15 digit precision on subsecond time: " << txt << std::endl;    
}

