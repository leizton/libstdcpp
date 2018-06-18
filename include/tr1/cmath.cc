// TR1 cmath -*- C++ -*-

// Copyright (C) 2006-2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file tr1/cmath
 *  This is a TR1 C++ Library header. 
 */

#ifndef _GLIBCXX_TR1_CMATH
#define _GLIBCXX_TR1_CMATH 1

#pragma GCC system_header

#include <cmath>

#ifdef _GLIBCXX_USE_C99_MATH_TR1

#undef acosh
#undef acoshf
#undef acoshl
#undef asinh
#undef asinhf
#undef asinhl
#undef atanh
#undef atanhf
#undef atanhl
#undef cbrt
#undef cbrtf
#undef cbrtl
#undef copysign
#undef copysignf
#undef copysignl
#undef erf
#undef erff
#undef erfl
#undef erfc
#undef erfcf
#undef erfcl
#undef exp2
#undef exp2f
#undef exp2l
#undef expm1
#undef expm1f
#undef expm1l
#undef fdim
#undef fdimf
#undef fdiml
#undef fma
#undef fmaf
#undef fmal
#undef fmax
#undef fmaxf
#undef fmaxl
#undef fmin
#undef fminf
#undef fminl
#undef hypot
#undef hypotf
#undef hypotl
#undef ilogb
#undef ilogbf
#undef ilogbl
#undef lgamma
#undef lgammaf
#undef lgammal
#undef llrint
#undef llrintf
#undef llrintl
#undef llround
#undef llroundf
#undef llroundl
#undef log1p
#undef log1pf
#undef log1pl
#undef log2
#undef log2f
#undef log2l
#undef logb
#undef logbf
#undef logbl
#undef lrint
#undef lrintf
#undef lrintl
#undef lround
#undef lroundf
#undef lroundl
#undef nan
#undef nanf
#undef nanl
#undef nearbyint
#undef nearbyintf
#undef nearbyintl
#undef nextafter
#undef nextafterf
#undef nextafterl
#undef nexttoward
#undef nexttowardf
#undef nexttowardl
#undef remainder
#undef remainderf
#undef remainderl
#undef remquo
#undef remquof
#undef remquol
#undef rint
#undef rintf
#undef rintl
#undef round
#undef roundf
#undef roundl
#undef scalbln
#undef scalblnf
#undef scalblnl
#undef scalbn
#undef scalbnf
#undef scalbnl
#undef tgamma
#undef tgammaf
#undef tgammal
#undef trunc
#undef truncf
#undef truncl

