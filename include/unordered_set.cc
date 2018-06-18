// <unordered_set> -*- C++ -*-

// Copyright (C) 2007-2018 Free Software Foundation, Inc.
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

/** @file include/unordered_set
 *  This is a Standard C++ Library header.
 */

#ifndef _GLIBCXX_UNORDERED_SET
#define _GLIBCXX_UNORDERED_SET 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#include <type_traits>
#include <initializer_list>
#include <bits/allocator.h>
#include <ext/alloc_traits.h>
#include <ext/aligned_buffer.h>
#include <bits/stl_pair.h>
#include <bits/stl_function.h> // equal_to, _Identity, _Select1st
#include <bits/functional_hash.h>
#include <bits/hashtable.h>
#include <bits/unordered_set.h>
#include <bits/range_access.h>

#ifdef _GLIBCXX_DEBUG
# include <debug/unordered_set>
#endif

#ifdef _GLIBCXX_PROFILE
# include <profile/unordered_set>
#endif
#endif // C++11

#endif // _GLIBCXX_UNORDERED_SET
