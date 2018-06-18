// <experimental/unordered_map> -*- C++ -*-

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

/** @file experimental/unordered_map
 *  This is a TS C++ Library header.
 */

#ifndef _GLIBCXX_EXPERIMENTAL_UNORDERED_MAP
#define _GLIBCXX_EXPERIMENTAL_UNORDERED_MAP 1

#pragma GCC system_header

#if __cplusplus >= 201402L

#include <unordered_map>
#include <experimental/bits/erase_if.h>
#include <experimental/memory_resource>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace experimental
{
inline namespace fundamentals_v2
{
  template<typename _Key, typename _Tp, typename _Hash, typename _CPred,
      typename _Alloc, typename _Predicate>
    inline void
    erase_if(unordered_map<_Key, _Tp, _Hash, _CPred, _Alloc>& __cont,
        _Predicate __pred)
    { __detail::__erase_nodes_if(__cont, __pred); }

  template<typename _Key, typename _Tp, typename _Hash, typename _CPred,
      typename _Alloc, typename _Predicate>
    inline void
    erase_if(unordered_multimap<_Key, _Tp, _Hash, _CPred, _Alloc>& __cont,
        _Predicate __pred)
    { __detail::__erase_nodes_if(__cont, __pred); }

  namespace pmr {
    template<typename _Key, typename _Tp, typename _Hash = hash<_Key>,
        typename _Pred = equal_to<_Key>>
      using unordered_map
      = std::unordered_map<_Key, _Tp, _Hash, _Pred,
            polymorphic_allocator<pair<const _Key, _Tp>>>;

    template<typename _Key, typename _Tp, typename _Hash = hash<_Key>,
        typename _Pred = equal_to<_Key>>
      using unordered_multimap
      = std::unordered_multimap<_Key, _Tp, _Hash, _Pred,
                 polymorphic_allocator<pair<const _Key, _Tp>>>;
  } // namespace pmr

} // namespace fundamentals_v2
} // namespace experimental

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif // C++14

#endif // _GLIBCXX_EXPERIMENTAL_UNORDERED_MAP