#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace tr1
{
#if _GLIBCXX_USE_C99_MATH_TR1

  // Using declarations to bring names from libc's <math.h> into std::tr1.

  // types
  using ::double_t;
  using ::float_t;

  // functions
  using ::acosh;
  using ::acoshf;
  using ::acoshl;

  using ::asinh;
  using ::asinhf;
  using ::asinhl;

  using ::atanh;
  using ::atanhf;
  using ::atanhl;

  using ::cbrt;
  using ::cbrtf;
  using ::cbrtl;

  using ::copysign;
  using ::copysignf;
  using ::copysignl;

  using ::erf;
  using ::erff;
  using ::erfl;

  using ::erfc;
  using ::erfcf;
  using ::erfcl;

  using ::exp2;
  using ::exp2f;
  using ::exp2l;

  using ::expm1;
  using ::expm1f;
  using ::expm1l;

  using ::fdim;
  using ::fdimf;
  using ::fdiml;

  using ::fma;
  using ::fmaf;
  using ::fmal;

  using ::fmax;
  using ::fmaxf;
  using ::fmaxl;

  using ::fmin;
  using ::fminf;
  using ::fminl;

  using ::hypot;
  using ::hypotf;
  using ::hypotl;

  using ::ilogb;
  using ::ilogbf;
  using ::ilogbl;

  using ::lgamma;
  using ::lgammaf;
  using ::lgammal;

  using ::llrint;
  using ::llrintf;
  using ::llrintl;

  using ::llround;
  using ::llroundf;
  using ::llroundl;

  using ::log1p;
  using ::log1pf;
  using ::log1pl;

  using ::log2;
  using ::log2f;
  using ::log2l;

  using ::logb;
  using ::logbf;
  using ::logbl;

  using ::lrint;
  using ::lrintf;
  using ::lrintl;

  using ::lround;
  using ::lroundf;
  using ::lroundl;

  using ::nan;
  using ::nanf;
  using ::nanl;

  using ::nearbyint;
  using ::nearbyintf;
  using ::nearbyintl;

  using ::nextafter;
  using ::nextafterf;
  using ::nextafterl;

  using ::nexttoward;
  using ::nexttowardf;
  using ::nexttowardl;

  using ::remainder;
  using ::remainderf;
  using ::remainderl;

  using ::remquo;
  using ::remquof;
  using ::remquol;

  using ::rint;
  using ::rintf;
  using ::rintl;

  using ::round;
  using ::roundf;
  using ::roundl;

  using ::scalbln;
  using ::scalblnf;
  using ::scalblnl;

  using ::scalbn;
  using ::scalbnf;
  using ::scalbnl;

  using ::tgamma;
  using ::tgammaf;
  using ::tgammal;

  using ::trunc;
  using ::truncf;
  using ::truncl;

#endif

#if _GLIBCXX_USE_C99_MATH
#if !_GLIBCXX_USE_C99_FP_MACROS_DYNAMIC

  /// Function template definitions [8.16.3].
  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    fpclassify(_Tp __f)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL,
                             FP_SUBNORMAL, FP_ZERO, __type(__f));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    isfinite(_Tp __f)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_isfinite(__type(__f));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    isinf(_Tp __f)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_isinf(__type(__f));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    isnan(_Tp __f)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_isnan(__type(__f));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    isnormal(_Tp __f)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_isnormal(__type(__f));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    signbit(_Tp __f)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_signbit(__type(__f));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    isgreater(_Tp __f1, _Tp __f2)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_isgreater(__type(__f1), __type(__f2));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    isgreaterequal(_Tp __f1, _Tp __f2)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_isgreaterequal(__type(__f1), __type(__f2));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    isless(_Tp __f1, _Tp __f2)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_isless(__type(__f1), __type(__f2));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    islessequal(_Tp __f1, _Tp __f2)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_islessequal(__type(__f1), __type(__f2));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    islessgreater(_Tp __f1, _Tp __f2)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_islessgreater(__type(__f1), __type(__f2));
    }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_arithmetic<_Tp>::__value,
                                      int>::__type
    isunordered(_Tp __f1, _Tp __f2)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __builtin_isunordered(__type(__f1), __type(__f2));
    }

#endif
#endif

#if _GLIBCXX_USE_C99_MATH_TR1

  /** Additional overloads [8.16.4].
   *  @{
   */

  // For functions defined in C++03 the additional overloads are already
  // declared in <cmath> so we can just re-declare them in std::tr1.

  using std::acos;
  using std::asin;
  using std::atan;
  using std::atan2;
  using std::ceil;
  using std::cos;
  using std::cosh;
  using std::exp;
  using std::floor;
  using std::fmod;
  using std::frexp;
  using std::ldexp;
  using std::log;
  using std::log10;
  using std::sin;
  using std::sinh;
  using std::sqrt;
  using std::tan;
  using std::tanh;

#if __cplusplus >= 201103L

  // Since C++11, <cmath> defines additional overloads for these functions
  // in namespace std.

  using std::acosh;
  using std::asinh;
  using std::atanh;
  using std::cbrt;
  using std::copysign;
  using std::erf;
  using std::erfc;
  using std::exp2;
  using std::expm1;
  using std::fdim;
  using std::fma;
  using std::fmax;
  using std::fmin;
  using std::hypot;
  using std::ilogb;
  using std::lgamma;
  using std::llrint;
  using std::llround;
  using std::log1p;
  using std::log2;
  using std::logb;
  using std::lrint;
  using std::lround;
  using std::nan;
  using std::nearbyint;
  using std::nextafter;
  using std::nexttoward;
  using std::remainder;
  using std::remquo;
  using std::rint;
  using std::round;
  using std::scalbln;
  using std::scalbn;
  using std::tgamma;
  using std::trunc;

