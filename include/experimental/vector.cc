// <experimental/vector> -*- C++ -*-

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

/** @file experimental/vector
 *  This is a TS C++ Library header.
 */

#ifndef _GLIBCXX_EXPERIMENTAL_VECTOR
#define _GLIBCXX_EXPERIMENTAL_VECTOR 1

#pragma GCC system_header

#if __cplusplus >= 201402L

#include <vector>
#include <algorithm>
#include <experimental/memory_resource>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace experimental
{
inline namespace fundamentals_v2
{
#define __cpp_lib_experimental_erase_if 201411

  template<typename _Tp, typename _Alloc, typename _Predicate>
    inline void
    erase_if(vector<_Tp, _Alloc>& __cont, _Predicate __pred)
    {
      __cont.erase(std::remove_if(__cont.begin(), __cont.end(), __pred),
              __cont.end());
    }

  template<typename _Tp, typename _Alloc, typename _Up>
    inline void
    erase(vector<_Tp, _Alloc>& __cont, const _Up& __value)
    {
      __cont.erase(std::remove(__cont.begin(), __cont.end(), __value),
              __cont.end());
    }

  namespace pmr {
    template<typename _Tp>
      using vector = std::vector<_Tp, polymorphic_allocator<_Tp>>;
  } // namespace pmr

} // namespace fundamentals_v2
} // namespace experimental

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif // C++14

#endif // _GLIBCXX_EXPERIMENTAL_VECTOR
