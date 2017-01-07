// =============================================================================
// The code in this file has been adapted from code in the locale header from
// the LLVM libc++ project and is licensed as follows.
//
// =============================================================================
// libc++ License
// =============================================================================
// 
// The libc++ library is dual licensed under both the University of Illinois
// "BSD-Like" license and the MIT license.  As a user of this code you may
// choose to use it under either license.  As a contributor, you agree to allow
// your code to be used under both.
// 
// Full text of the relevant licenses is included below.
// 
// =============================================================================
// 
// University of Illinois/NCSA
// Open Source License
// 
// Copyright (c) 2009-2016 by the contributors listed in CREDITS.TXT
// 
// All rights reserved.
// 
// Developed by:
// 
//     LLVM Team
// 
//     University of Illinois at Urbana-Champaign
// 
//     http://llvm.org
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// with the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimers.
// 
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimers in the
//       documentation and/or other materials provided with the distribution.
// 
//     * Neither the names of the LLVM Team, University of Illinois at
//       Urbana-Champaign, nor the names of its contributors may be used to
//       endorse or promote products derived from this Software without specific
//       prior written permission.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH
// THE SOFTWARE.
// 
// =============================================================================
// 
// Copyright (c) 2009-2014 by the contributors listed in CREDITS.TXT
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef RCPP_CCTZ_GET_TIME
#define RCPP_CCTZ_GET_TIME

#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iterator>

namespace std_backport
{
template <class T>
class fake_unique_ptr
{
private:
    T* p_;
    
public:
    fake_unique_ptr() : p_(NULL) {}
    fake_unique_ptr(T *p) : p_(p) {}
    
    ~fake_unique_ptr()
    {
        if (p_) free(p_);
    }
    
    T *get() { return p_; }
    
    void reset(T *p)
    {
        if (p_) free(p_);
        p_ = p;
    }
};

template <class InputIterator, class ForwardIterator, class Ctype>
ForwardIterator
scan_keyword(InputIterator& b, InputIterator e,
               ForwardIterator kb, ForwardIterator ke,
               const Ctype& ct, std::ios_base::iostate& err,
               bool case_sensitive = true)
{
    typedef typename std::iterator_traits<InputIterator>::value_type CharT;
    size_t nkw = static_cast<size_t>(distance(kb, ke));
    const unsigned char doesnt_match = '\0';
    const unsigned char might_match = '\1';
    const unsigned char does_match = '\2';
    unsigned char statbuf[100];
    unsigned char* status = statbuf;
    fake_unique_ptr< unsigned char > stat_hold;
    
    if (nkw > sizeof(statbuf))
    {
        status = (unsigned char*)malloc(nkw);
        
        if (status == 0)
        {
            throw std::bad_alloc();
        }
        
        stat_hold.reset(status);
    }
    size_t n_might_match = nkw;  // At this point, any keyword might match
    size_t n_does_match = 0;       // but none of them definitely do
    // Initialize all statuses to might_match, except for "" keywords are does_match
    unsigned char* st = status;
    for (ForwardIterator ky = kb; ky != ke; ++ky, (void) ++st)
    {
        if (!ky->empty())
            *st = might_match;
        else
        {
            *st = does_match;
            --n_might_match;
            ++n_does_match;
        }
    }
    // While there might be a match, test keywords against the next CharT
    for (size_t indx = 0; b != e && n_might_match > 0; ++indx)
    {
        // Peek at the next CharT but don't consume it
        CharT c = *b;
        if (!case_sensitive)
            c = ct.toupper(c);
        bool consume = false;
        // For each keyword which might match, see if the indx character is c
        // If a match if found, consume c
        // If a match is found, and that is the last character in the keyword,
        //    then that keyword matches.
        // If the keyword doesn't match this character, then change the keyword
        //    to doesn't match
        st = status;
        for (ForwardIterator ky = kb; ky != ke; ++ky, (void) ++st)
        {
            if (*st == might_match)
            {
                CharT kc = (*ky)[indx];
                if (!case_sensitive)
                    kc = ct.toupper(kc);
                if (c == kc)
                {
                    consume = true;
                    if (ky->size() == indx+1)
                    {
                        *st = does_match;
                        --n_might_match;
                        ++n_does_match;
                    }
                }
                else
                {
                    *st = doesnt_match;
                    --n_might_match;
                }
            }
        }
        // consume if we matched a character
        if (consume)
        {
            ++b;
            // If we consumed a character and there might be a matched keyword that
            //   was marked matched on a previous iteration, then such keywords
            //   which are now marked as not matching.
            if (n_might_match + n_does_match > 1)
            {
                st = status;
                for (ForwardIterator ky = kb; ky != ke; ++ky, (void) ++st)
                {
                    if (*st == does_match && ky->size() != indx+1)
                    {
                        *st = doesnt_match;
                        --n_does_match;
                    }
                }
            }
        }
    }
    // We've exited the loop because we hit eof and/or we have no more "might matches".
    if (b == e)
        err |= std::ios_base::eofbit;
    // Return the first matching result
    for (st = status; kb != ke; ++kb, (void) ++st)
        if (*st == does_match)
            break;
    if (kb == ke)
    {
        err |= std::ios_base::failbit;
    }
    
    return kb;
}

template <class CharT, class InputIterator>
int
get_up_to_n_digits(InputIterator& b, InputIterator e,
                     std::ios_base::iostate& err, const std::ctype<CharT>& ct, int n)
{
    // Precondition:  n >= 1
    if (b == e)
    {
        err |= std::ios_base::eofbit | std::ios_base::failbit;
        return 0;
    }
    // get first digit
    CharT c = *b;
    if (!ct.is(std::ctype_base::digit, c))
    {
        err |= std::ios_base::failbit;
        return 0;
    }
    int r = ct.narrow(c, 0) - '0';
    for (++b, (void) --n; b != e && n > 0; ++b, (void) --n)
    {
        // get next digit
        c = *b;
        if (!ct.is(std::ctype_base::digit, c))
            return r;
        r = r * 10 + ct.narrow(c, 0) - '0';
    }
    if (b == e)
        err |= std::ios_base::eofbit;
    return r;
}

template <class CharT>
class time_get_c_storage
{
protected:
    typedef std::basic_string<CharT> string_type;
    