#else // __cplusplus < 201103L

  // In C++03 we need to provide the additional overloads.

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  acosh(float __x)
  { return __builtin_acoshf(__x); }

  inline long double
  acosh(long double __x)
  { return __builtin_acoshl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    acosh(_Tp __x)
    { return __builtin_acosh(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  asinh(float __x)
  { return __builtin_asinhf(__x); }

  inline long double
  asinh(long double __x)
  { return __builtin_asinhl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    asinh(_Tp __x)
    { return __builtin_asinh(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  atanh(float __x)
  { return __builtin_atanhf(__x); }

  inline long double
  atanh(long double __x)
  { return __builtin_atanhl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    atanh(_Tp __x)
    { return __builtin_atanh(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  cbrt(float __x)
  { return __builtin_cbrtf(__x); }

  inline long double
  cbrt(long double __x)
  { return __builtin_cbrtl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    cbrt(_Tp __x)
    { return __builtin_cbrt(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  copysign(float __x, float __y)
  { return __builtin_copysignf(__x, __y); }

  inline long double
  copysign(long double __x, long double __y)
  { return __builtin_copysignl(__x, __y); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    copysign(_Tp __x, _Up __y)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return copysign(__type(__x), __type(__y));
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  erf(float __x)
  { return __builtin_erff(__x); }

  inline long double
  erf(long double __x)
  { return __builtin_erfl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    erf(_Tp __x)
    { return __builtin_erf(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  erfc(float __x)
  { return __builtin_erfcf(__x); }

  inline long double
  erfc(long double __x)
  { return __builtin_erfcl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    erfc(_Tp __x)
    { return __builtin_erfc(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  exp2(float __x)
  { return __builtin_exp2f(__x); }

  inline long double
  exp2(long double __x)
  { return __builtin_exp2l(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    exp2(_Tp __x)
    { return __builtin_exp2(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  expm1(float __x)
  { return __builtin_expm1f(__x); }

  inline long double
  expm1(long double __x)
  { return __builtin_expm1l(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    expm1(_Tp __x)
    { return __builtin_expm1(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  fdim(float __x, float __y)
  { return __builtin_fdimf(__x, __y); }

  inline long double
  fdim(long double __x, long double __y)
  { return __builtin_fdiml(__x, __y); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    fdim(_Tp __x, _Up __y)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return fdim(__type(__x), __type(__y));
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  fma(float __x, float __y, float __z)
  { return __builtin_fmaf(__x, __y, __z); }

  inline long double
  fma(long double __x, long double __y, long double __z)
  { return __builtin_fmal(__x, __y, __z); }
#endif

  template<typename _Tp, typename _Up, typename _Vp>
    inline typename __gnu_cxx::__promote_3<_Tp, _Up, _Vp>::__type
    fma(_Tp __x, _Up __y, _Vp __z)
    {
      typedef typename __gnu_cxx::__promote_3<_Tp, _Up, _Vp>::__type __type;
      return fma(__type(__x), __type(__y), __type(__z));
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  fmax(float __x, float __y)
  { return __builtin_fmaxf(__x, __y); }

  inline long double
  fmax(long double __x, long double __y)
  { return __builtin_fmaxl(__x, __y); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    fmax(_Tp __x, _Up __y)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return fmax(__type(__x), __type(__y));
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  fmin(float __x, float __y)
  { return __builtin_fminf(__x, __y); }

  inline long double
  fmin(long double __x, long double __y)
  { return __builtin_fminl(__x, __y); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    fmin(_Tp __x, _Up __y)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return fmin(__type(__x), __type(__y));
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  hypot(float __x, float __y)
  { return __builtin_hypotf(__x, __y); }

  inline long double
  hypot(long double __x, long double __y)
  { return __builtin_hypotl(__x, __y); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    hypot(_Tp __y, _Up __x)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return hypot(__type(__y), __type(__x));
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline int
  ilogb(float __x)
  { return __builtin_ilogbf(__x); }

  inline int
  ilogb(long double __x)
  { return __builtin_ilogbl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      int>::__type
    ilogb(_Tp __x)
    { return __builtin_ilogb(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  lgamma(float __x)
  { return __builtin_lgammaf(__x); }

  inline long double
  lgamma(long double __x)
  { return __builtin_lgammal(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    lgamma(_Tp __x)
    { return __builtin_lgamma(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline long long
  llrint(float __x)
  { return __builtin_llrintf(__x); }

  inline long long
  llrint(long double __x)
  { return __builtin_llrintl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      long long>::__type
    llrint(_Tp __x)
    { return __builtin_llrint(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline long long
  llround(float __x)
  { return __builtin_llroundf(__x); }

  inline long long
  llround(long double __x)
  { return __builtin_llroundl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      long long>::__type
    llround(_Tp __x)
    { return __builtin_llround(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  log1p(float __x)
  { return __builtin_log1pf(__x); }

  inline long double
  log1p(long double __x)
  { return __builtin_log1pl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    log1p(_Tp __x)
    { return __builtin_log1p(__x); }

  // DR 568.
#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  log2(float __x)
  { return __builtin_log2f(__x); }

  inline long double
  log2(long double __x)
  { return __builtin_log2l(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    log2(_Tp __x)
    { return __builtin_log2(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  logb(float __x)
  { return __builtin_logbf(__x); }

  inline long double
  logb(long double __x)
  { return __builtin_logbl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    logb(_Tp __x)
    {
      return __builtin_logb(__x);
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline long
  lrint(float __x)
  { return __builtin_lrintf(__x); }

  inline long
  lrint(long double __x)
  { return __builtin_lrintl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      long>::__type
    lrint(_Tp __x)
    { return __builtin_lrint(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline long
  lround(float __x)
  { return __builtin_lroundf(__x); }

  inline long
  lround(long double __x)
  { return __builtin_lroundl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      long>::__type
    lround(_Tp __x)
    { return __builtin_lround(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  nearbyint(float __x)
  { return __builtin_nearbyintf(__x); }

  inline long double
  nearbyint(long double __x)
  { return __builtin_nearbyintl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    nearbyint(_Tp __x)
    { return __builtin_nearbyint(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  nextafter(float __x, float __y)
  { return __builtin_nextafterf(__x, __y); }

  inline long double
  nextafter(long double __x, long double __y)
  { return __builtin_nextafterl(__x, __y); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    nextafter(_Tp __x, _Up __y)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return nextafter(__type(__x), __type(__y));
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  nexttoward(float __x, long double __y)
  { return __builtin_nexttowardf(__x, __y); }

  inline long double
  nexttoward(long double __x, long double __y)
  { return __builtin_nexttowardl(__x, __y); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    nexttoward(_Tp __x, long double __y)
    { return __builtin_nexttoward(__x, __y); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  remainder(float __x, float __y)
  { return __builtin_remainderf(__x, __y); }

  inline long double
  remainder(long double __x, long double __y)
  { return __builtin_remainderl(__x, __y); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    remainder(_Tp __x, _Up __y)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return remainder(__type(__x), __type(__y));
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  remquo(float __x, float __y, int* __pquo)
  { return __builtin_remquof(__x, __y, __pquo); }

  inline long double
  remquo(long double __x, long double __y, int* __pquo)
  { return __builtin_remquol(__x, __y, __pquo); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    remquo(_Tp __x, _Up __y, int* __pquo)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return remquo(__type(__x), __type(__y), __pquo);
    }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  rint(float __x)
  { return __builtin_rintf(__x); }

  inline long double
  rint(long double __x)
  { return __builtin_rintl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    rint(_Tp __x)
    { return __builtin_rint(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  round(float __x)
  { return __builtin_roundf(__x); }

  inline long double
  round(long double __x)
  { return __builtin_roundl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    round(_Tp __x)
    { return __builtin_round(__x); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  scalbln(float __x, long __ex)
  { return __builtin_scalblnf(__x, __ex); }

  inline long double
  scalbln(long double __x, long __ex)
  { return __builtin_scalblnl(__x, __ex); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    scalbln(_Tp __x, long __ex)
    { return __builtin_scalbln(__x, __ex); }
 
#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  scalbn(float __x, int __ex)
  { return __builtin_scalbnf(__x, __ex); }

  inline long double
  scalbn(long double __x, int __ex)
  { return __builtin_scalbnl(__x, __ex); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    scalbn(_Tp __x, int __ex)
    { return __builtin_scalbn(__x, __ex); }

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  tgamma(float __x)
  { return __builtin_tgammaf(__x); }

  inline long double
  tgamma(long double __x)
  { return __builtin_tgammal(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    tgamma(_Tp __x)
    { return __builtin_tgamma(__x); }
 
#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  trunc(float __x)
  { return __builtin_truncf(__x); }

  inline long double
  trunc(long double __x)
  { return __builtin_truncl(__x); }
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value, 
                                      double>::__type
    trunc(_Tp __x)
    { return __builtin_trunc(__x); }

#endif // __cplusplus < 201103L

  // @}

#endif /* _GLIBCXX_USE_C99_MATH_TR1 */

  // DR 550. What should the return type of pow(float,int) be?
  // NB: C++11 and TR1 != C++03.

  // We cannot do "using std::pow;" because that would bring in unwanted
  // pow(*, int) overloads in C++03, with the wrong return type. Instead we
  // define all the necessary overloads, but the std::tr1::pow(double, double)
  // overload cannot be provided here, because <tr1/math.h> would add it to
  // the global namespace where it would clash with ::pow(double,double) from
  // libc (revealed by the fix of PR c++/54537).
  // The solution is to forward std::tr1::pow(double,double) to
  // std::pow(double,double) via the function template below. See
  // the discussion about this issue here:
  // http://gcc.gnu.org/ml/gcc-patches/2012-09/msg01278.html

#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  pow(float __x, float __y)
  { return std::pow(__x, __y); }

  inline long double
  pow(long double __x, long double __y)
  { return std::pow(__x, __y); }
#endif

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__promote_2<_Tp, _Up>::__type
    pow(_Tp __x, _Up __y)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
      return std::pow(__type(__x), __type(__y));
    }

#if __cplusplus >= 201103L
  // We also deal with fabs in a special way, because "using std::fabs;"
  // could bring in C++11's std::fabs<T>(const std::complex<T>&) with a
  // different return type from std::tr1::fabs<T>(const std::complex<T>&).
  // We define the necessary overloads, except std::tr1::fabs(double) which
  // could clash with ::fabs(double) from libc.
  // The function template handles double as well as integers, forwarding
  // to std::fabs.

#ifndef __CORRECT_ISO_CPP_MATH_H_PROTO
#ifndef __CORRECT_ISO_CPP11_MATH_H_PROTO_FP
  inline float
  fabs(float __x)
  { return __builtin_fabsf(__x); }

  inline long double
  fabs(long double __x)
  { return __builtin_fabsl(__x); }
#endif
#endif

  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    fabs(_Tp __x)
    { return std::fabs(__x); }

#else  // ! C++11

  // For C++03 just use std::fabs as there is no overload for std::complex<>.
  using std::fabs;

#endif // C++11

#if _GLIBCXX_USE_STD_SPEC_FUNCS

  /**
   * @defgroup tr1_math_spec_func Mathematical Special Functions
   * @ingroup numerics
   *
   * A collection of advanced mathematical special functions.
   * @{
   */

  using std::assoc_laguerref;
  using std::assoc_laguerrel;
  using std::assoc_laguerre;

  using std::assoc_legendref;
  using std::assoc_legendrel;
  using std::assoc_legendre;

  using std::betaf;
  using std::betal;
  using std::beta;

  using std::comp_ellint_1f;
  using std::comp_ellint_1l;
  using std::comp_ellint_1;

  using std::comp_ellint_2f;
  using std::comp_ellint_2l;
  using std::comp_ellint_2;

  using std::comp_ellint_3f;
  using std::comp_ellint_3l;
  using std::comp_ellint_3;

  using __gnu_cxx::conf_hypergf;
  using __gnu_cxx::conf_hypergl;
  using __gnu_cxx::conf_hyperg;

  using std::cyl_bessel_if;
  using std::cyl_bessel_il;
  using std::cyl_bessel_i;

  using std::cyl_bessel_jf;
  using std::cyl_bessel_jl;
  using std::cyl_bessel_j;

  using std::cyl_bessel_kf;
  using std::cyl_bessel_kl;
  using std::cyl_bessel_k;

  using std::cyl_neumannf;
  using std::cyl_neumannl;
  using std::cyl_neumann;

  using std::ellint_1f;
  using std::ellint_1l;
  using std::ellint_1;

  using std::ellint_2f;
  using std::ellint_2l;
  using std::ellint_2;

  using std::ellint_3f;
  using std::ellint_3l;
  using std::ellint_3;

  using std::expintf;
  using std::expintl;
  using std::expint;

  using std::hermitef;
  using std::hermitel;
  using std::hermite;

  using __gnu_cxx::hypergf;
  using __gnu_cxx::hypergl;
  using __gnu_cxx::hyperg;

  using std::laguerref;
  using std::laguerrel;
  using std::laguerre;

  using std::legendref;
  using std::legendrel;
  using std::legendre;

  using std::riemann_zetaf;
  using std::riemann_zetal;
  using std::riemann_zeta;

  using std::sph_besself;
  using std::sph_bessell;
  using std::sph_bessel;

  using std::sph_legendref;
  using std::sph_legendrel;
  using std::sph_legendre;

  using std::sph_neumannf;
  using std::sph_neumannl;
  using std::sph_neumann;

  /* @} */ // tr1_math_spec_func

#else // ! _GLIBCXX_USE_STD_SPEC_FUNCS

} // namespace tr1

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#include <bits/stl_algobase.h>
#include <limits>
#include <tr1/type_traits>

#include <tr1/gamma.tcc>
#include <tr1/bessel_function.tcc>
#include <tr1/beta_function.tcc>
#include <tr1/ell_integral.tcc>
#include <tr1/exp_integral.tcc>
#include <tr1/hypergeometric.tcc>
#include <tr1/legendre_function.tcc>
#include <tr1/modified_bessel_func.tcc>
#include <tr1/poly_hermite.tcc>
#include <tr1/poly_laguerre.tcc>
#include <tr1/riemann_zeta.tcc>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace tr1
{
  /**
   * @defgroup tr1_math_spec_func Mathematical Special Functions
   * @ingroup numerics
   *
   * A collection of advanced mathematical special functions.
   * @{
   */

  inline float
  assoc_laguerref(unsigned int __n, unsigned int __m, float __x)
  { return __detail::__assoc_laguerre<float>(__n, __m, __x); }

  inline long double
  assoc_laguerrel(unsigned int __n, unsigned int __m, long double __x)
  {
    return __detail::__assoc_laguerre<long double>(__n, __m, __x);
  }

  ///  5.2.1.1  Associated Laguerre polynomials.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    assoc_laguerre(unsigned int __n, unsigned int __m, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__assoc_laguerre<__type>(__n, __m, __x);
    }

  inline float
  assoc_legendref(unsigned int __l, unsigned int __m, float __x)
  { return __detail::__assoc_legendre_p<float>(__l, __m, __x); }

  inline long double
  assoc_legendrel(unsigned int __l, unsigned int __m, long double __x)
  { return __detail::__assoc_legendre_p<long double>(__l, __m, __x); }

  ///  5.2.1.2  Associated Legendre functions.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    assoc_legendre(unsigned int __l, unsigned int __m, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__assoc_legendre_p<__type>(__l, __m, __x);
    }

  inline float
  betaf(float __x, float __y)
  { return __detail::__beta<float>(__x, __y); }

  inline long double
  betal(long double __x, long double __y)
  { return __detail::__beta<long double>(__x, __y); }

  ///  5.2.1.3  Beta functions.
  template<typename _Tpx, typename _Tpy>
    inline typename __gnu_cxx::__promote_2<_Tpx, _Tpy>::__type
    beta(_Tpx __x, _Tpy __y)
    {
      typedef typename __gnu_cxx::__promote_2<_Tpx, _Tpy>::__type __type;
      return __detail::__beta<__type>(__x, __y);
    }

  inline float
  comp_ellint_1f(float __k)
  { return __detail::__comp_ellint_1<float>(__k); }

  inline long double
  comp_ellint_1l(long double __k)
  { return __detail::__comp_ellint_1<long double>(__k); }

  ///  5.2.1.4  Complete elliptic integrals of the first kind.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    comp_ellint_1(_Tp __k)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__comp_ellint_1<__type>(__k);
    }

  inline float
  comp_ellint_2f(float __k)
  { return __detail::__comp_ellint_2<float>(__k); }

  inline long double
  comp_ellint_2l(long double __k)
  { return __detail::__comp_ellint_2<long double>(__k); }

  ///  5.2.1.5  Complete elliptic integrals of the second kind.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    comp_ellint_2(_Tp __k)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__comp_ellint_2<__type>(__k);
    }

  inline float
  comp_ellint_3f(float __k, float __nu)
  { return __detail::__comp_ellint_3<float>(__k, __nu); }

  inline long double
  comp_ellint_3l(long double __k, long double __nu)
  { return __detail::__comp_ellint_3<long double>(__k, __nu); }

  ///  5.2.1.6  Complete elliptic integrals of the third kind.
  template<typename _Tp, typename _Tpn>
    inline typename __gnu_cxx::__promote_2<_Tp, _Tpn>::__type
    comp_ellint_3(_Tp __k, _Tpn __nu)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Tpn>::__type __type;
      return __detail::__comp_ellint_3<__type>(__k, __nu);
    }

  inline float
  conf_hypergf(float __a, float __c, float __x)
  { return __detail::__conf_hyperg<float>(__a, __c, __x); }

  inline long double
  conf_hypergl(long double __a, long double __c, long double __x)
  { return __detail::__conf_hyperg<long double>(__a, __c, __x); }

  ///  5.2.1.7  Confluent hypergeometric functions.
  template<typename _Tpa, typename _Tpc, typename _Tp>
    inline typename __gnu_cxx::__promote_3<_Tpa, _Tpc, _Tp>::__type
    conf_hyperg(_Tpa __a, _Tpc __c, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote_3<_Tpa, _Tpc, _Tp>::__type __type;
      return __detail::__conf_hyperg<__type>(__a, __c, __x);
    }

  inline float
  cyl_bessel_if(float __nu, float __x)
  { return __detail::__cyl_bessel_i<float>(__nu, __x); }

  inline long double
  cyl_bessel_il(long double __nu, long double __x)
  { return __detail::__cyl_bessel_i<long double>(__nu, __x); }

  ///  5.2.1.8  Regular modified cylindrical Bessel functions.
  template<typename _Tpnu, typename _Tp>
    inline typename __gnu_cxx::__promote_2<_Tpnu, _Tp>::__type
    cyl_bessel_i(_Tpnu __nu, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote_2<_Tpnu, _Tp>::__type __type;
      return __detail::__cyl_bessel_i<__type>(__nu, __x);
    }

  inline float
  cyl_bessel_jf(float __nu, float __x)
  { return __detail::__cyl_bessel_j<float>(__nu, __x); }

  inline long double
  cyl_bessel_jl(long double __nu, long double __x)
  { return __detail::__cyl_bessel_j<long double>(__nu, __x); }

  ///  5.2.1.9  Cylindrical Bessel functions (of the first kind).
  template<typename _Tpnu, typename _Tp>
    inline typename __gnu_cxx::__promote_2<_Tpnu, _Tp>::__type
    cyl_bessel_j(_Tpnu __nu, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote_2<_Tpnu, _Tp>::__type __type;
      return __detail::__cyl_bessel_j<__type>(__nu, __x);
    }

  inline float
  cyl_bessel_kf(float __nu, float __x)
  { return __detail::__cyl_bessel_k<float>(__nu, __x); }

  inline long double
  cyl_bessel_kl(long double __nu, long double __x)
  { return __detail::__cyl_bessel_k<long double>(__nu, __x); }

  ///  5.2.1.10  Irregular modified cylindrical Bessel functions.
  template<typename _Tpnu, typename _Tp>
    inline typename __gnu_cxx::__promote_2<_Tpnu, _Tp>::__type
    cyl_bessel_k(_Tpnu __nu, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote_2<_Tpnu, _Tp>::__type __type;
      return __detail::__cyl_bessel_k<__type>(__nu, __x);
    }

  inline float
  cyl_neumannf(float __nu, float __x)
  { return __detail::__cyl_neumann_n<float>(__nu, __x); }

  inline long double
  cyl_neumannl(long double __nu, long double __x)
  { return __detail::__cyl_neumann_n<long double>(__nu, __x); }

  ///  5.2.1.11  Cylindrical Neumann functions.
  template<typename _Tpnu, typename _Tp>
    inline typename __gnu_cxx::__promote_2<_Tpnu, _Tp>::__type
    cyl_neumann(_Tpnu __nu, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote_2<_Tpnu, _Tp>::__type __type;
      return __detail::__cyl_neumann_n<__type>(__nu, __x);
    }

  inline float
  ellint_1f(float __k, float __phi)
  { return __detail::__ellint_1<float>(__k, __phi); }

  inline long double
  ellint_1l(long double __k, long double __phi)
  { return __detail::__ellint_1<long double>(__k, __phi); }

  ///  5.2.1.12  Incomplete elliptic integrals of the first kind.
  template<typename _Tp, typename _Tpp>
    inline typename __gnu_cxx::__promote_2<_Tp, _Tpp>::__type
    ellint_1(_Tp __k, _Tpp __phi)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Tpp>::__type __type;
      return __detail::__ellint_1<__type>(__k, __phi);
    }

  inline float
  ellint_2f(float __k, float __phi)
  { return __detail::__ellint_2<float>(__k, __phi); }

  inline long double
  ellint_2l(long double __k, long double __phi)
  { return __detail::__ellint_2<long double>(__k, __phi); }

  ///  5.2.1.13  Incomplete elliptic integrals of the second kind.
  template<typename _Tp, typename _Tpp>
    inline typename __gnu_cxx::__promote_2<_Tp, _Tpp>::__type
    ellint_2(_Tp __k, _Tpp __phi)
    {
      typedef typename __gnu_cxx::__promote_2<_Tp, _Tpp>::__type __type;
      return __detail::__ellint_2<__type>(__k, __phi);
    }

  inline float
  ellint_3f(float __k, float __nu, float __phi)
  { return __detail::__ellint_3<float>(__k, __nu, __phi); }

  inline long double
  ellint_3l(long double __k, long double __nu, long double __phi)
  { return __detail::__ellint_3<long double>(__k, __nu, __phi); }

  ///  5.2.1.14  Incomplete elliptic integrals of the third kind.
  template<typename _Tp, typename _Tpn, typename _Tpp>
    inline typename __gnu_cxx::__promote_3<_Tp, _Tpn, _Tpp>::__type
    ellint_3(_Tp __k, _Tpn __nu, _Tpp __phi)
    {
      typedef typename __gnu_cxx::__promote_3<_Tp, _Tpn, _Tpp>::__type __type;
      return __detail::__ellint_3<__type>(__k, __nu, __phi);
    }

  inline float
  expintf(float __x)
  { return __detail::__expint<float>(__x); }

  inline long double
  expintl(long double __x)
  { return __detail::__expint<long double>(__x); }

  ///  5.2.1.15  Exponential integrals.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    expint(_Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__expint<__type>(__x);
    }

  inline float
  hermitef(unsigned int __n, float __x)
  { return __detail::__poly_hermite<float>(__n, __x); }

  inline long double
  hermitel(unsigned int __n, long double __x)
  { return __detail::__poly_hermite<long double>(__n, __x); }

  ///  5.2.1.16  Hermite polynomials.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    hermite(unsigned int __n, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__poly_hermite<__type>(__n, __x);
    }

  inline float
  hypergf(float __a, float __b, float __c, float __x)
  { return __detail::__hyperg<float>(__a, __b, __c, __x); }

  inline long double
  hypergl(long double __a, long double __b, long double __c, long double __x)
  { return __detail::__hyperg<long double>(__a, __b, __c, __x); }

  ///  5.2.1.17  Hypergeometric functions.
  template<typename _Tpa, typename _Tpb, typename _Tpc, typename _Tp>
    inline typename __gnu_cxx::__promote_4<_Tpa, _Tpb, _Tpc, _Tp>::__type
    hyperg(_Tpa __a, _Tpb __b, _Tpc __c, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote_4<_Tpa, _Tpb, _Tpc, _Tp>::__type __type;
      return __detail::__hyperg<__type>(__a, __b, __c, __x);
    }

  inline float
  laguerref(unsigned int __n, float __x)
  { return __detail::__laguerre<float>(__n, __x); }

  inline long double
  laguerrel(unsigned int __n, long double __x)
  { return __detail::__laguerre<long double>(__n, __x); }

  ///  5.2.1.18  Laguerre polynomials.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    laguerre(unsigned int __n, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__laguerre<__type>(__n, __x);
    }

  inline float
  legendref(unsigned int __n, float __x)
  { return __detail::__poly_legendre_p<float>(__n, __x); }

  inline long double
  legendrel(unsigned int __n, long double __x)
  { return __detail::__poly_legendre_p<long double>(__n, __x); }

  ///  5.2.1.19  Legendre polynomials.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    legendre(unsigned int __n, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__poly_legendre_p<__type>(__n, __x);
    }

  inline float
  riemann_zetaf(float __x)
  { return __detail::__riemann_zeta<float>(__x); }

  inline long double
  riemann_zetal(long double __x)
  { return __detail::__riemann_zeta<long double>(__x); }

  ///  5.2.1.20  Riemann zeta function.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    riemann_zeta(_Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__riemann_zeta<__type>(__x);
    }

  inline float
  sph_besself(unsigned int __n, float __x)
  { return __detail::__sph_bessel<float>(__n, __x); }

  inline long double
  sph_bessell(unsigned int __n, long double __x)
  { return __detail::__sph_bessel<long double>(__n, __x); }

  ///  5.2.1.21  Spherical Bessel functions.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    sph_bessel(unsigned int __n, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__sph_bessel<__type>(__n, __x);
    }

  inline float
  sph_legendref(unsigned int __l, unsigned int __m, float __theta)
  { return __detail::__sph_legendre<float>(__l, __m, __theta); }

  inline long double
  sph_legendrel(unsigned int __l, unsigned int __m, long double __theta)
  { return __detail::__sph_legendre<long double>(__l, __m, __theta); }

  ///  5.2.1.22  Spherical associated Legendre functions.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    sph_legendre(unsigned int __l, unsigned int __m, _Tp __theta)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__sph_legendre<__type>(__l, __m, __theta);
    }

  inline float
  sph_neumannf(unsigned int __n, float __x)
  { return __detail::__sph_neumann<float>(__n, __x); }

  inline long double
  sph_neumannl(unsigned int __n, long double __x)
  { return __detail::__sph_neumann<long double>(__n, __x); }

  ///  5.2.1.23  Spherical Neumann functions.
  template<typename _Tp>
    inline typename __gnu_cxx::__promote<_Tp>::__type
    sph_neumann(unsigned int __n, _Tp __x)
    {
      typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return __detail::__sph_neumann<__type>(__n, __x);
    }

  /* @} */ // tr1_math_spec_func
#endif // _GLIBCXX_USE_STD_SPEC_FUNCS

} // namespace tr1

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif // _GLIBCXX_TR1_CMATH
