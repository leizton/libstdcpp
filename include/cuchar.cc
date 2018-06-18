// -*- C++ -*- forwarding header.

// Copyright (C) 2015-2018 Free Software Foundation, Inc.
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

/** @file include/cuchar
 *  This is a Standard C++ Library file.  You should @c \\#include this file
 *  in your programs, rather than any of the @a *.h implementation files.
 *
 *  This is the C++ version of the Standard C Library header @c uchar.h,
 *  and its contents are (mostly) the same as that header, but are all
 *  contained in the namespace @c std (except for names which are defined
 *  as macros in C).
 */

//
// ISO C++ 14882:2011 21.8
//

#ifndef _GLIBCXX_CUCHAR
#define _GLIBCXX_CUCHAR 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#include <bits/c++config.h>
#include <cwchar>

#if _GLIBCXX_USE_C11_UCHAR_CXX11

#include <uchar.h>

// Get rid of those macros defined in <uchar.h> in lieu of real functions.
#undef mbrtoc16
#undef c16rtomb
#undef mbrtoc32
#undef c32rtomb

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  using ::mbrtoc16;
  using ::c16rtomb;
  using ::mbrtoc32;
  using ::c32rtomb;

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif // _GLIBCXX_USE_C11_UCHAR_CXX11

#endif // C++11

#endif // _GLIBCXX_CUCHAR
