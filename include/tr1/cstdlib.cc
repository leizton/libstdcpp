// TR1 cstdlib -*- C++ -*-

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

/** @file tr1/cstdlib
 *  This is a TR1 C++ Library header. 
 */

#ifndef _GLIBCXX_TR1_CSTDLIB
#define _GLIBCXX_TR1_CSTDLIB 1

#pragma GCC system_header

#include <cstdlib>

#if _GLIBCXX_HOSTED

#if _GLIBCXX_USE_C99_STDLIB

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace tr1
{
#if !_GLIBCXX_USE_C99_LONG_LONG_DYNAMIC
  // types
  using std::lldiv_t;

  // functions
  using std::llabs;
  using std::lldiv;
#endif

  using std::atoll;
  using std::strtoll;
  using std::strtoull;

  using std::strtof;
  using std::strtold;

  // overloads
  using std::abs;
#if !_GLIBCXX_USE_C99_LONG_LONG_DYNAMIC
  using std::div;
#endif
}
}

#endif // _GLIBCXX_USE_C99_STDLIB

#endif // _GLIBCXX_HOSTED

#endif // _GLIBCXX_TR1_CSTDLIB