    string_type am_pm_[2];
    
    virtual const string_type* am_pm() const;
    virtual const string_type& c() const;
    virtual const string_type& r() const;
    virtual const string_type& x() const;
    virtual const string_type& X() const;

public:
    time_get_c_storage(const std::locale& loc);
    ~time_get_c_storage() {}
};

template<>
time_get_c_storage<char>::time_get_c_storage(const std::locale& loc)
{
    std::basic_ostringstream<char> os;
    const std::time_put<char> &tp = std::use_facet< std::time_put<char> >(loc);
    
    std::tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 11;
    tm.tm_mday = 0;
    tm.tm_mon = 0;
    tm.tm_year = 0;
    tm.tm_wday = 0;
    tm.tm_yday = 0;
    tm.tm_isdst = -1;
    
    std::ostreambuf_iterator<char> out(os);
    tp.put(out, os, ' ', &tm, 'p');
    am_pm_[0] = os.str();
    
    os.str("");
    std::ostreambuf_iterator<char> out2(os);
    tm.tm_hour = 13;
    tp.put(out2, os, ' ', &tm, 'p');
    am_pm_[1] = os.str();
}

template<>
time_get_c_storage<wchar_t>::time_get_c_storage(const std::locale& loc)
{
    std::basic_ostringstream<wchar_t> os;
    const std::time_put<wchar_t> &tp = std::use_facet< std::time_put< wchar_t > >(loc);
    
    std::tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 11;
    tm.tm_mday = 0;
    tm.tm_mon = 0;
    tm.tm_year = 0;
    tm.tm_wday = 0;
    tm.tm_yday = 0;
    tm.tm_isdst = -1;
    
    std::ostreambuf_iterator<wchar_t> out(os);
    tp.put(out, os, L' ', &tm, L'p');
    am_pm_[0] = os.str();
    
    os.str(L"");
    std::ostreambuf_iterator<wchar_t> out2(os);
    
    tm.tm_hour = 13;
    tp.put(out2, os, L' ', &tm, L'p');
    am_pm_[1] = os.str();
} 

template <>
const std::string*
time_get_c_storage<char>::am_pm() const
{
    return am_pm_;
}

template <>
const std::wstring*
time_get_c_storage<wchar_t>::am_pm() const
{
    return am_pm_;
}

template <>
const std::string&
time_get_c_storage<char>::x() const
{
    static std::string s("%m/%d/%y");
    return s;
}

template <>
const std::wstring&
time_get_c_storage<wchar_t>::x() const
{
    static std::wstring s(L"%m/%d/%y");
    return s;
}

template <>
const std::string&
time_get_c_storage<char>::X() const
{
    static std::string s("%H:%M:%S");
    return s;
}

template <>
const std::wstring&
time_get_c_storage<wchar_t>::X() const
{
    static std::wstring s(L"%H:%M:%S");
    return s;
}

template <>
const std::string&
time_get_c_storage<char>::c() const
{
    static std::string s("%a %b %d %H:%M:%S %Y");
    return s;
}

template <>
const std::wstring&
time_get_c_storage<wchar_t>::c() const
{
    static std::wstring s(L"%a %b %d %H:%M:%S %Y");
    return s;
}

template <>
const std::string&
time_get_c_storage<char>::r() const
{
    static std::string s("%I:%M:%S %p");
    return s;
}

template <>
const std::wstring&
time_get_c_storage<wchar_t>::r() const
{
    static std::wstring s(L"%I:%M:%S %p");
    return s;
}

template <class CharT, class InputIterator>
class time_parser : public time_get_c_storage<CharT>
{
private:
    const std::time_get<CharT, InputIterator> &tg_;
    
public:
    typedef CharT                  char_type;
    typedef InputIterator          iter_type;
    typedef std::time_base::dateorder    dateorder;
    typedef std::basic_string<char_type> string_type;
    
