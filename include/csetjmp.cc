// -*- C++ -*- forwarding header.

// Copyright (C) 1997-2018 Free Software Foundation, Inc.
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

/** @file csetjmp
 *  This is a Standard C++ Library file.  You should @c \\#include this file
 *  in your programs, rather than any of the @a *.h implementation files.
 *
 *  This is the C++ version of the Standard C Library header @c setjmp.h,
 *  and its contents are (mostly) the same as that header, but are all
 *  contained in the namespace @c std (except for names which are defined
 *  as macros in C).
 */

//
// ISO C++ 14882: 20.4.6  C library
//

#pragma GCC system_header

#include <bits/c++config.h>
#include <setjmp.h>

#ifndef _GLIBCXX_CSETJMP
#define _GLIBCXX_CSETJMP 1

// Get rid of those macros defined in <setjmp.h> in lieu of real functions.
#undef longjmp

// Adhere to section 17.4.1.2 clause 5 of ISO 14882:1998
#ifndef setjmp
#define setjmp(env) setjmp (env)
#endif

namespace std
{
  using ::jmp_buf;
  using ::longjmp;
} // namespace std

#endif