    time_parser(const std::locale& loc) : time_get_c_storage<CharT>(loc),
        tg_(std::use_facet< std::time_get<CharT, InputIterator> >(loc))
    {
    }
    
    iter_type get(iter_type b, iter_type e, std::ios_base& iob,
                  std::ios_base::iostate& err, tm *tm,
                  char fmt, char mod = 0) const
    {
        return do_get(b, e, iob, err, tm, fmt, mod);
    }

    iter_type get(iter_type b, iter_type e, std::ios_base& iob,
                  std::ios_base::iostate& err, tm* tm,
                  const char_type* fmtb, const char_type* fmte) const;
    
private:
    InputIterator do_get(iter_type b, iter_type e,
                          std::ios_base& iob,
                          std::ios_base::iostate& err, tm* tm,
                          char fmt, char) const;
    
    void get_white_space(iter_type& b, iter_type e,
                           std::ios_base::iostate& err, const std::ctype<char_type>& ct) const;
    void get_percent(iter_type& b, iter_type e, std::ios_base::iostate& err,
                       const std::ctype<char_type>& ct) const;
    void get_day(int& d,
                   iter_type& b, iter_type e,
                   std::ios_base::iostate& err,
                   const std::ctype<char_type>& ct) const;
    void get_month(int& m,
                     iter_type& b, iter_type e,
                     std::ios_base::iostate& err,
                     const std::ctype<char_type>& ct) const;
    void get_year(int& y,
                   iter_type& b, iter_type e,
                   std::ios_base::iostate& err,
                   const std::ctype<char_type>& ct) const;
    void get_year4(int& y,
                    iter_type& b, iter_type e,
                    std::ios_base::iostate& err,
                    const std::ctype<char_type>& ct) const;
    void get_hour(int& d,
                    iter_type& b, iter_type e,
                    std::ios_base::iostate& err,
                    const std::ctype<char_type>& ct) const;
    void get_12_hour(int& h,
                       iter_type& b, iter_type e,
                       std::ios_base::iostate& err,
                       const std::ctype<char_type>& ct) const;
    void get_am_pm(int& h,
                     iter_type& b, iter_type e,
                     std::ios_base::iostate& err,
                     const std::ctype<char_type>& ct) const;
    void get_minute(int& m,
                      iter_type& b, iter_type e,
                      std::ios_base::iostate& err,
                      const std::ctype<char_type>& ct) const;
    void get_second(int& s,
                      iter_type& b, iter_type e,
                      std::ios_base::iostate& err,
                      const std::ctype<char_type>& ct) const;
    void get_weekday(int& w,
                       iter_type& b, iter_type e,
                       std::ios_base::iostate& err,
                       const std::ctype<char_type>& ct) const;
    void get_day_year_num(int& w,
                            iter_type& b, iter_type e,
                            std::ios_base::iostate& err,
                            const std::ctype<char_type>& ct) const;

};

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_day(int& d,
                                            iter_type& b, iter_type e,
                                            std::ios_base::iostate& err,
                                            const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 2);
    if (!(err & std::ios_base::failbit) && 1 <= t && t <= 31)
        d = t;
    else
    {
        err |= std::ios_base::failbit;
    }
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_month(int& m,
                                              iter_type& b, iter_type e,
                                              std::ios_base::iostate& err,
                                              const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 2) - 1;
    if (!(err & std::ios_base::failbit) && t <= 11)
        m = t;
    else
    {
        err |= std::ios_base::failbit;
    }
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_year(int& y,
                                             iter_type& b, iter_type e,
                                             std::ios_base::iostate& err,
                                             const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 4);
    if (!(err & std::ios_base::failbit))
    {
        if (t < 69)
            t += 2000;
        else if (69 <= t && t <= 99)
            t += 1900;
        y = t - 1900;
    }
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_year4(int& y,
                                              iter_type& b, iter_type e,
                                              std::ios_base::iostate& err,
                                              const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 4);
    if (!(err & std::ios_base::failbit))
        y = t - 1900;
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_hour(int& h,
                                             iter_type& b, iter_type e,
                                             std::ios_base::iostate& err,
                                             const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 2);
    if (!(err & std::ios_base::failbit) && t <= 23)
        h = t;
    else
    {
        err |= std::ios_base::failbit;
    }
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_12_hour(int& h,
                                                iter_type& b, iter_type e,
                                                std::ios_base::iostate& err,
                                                const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 2);
    if (!(err & std::ios_base::failbit) && 1 <= t && t <= 12)
        h = t;
    else
    {
        err |= std::ios_base::failbit;
    }
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_minute(int& m,
                                               iter_type& b, iter_type e,
                                               std::ios_base::iostate& err,
                                               const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 2);
    if (!(err & std::ios_base::failbit) && t <= 59)
        m = t;
    else
    {
        err |= std::ios_base::failbit;
    }
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_second(int& s,
                                               iter_type& b, iter_type e,
                                               std::ios_base::iostate& err,
                                               const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 2);
    if (!(err & std::ios_base::failbit) && t <= 60)
        s = t;
    else
    {
        err |= std::ios_base::failbit;
    }
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_weekday(int& w,
                                                iter_type& b, iter_type e,
                                                std::ios_base::iostate& err,
                                                const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 1);
    if (!(err & std::ios_base::failbit) && t <= 6)
        w = t;
    else
        err |= std::ios_base::failbit;
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_day_year_num(int& d,
                                                     iter_type& b, iter_type e,
                                                     std::ios_base::iostate& err,
                                                     const std::ctype<char_type>& ct) const
{
    int t = get_up_to_n_digits(b, e, err, ct, 3);
    if (!(err & std::ios_base::failbit) && t <= 365)
        d = t;
    else
    {
        err |= std::ios_base::failbit;
    }
}


template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_white_space(iter_type& b, iter_type e,
                                                    std::ios_base::iostate& err,
                                                    const std::ctype<char_type>& ct) const
{
    for (; b != e && ct.is(std::ctype_base::space, *b); ++b)
        ;
    if (b == e)
        err |= std::ios_base::eofbit;
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_am_pm(int& h,
                                              iter_type& b, iter_type e,
                                              std::ios_base::iostate& err,
                                              const std::ctype<char_type>& ct) const
{
    const string_type* ap = this->am_pm();
    if (ap[0].size() + ap[1].size() == 0)
    {
        err |= std::ios_base::failbit;
        return;
    }
    std::ptrdiff_t i = scan_keyword(b, e, ap, ap+2, ct, err, false) - ap;
    if (i == 0 && h == 12)
        h = 0;
    else if (i == 1 && h < 12)
        h += 12;
}

template <class CharT, class InputIterator>
void
time_parser<CharT, InputIterator>::get_percent(iter_type& b, iter_type e,
                                                std::ios_base::iostate& err,
                                                const std::ctype<char_type>& ct) const
{
    if (b == e)
    {
        err |= std::ios_base::eofbit | std::ios_base::failbit;
        return;
    }
    if (ct.narrow(*b, 0) != '%')
    {
        err |= std::ios_base::failbit;
    }
    else if(++b == e)
    {
        err |= std::ios_base::eofbit;
    }
}

template <class CharT, class InputIterator>
InputIterator
time_parser<CharT, InputIterator>::get(iter_type b, iter_type e,
                                      std::ios_base& iob,
                                      std::ios_base::iostate& err, tm* tm,
                                      const char_type* fmtb, const char_type* fmte) const
{
    const std::ctype<char_type>& ct = std::use_facet<std::ctype<char_type> >(iob.getloc());

    err = std::ios_base::goodbit;
    while (fmtb != fmte && err == std::ios_base::goodbit)
    {
        if (b == e)
        {
            err = std::ios_base::failbit;
            break;
        }
        if (ct.narrow(*fmtb, 0) == '%')
        {
            if (++fmtb == fmte)
            {
                err = std::ios_base::failbit;
                break;
            }
            char cmd = ct.narrow(*fmtb, 0);
            char opt = '\0';
            if (cmd == 'E' || cmd == '0')
            {
                if (++fmtb == fmte)
                {
                    err = std::ios_base::failbit;
                    break;
                }
                opt = cmd;
                cmd = ct.narrow(*fmtb, 0);
            }
            b = do_get(b, e, iob, err, tm, cmd, opt);
            ++fmtb;
        }
        else if (ct.is(std::ctype_base::space, *fmtb))
        {
            for (++fmtb; fmtb != fmte && ct.is(std::ctype_base::space, *fmtb); ++fmtb)
                ;
            for (        ;    b != e    && ct.is(std::ctype_base::space, *b);    ++b)
                ;
        }
        else if (ct.toupper(*b) == ct.toupper(*fmtb))
        {
            ++b;
            ++fmtb;
        }
        else
        {
            err = std::ios_base::failbit;
        }
    }
    if (b == e)
        err |= std::ios_base::eofbit;
    return b;
}

template <class CharT, class InputIterator>
InputIterator
time_parser<CharT, InputIterator>::do_get(iter_type b, iter_type e,
                                         std::ios_base& iob,
                                         std::ios_base::iostate& err, tm* tm,
                                         char fmt, char) const
{
    err = std::ios_base::goodbit;
    const std::ctype<char_type>& ct = std::use_facet<std::ctype<char_type> >(iob.getloc());
    
    switch (fmt)
    {
    case 'a':
    case 'A':
        b = tg_.get_weekday(b, e, iob, err, tm);
        break;
    case 'b':
    case 'B':
    case 'h':
        b = tg_.get_monthname(b, e, iob, err, tm);
        break;
    case 'c':
        {
        const string_type& fm = this->c();
        b = get(b, e, iob, err, tm, fm.data(), fm.data() + fm.size());
        }
        break;
    case 'd':
    case 'e':
        get_day(tm->tm_mday, b, e, err, ct);
        break;
    case 'D':
        {
        const char_type fm[] = {'%', 'm', '/', '%', 'd', '/', '%', 'y'};
        b = get(b, e, iob, err, tm, fm, fm + sizeof(fm)/sizeof(fm[0]));
        }
        break;
    case 'F':
        {
        const char_type fm[] = {'%', 'Y', '-', '%', 'm', '-', '%', 'd'};
        b = get(b, e, iob, err, tm, fm, fm + sizeof(fm)/sizeof(fm[0]));
        }
        break;
    case 'H':
        get_hour(tm->tm_hour, b, e, err, ct);
        break;
    case 'I':
        get_12_hour(tm->tm_hour, b, e, err, ct);
        break;
    case 'j':
        get_day_year_num(tm->tm_yday, b, e, err, ct);
        break;
    case 'm':
        get_month(tm->tm_mon, b, e, err, ct);
        break;
    case 'M':
        get_minute(tm->tm_min, b, e, err, ct);
        break;
    case 'n':
    case 't':
        get_white_space(b, e, err, ct);
        break;
    case 'p':
        get_am_pm(tm->tm_hour, b, e, err, ct);
        break;
    case 'r':
        {
        const char_type fm[] = {'%', 'I', ':', '%', 'M', ':', '%', 'S', ' ', '%', 'p'};
        b = get(b, e, iob, err, tm, fm, fm + sizeof(fm)/sizeof(fm[0]));
        }
        break;
    case 'R':
        {
        const char_type fm[] = {'%', 'H', ':', '%', 'M'};
        b = get(b, e, iob, err, tm, fm, fm + sizeof(fm)/sizeof(fm[0]));
        }
        break;
    case 'S':
        get_second(tm->tm_sec, b, e, err, ct);
        break;
    case 'T':
        {
        const char_type fm[] = {'%', 'H', ':', '%', 'M', ':', '%', 'S'};
        b = get(b, e, iob, err, tm, fm, fm + sizeof(fm)/sizeof(fm[0]));
        }
        break;
    case 'w':
        get_weekday(tm->tm_wday, b, e, err, ct);
        break;
    case 'x':
        return tg_.get_date(b, e, iob, err, tm);
    case 'X':
        {
        const string_type& fm = this->X();
        b = get(b, e, iob, err, tm, fm.data(), fm.data() + fm.size());
        }
        break;
    case 'y':
        get_year(tm->tm_year, b, e, err, ct);
        break;
    case 'Y':
        get_year4(tm->tm_year, b, e, err, ct);
        break;
    case '%':
        get_percent(b, e, err, ct);
        break;
    default:
        err |= std::ios_base::failbit;
    }
    return b;
}
        
        
        
        
template <class CharT> class iom_t9;

template <class CharT, class Traits>
std::basic_istream<CharT, Traits>&
operator>>(std::basic_istream<CharT, Traits>& is, const iom_t9<CharT>& x);

template <class CharT>
class iom_t9
{
    tm* tm_;
    const CharT* fmt_;
public:
    iom_t9(tm* tm, const CharT* fmt)
        : tm_(tm), fmt_(fmt) {}

    template <class _Cp, class Traits>
    friend
    std::basic_istream<_Cp, Traits>&
    operator>>(std::basic_istream<_Cp, Traits>& is, const iom_t9<_Cp>& x);
};


template <class CharT, class Traits>
std::basic_istream<CharT, Traits>&
operator>>(std::basic_istream<CharT, Traits>& is, const iom_t9<CharT>& x)
{
        typename std::basic_istream<CharT, Traits>::sentry s(is);
        if (s)
        {
            typedef std::istreambuf_iterator<CharT, Traits> _Ip;
            typedef time_parser<CharT, _Ip> _Fp;
            std::ios_base::iostate err = std::ios_base::goodbit;
            const _Fp tf(is.getloc());
            tf.get(_Ip(is), _Ip(), is, err, x.tm_,
                     x.fmt_, x.fmt_ + Traits::length(x.fmt_));
            is.setstate(err);
        }
    return is;
}


template <class CharT>
inline
iom_t9<CharT>
get_time(tm* tm, const CharT* fmt)
{
    return iom_t9<CharT>(tm, fmt);
}

}

#endif