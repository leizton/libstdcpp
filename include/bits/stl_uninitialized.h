// Raw memory manipulators -*- C++ -*-

// Copyright (C) 2001-2019 Free Software Foundation, Inc.
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

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file bits/stl_uninitialized.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */

#ifndef _STL_UNINITIALIZED_H
#define _STL_UNINITIALIZED_H 1

#if __cplusplus > 201402L
#include <utility>
#endif

#if __cplusplus >= 201103L
#include <type_traits>
#endif

namespace std _GLIBCXX_VISIBILITY(default) {
  _GLIBCXX_BEGIN_NAMESPACE_VERSION

  template <bool _TrivialValueTypes>
  struct __uninitialized_copy {
    template <typename _InputIterator, typename _ForwardIterator>
    static _ForwardIterator
    __uninit_copy(_InputIterator first, _InputIterator last,
                  _ForwardIterator result) {
      _ForwardIterator cur = result;
      __try {
        for (; first != last; ++first, (void)++cur) {
          std::_Construct(std::__addressof(*cur), *first);
        }
        return cur;
      }
      __catch(...) {
        std::_Destroy(result, cur);
        __throw_exception_again;
      }
    }
  };

  template <>
  struct __uninitialized_copy<true> {
    template <typename _InputIterator, typename _ForwardIterator>
    static _ForwardIterator
    __uninit_copy(_InputIterator first, _InputIterator last,
                  _ForwardIterator result) { return std::copy(first, last, result); }
  };

  /**
   *  @brief Copies the range [first,last) into result.
   *  @param  first  An input iterator.
   *  @param  last   An input iterator.
   *  @param  result An output iterator.
   *  @return   result + (first - last)
   *
   *  Like copy(), but does not require an initialized output range.
  */
  template <typename _InputIterator, typename _ForwardIterator>
  inline _ForwardIterator
  uninitialized_copy(_InputIterator first, _InputIterator last,
                     _ForwardIterator result) {
    typedef typename iterator_traits<_InputIterator>::value_type _ValueType1;
    typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType2;
    // trivial types can have deleted assignment
    typedef typename iterator_traits<_InputIterator>::reference _RefType1;
    typedef typename iterator_traits<_ForwardIterator>::reference _RefType2;
    const bool __assignable = is_assignable<_RefType2, _RefType1>::value;

    return std::__uninitialized_copy<__is_trivial(_ValueType1) && __is_trivial(_ValueType2) && __assignable > ::__uninit_copy(first, last, result);
  }

  template <bool _TrivialValueType>
  struct __uninitialized_fill {
    template <typename _ForwardIterator, typename _Tp>
    static void
    __uninit_fill(_ForwardIterator first, _ForwardIterator last,
                  const _Tp& __x) {
      _ForwardIterator cur = first;
      __try {
        for (; cur != last; ++cur)
          std::_Construct(std::__addressof(*cur), __x);
      }
      __catch(...) {
        std::_Destroy(first, cur);
        __throw_exception_again;
      }
    }
  };

  template <>
  struct __uninitialized_fill<true> {
    template <typename _ForwardIterator, typename _Tp>
    static void
    __uninit_fill(_ForwardIterator first, _ForwardIterator last,
                  const _Tp& __x) { std::fill(first, last, __x); }
  };

  /**
   *  @brief Copies the value x into the range [first,last).
   *  @param  first  An input iterator.
   *  @param  last   An input iterator.
   *  @param  __x      The source value.
   *  @return   Nothing.
   *
   *  Like fill(), but does not require an initialized output range.
  */
  template <typename _ForwardIterator, typename _Tp>
  inline void
  uninitialized_fill(_ForwardIterator first, _ForwardIterator last,
                     const _Tp& __x) {
    typedef typename iterator_traits<_ForwardIterator>::value_type
        _ValueType;
#if __cplusplus < 201103L
    const bool __assignable = true;
#else
    // trivial types can have deleted assignment
    const bool __assignable = is_copy_assignable<_ValueType>::value;
#endif

    std::__uninitialized_fill<__is_trivial(_ValueType) && __assignable>::
        __uninit_fill(first, last, __x);
  }

  template <bool _TrivialValueType>
  struct __uninitialized_fill_n {
    template <typename _ForwardIterator, typename _Size, typename _Tp>
    static _ForwardIterator
    __uninit_fill_n(_ForwardIterator first, _Size __n,
                    const _Tp& __x) {
      _ForwardIterator cur = first;
      __try {
        for (; __n > 0; --__n, (void)++cur)
          std::_Construct(std::__addressof(*cur), __x);
        return cur;
      }
      __catch(...) {
        std::_Destroy(first, cur);
        __throw_exception_again;
      }
    }
  };

  template <>
  struct __uninitialized_fill_n<true> {
    template <typename _ForwardIterator, typename _Size, typename _Tp>
    static _ForwardIterator
    __uninit_fill_n(_ForwardIterator first, _Size __n,
                    const _Tp& __x) { return std::fill_n(first, __n, __x); }
  };

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 1339. uninitialized_fill_n should return the end of its range
  /**
   *  @brief Copies the value x into the range [first,first+n).
   *  @param  first  An input iterator.
   *  @param  __n      The number of copies to make.
   *  @param  __x      The source value.
   *  @return   Nothing.
   *
   *  Like fill_n(), but does not require an initialized output range.
  */
  template <typename _ForwardIterator, typename _Size, typename _Tp>
  inline _ForwardIterator
  uninitialized_fill_n(_ForwardIterator first, _Size __n, const _Tp& __x) {
    typedef typename iterator_traits<_ForwardIterator>::value_type
        _ValueType;
#if __cplusplus < 201103L
    const bool __assignable = true;
#else
    // trivial types can have deleted assignment
    const bool __assignable = is_copy_assignable<_ValueType>::value;
#endif
    return __uninitialized_fill_n < __is_trivial(_ValueType) && __assignable > ::
                                                                                   __uninit_fill_n(first, __n, __x);
  }

  // Extensions: versions of uninitialized_copy, uninitialized_fill,
  //  and uninitialized_fill_n that take an allocator parameter.
  //  We dispatch back to the standard versions when we're given the
  //  default allocator.  For nondefault allocators we do not use
  //  any of the POD optimizations.

  template <typename _InputIterator, typename _ForwardIterator, typename _Allocator>
  _ForwardIterator
  __uninitialized_copy_a(_InputIterator first, _InputIterator last,
                         _ForwardIterator result, _Allocator & __alloc) {
    _ForwardIterator cur = result;
    __try {
      typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
      for (; first != last; ++first, (void)++cur) {
        // 调用 copy_constructor
        __traits::construct(__alloc, std::__addressof(*cur), *first);
      }
      return cur;
    }
    __catch(...) {
      std::_Destroy(result, cur, __alloc);
      __throw_exception_again;
    }
  }

  template <typename _InputIterator, typename _ForwardIterator, typename _Tp>
  inline _ForwardIterator
  __uninitialized_copy_a(_InputIterator first, _InputIterator last,
                         _ForwardIterator result, allocator<_Tp>&) {
    return std::uninitialized_copy(first, last, result);
  }

  template <typename _InputIterator, typename _ForwardIterator,
            typename _Allocator>
  inline _ForwardIterator
  __uninitialized_move_a(_InputIterator first, _InputIterator last,
                         _ForwardIterator result, _Allocator & __alloc) {
    return std::__uninitialized_copy_a(_GLIBCXX_MAKE_MOVE_ITERATOR(first),
                                       _GLIBCXX_MAKE_MOVE_ITERATOR(last),
                                       result, __alloc);
  }

  template <typename _InputIterator, typename _ForwardIterator, typename _Allocator>
  inline _ForwardIterator
  __uninitialized_move_if_noexcept_a(_InputIterator first,
                                     _InputIterator last,
                                     _ForwardIterator result,
                                     _Allocator & __alloc) {
    return std::__uninitialized_copy_a(
        std::__make_move_if_noexcept_iterator(first),
        std::__make_move_if_noexcept_iterator(last),
        result, __alloc);
  }

  template <typename _ForwardIterator, typename _Tp, typename _Allocator>
  void
  __uninitialized_fill_a(_ForwardIterator first, _ForwardIterator last,
                         const _Tp& __x, _Allocator& __alloc) {
    _ForwardIterator cur = first;
    __try {
      typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
      for (; cur != last; ++cur)
        __traits::construct(__alloc, std::__addressof(*cur), __x);
    }
    __catch(...) {
      std::_Destroy(first, cur, __alloc);
      __throw_exception_again;
    }
  }

  template <typename _ForwardIterator, typename _Tp, typename _Tp2>
  inline void
  __uninitialized_fill_a(_ForwardIterator first, _ForwardIterator last,
                         const _Tp& __x, allocator<_Tp2>&) { std::uninitialized_fill(first, last, __x); }

  template <typename _ForwardIterator, typename _Size, typename _Tp,
            typename _Allocator>
  _ForwardIterator
  __uninitialized_fill_n_a(_ForwardIterator first, _Size __n,
                           const _Tp& __x, _Allocator& __alloc) {
    _ForwardIterator cur = first;
    __try {
      typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
      for (; __n > 0; --__n, (void)++cur)
        __traits::construct(__alloc, std::__addressof(*cur), __x);
      return cur;
    }
    __catch(...) {
      std::_Destroy(first, cur, __alloc);
      __throw_exception_again;
    }
  }

  template <typename _ForwardIterator, typename _Size, typename _Tp,
            typename _Tp2>
  inline _ForwardIterator
  __uninitialized_fill_n_a(_ForwardIterator first, _Size __n,
                           const _Tp& __x, allocator<_Tp2>&) { return std::uninitialized_fill_n(first, __n, __x); }

  // Extensions: __uninitialized_copy_move, __uninitialized_move_copy,
  // __uninitialized_fill_move, __uninitialized_move_fill.
  // All of these algorithms take a user-supplied allocator, which is used
  // for construction and destruction.

  // __uninitialized_copy_move
  // Copies [first1, last1) into [result, result + (last1 - first1)), and
  //  move [first2, last2) into
  //  [result, result + (last1 - first1) + (last2 - first2)).
  template <typename _InputIterator1, typename _InputIterator2,
            typename _ForwardIterator, typename _Allocator>
  inline _ForwardIterator
  __uninitialized_copy_move(_InputIterator1 first1,
                            _InputIterator1 last1,
                            _InputIterator2 first2,
                            _InputIterator2 last2,
                            _ForwardIterator result,
                            _Allocator & __alloc) {
    _ForwardIterator __mid = std::__uninitialized_copy_a(first1, last1,
                                                         result,
                                                         __alloc);
    __try {
      return std::__uninitialized_move_a(first2, last2, __mid, __alloc);
    }
    __catch(...) {
      std::_Destroy(result, __mid, __alloc);
      __throw_exception_again;
    }
  }

  // __uninitialized_move_copy
  // Moves [first1, last1) into [result, result + (last1 - first1)), and
  //  copies [first2, last2) into
  //  [result, result + (last1 - first1) + (last2 - first2)).
  template <typename _InputIterator1, typename _InputIterator2,
            typename _ForwardIterator, typename _Allocator>
  inline _ForwardIterator
  __uninitialized_move_copy(_InputIterator1 first1,
                            _InputIterator1 last1,
                            _InputIterator2 first2,
                            _InputIterator2 last2,
                            _ForwardIterator result,
                            _Allocator & __alloc) {
    _ForwardIterator __mid = std::__uninitialized_move_a(first1, last1,
                                                         result,
                                                         __alloc);
    __try {
      return std::__uninitialized_copy_a(first2, last2, __mid, __alloc);
    }
    __catch(...) {
      std::_Destroy(result, __mid, __alloc);
      __throw_exception_again;
    }
  }

  // __uninitialized_fill_move
  // Fills [result, mid) with x, and moves [first, last) into
  //  [mid, mid + (last - first)).
  template <typename _ForwardIterator, typename _Tp, typename _InputIterator,
            typename _Allocator>
  inline _ForwardIterator
  __uninitialized_fill_move(_ForwardIterator result, _ForwardIterator __mid,
                            const _Tp& __x, _InputIterator first,
                            _InputIterator last, _Allocator& __alloc) {
    std::__uninitialized_fill_a(result, __mid, __x, __alloc);
    __try {
      return std::__uninitialized_move_a(first, last, __mid, __alloc);
    }
    __catch(...) {
      std::_Destroy(result, __mid, __alloc);
      __throw_exception_again;
    }
  }

  // __uninitialized_move_fill
  // Moves [first1, last1) into [first2, first2 + (last1 - first1)), and
  //  fills [first2 + (last1 - first1), last2) with x.
  template <typename _InputIterator, typename _ForwardIterator, typename _Tp,
            typename _Allocator>
  inline void
  __uninitialized_move_fill(_InputIterator first1, _InputIterator last1,
                            _ForwardIterator first2,
                            _ForwardIterator last2, const _Tp& __x,
                            _Allocator& __alloc) {
    _ForwardIterator __mid2 = std::__uninitialized_move_a(first1, last1,
                                                          first2,
                                                          __alloc);
    __try {
      std::__uninitialized_fill_a(__mid2, last2, __x, __alloc);
    }
    __catch(...) {
      std::_Destroy(first2, __mid2, __alloc);
      __throw_exception_again;
    }
  }

#if __cplusplus >= 201103L
  // Extensions: __uninitialized_default, __uninitialized_default_n,
  // __uninitialized_default_a, __uninitialized_default_n_a.

  template <bool _TrivialValueType>
  struct __uninitialized_default_1 {
    template <typename _ForwardIterator>
    static void
    __uninit_default(_ForwardIterator first, _ForwardIterator last) {
      _ForwardIterator cur = first;
      __try {
        for (; cur != last; ++cur)
          std::_Construct(std::__addressof(*cur));
      }
      __catch(...) {
        std::_Destroy(first, cur);
        __throw_exception_again;
      }
    }
  };

  template <>
  struct __uninitialized_default_1<true> {
    template <typename _ForwardIterator>
    static void
    __uninit_default(_ForwardIterator first, _ForwardIterator last) {
      typedef typename iterator_traits<_ForwardIterator>::value_type
          _ValueType;

      std::fill(first, last, _ValueType());
    }
  };

  template <bool _TrivialValueType>
  struct __uninitialized_default_n_1 {
    template <typename _ForwardIterator, typename _Size>
    static _ForwardIterator
    __uninit_default_n(_ForwardIterator first, _Size __n) {
      _ForwardIterator cur = first;
      __try {
        for (; __n > 0; --__n, (void)++cur)
          std::_Construct(std::__addressof(*cur));
        return cur;
      }
      __catch(...) {
        std::_Destroy(first, cur);
        __throw_exception_again;
      }
    }
  };

  template <>
  struct __uninitialized_default_n_1<true> {
    template <typename _ForwardIterator, typename _Size>
    static _ForwardIterator
    __uninit_default_n(_ForwardIterator first, _Size __n) {
      typedef typename iterator_traits<_ForwardIterator>::value_type
          _ValueType;

      return std::fill_n(first, __n, _ValueType());
    }
  };

  // __uninitialized_default
  // Fills [first, last) with std::distance(first, last) default
  // constructed value_types(s).
  template <typename _ForwardIterator>
  inline void
  __uninitialized_default(_ForwardIterator first,
                          _ForwardIterator last) {
    typedef typename iterator_traits<_ForwardIterator>::value_type
        _ValueType;
    // trivial types can have deleted assignment
    const bool __assignable = is_copy_assignable<_ValueType>::value;

    std::__uninitialized_default_1<__is_trivial(_ValueType) && __assignable>::
        __uninit_default(first, last);
  }

  // __uninitialized_default_n
  // Fills [first, first + n) with n default constructed value_type(s).
  template <typename _ForwardIterator, typename _Size>
  inline _ForwardIterator
  __uninitialized_default_n(_ForwardIterator first, _Size __n) {
    typedef typename iterator_traits<_ForwardIterator>::value_type
        _ValueType;
    // trivial types can have deleted assignment
    const bool __assignable = is_copy_assignable<_ValueType>::value;

    return __uninitialized_default_n_1 < __is_trivial(_ValueType) && __assignable > ::
                                                                                        __uninit_default_n(first, __n);
  }

  // __uninitialized_default_a
  // Fills [first, last) with std::distance(first, last) default
  // constructed value_types(s), constructed with the allocator alloc.
  template <typename _ForwardIterator, typename _Allocator>
  void
  __uninitialized_default_a(_ForwardIterator first,
                            _ForwardIterator last,
                            _Allocator & __alloc) {
    _ForwardIterator cur = first;
    __try {
      typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
      for (; cur != last; ++cur)
        __traits::construct(__alloc, std::__addressof(*cur));
    }
    __catch(...) {
      std::_Destroy(first, cur, __alloc);
      __throw_exception_again;
    }
  }

  template <typename _ForwardIterator, typename _Tp>
  inline void
  __uninitialized_default_a(_ForwardIterator first,
                            _ForwardIterator last,
                            allocator<_Tp>&) { std::__uninitialized_default(first, last); }

  // __uninitialized_default_n_a
  // Fills [first, first + n) with n default constructed value_types(s),
  // constructed with the allocator alloc.
  template <typename _ForwardIterator, typename _Size, typename _Allocator>
  _ForwardIterator
  __uninitialized_default_n_a(_ForwardIterator first, _Size __n,
                              _Allocator & __alloc) {
    _ForwardIterator cur = first;
    __try {
      typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
      for (; __n > 0; --__n, (void)++cur)
        __traits::construct(__alloc, std::__addressof(*cur));
      return cur;
    }
    __catch(...) {
      std::_Destroy(first, cur, __alloc);
      __throw_exception_again;
    }
  }

  template <typename _ForwardIterator, typename _Size, typename _Tp>
  inline _ForwardIterator
  __uninitialized_default_n_a(_ForwardIterator first, _Size __n,
                              allocator<_Tp>&) { return std::__uninitialized_default_n(first, __n); }

  template <bool _TrivialValueType>
  struct __uninitialized_default_novalue_1 {
    template <typename _ForwardIterator>
    static void
    __uninit_default_novalue(_ForwardIterator first,
                             _ForwardIterator last) {
      _ForwardIterator cur = first;
      __try {
        for (; cur != last; ++cur)
          std::_Construct_novalue(std::__addressof(*cur));
      }
      __catch(...) {
        std::_Destroy(first, cur);
        __throw_exception_again;
      }
    }
  };

  template <>
  struct __uninitialized_default_novalue_1<true> {
    template <typename _ForwardIterator>
    static void
    __uninit_default_novalue(_ForwardIterator first,
                             _ForwardIterator last) {
    }
  };

  template <bool _TrivialValueType>
  struct __uninitialized_default_novalue_n_1 {
    template <typename _ForwardIterator, typename _Size>
    static _ForwardIterator
    __uninit_default_novalue_n(_ForwardIterator first, _Size __n) {
      _ForwardIterator cur = first;
      __try {
        for (; __n > 0; --__n, (void)++cur)
          std::_Construct_novalue(std::__addressof(*cur));
        return cur;
      }
      __catch(...) {
        std::_Destroy(first, cur);
        __throw_exception_again;
      }
    }
  };

  template <>
  struct __uninitialized_default_novalue_n_1<true> {
    template <typename _ForwardIterator, typename _Size>
    static _ForwardIterator
    __uninit_default_novalue_n(_ForwardIterator first, _Size __n) { return std::next(first, __n); }
  };

  // __uninitialized_default_novalue
  // Fills [first, last) with std::distance(first, last) default-initialized
  // value_types(s).
  template <typename _ForwardIterator>
  inline void
  __uninitialized_default_novalue(_ForwardIterator first,
                                  _ForwardIterator last) {
    typedef typename iterator_traits<_ForwardIterator>::value_type
        _ValueType;

    std::__uninitialized_default_novalue_1<
        is_trivially_default_constructible<_ValueType>::value>::
        __uninit_default_novalue(first, last);
  }

  // __uninitialized_default_n
  // Fills [first, first + n) with n default-initialized value_type(s).
  template <typename _ForwardIterator, typename _Size>
  inline _ForwardIterator
  __uninitialized_default_novalue_n(_ForwardIterator first, _Size __n) {
    typedef typename iterator_traits<_ForwardIterator>::value_type
        _ValueType;

    return __uninitialized_default_novalue_n_1<
        is_trivially_default_constructible<_ValueType>::value>::
        __uninit_default_novalue_n(first, __n);
  }

  template <typename _InputIterator, typename _Size,
            typename _ForwardIterator>
  _ForwardIterator
  __uninitialized_copy_n(_InputIterator first, _Size __n,
                         _ForwardIterator result, input_iterator_tag) {
    _ForwardIterator cur = result;
    __try {
      for (; __n > 0; --__n, (void)++first, ++cur)
        std::_Construct(std::__addressof(*cur), *first);
      return cur;
    }
    __catch(...) {
      std::_Destroy(result, cur);
      __throw_exception_again;
    }
  }

  template <typename _RandomAccessIterator, typename _Size,
            typename _ForwardIterator>
  inline _ForwardIterator
  __uninitialized_copy_n(_RandomAccessIterator first, _Size __n,
                         _ForwardIterator result,
                         random_access_iterator_tag) { return std::uninitialized_copy(first, first + __n, result); }

  template <typename _InputIterator, typename _Size,
            typename _ForwardIterator>
  pair<_InputIterator, _ForwardIterator>
  __uninitialized_copy_n_pair(_InputIterator first, _Size __n,
                              _ForwardIterator result, input_iterator_tag) {
    _ForwardIterator cur = result;
    __try {
      for (; __n > 0; --__n, (void)++first, ++cur)
        std::_Construct(std::__addressof(*cur), *first);
      return {first, cur};
    }
    __catch(...) {
      std::_Destroy(result, cur);
      __throw_exception_again;
    }
  }

  template <typename _RandomAccessIterator, typename _Size,
            typename _ForwardIterator>
  inline pair<_RandomAccessIterator, _ForwardIterator>
  __uninitialized_copy_n_pair(_RandomAccessIterator first, _Size __n,
                              _ForwardIterator result,
                              random_access_iterator_tag) {
    auto __second_res = uninitialized_copy(first, first + __n, result);
    auto first_res = std::next(first, __n);
    return {first_res, __second_res};
  }

  /**
   *  @brief Copies the range [first,first+n) into result.
   *  @param  first  An input iterator.
   *  @param  __n      The number of elements to copy.
   *  @param  result An output iterator.
   *  @return  result + __n
   *
   *  Like copy_n(), but does not require an initialized output range.
  */
  template <typename _InputIterator, typename _Size, typename _ForwardIterator>
  inline _ForwardIterator
  uninitialized_copy_n(_InputIterator first, _Size __n,
                       _ForwardIterator result) { return std::__uninitialized_copy_n(first, __n, result,
                                                                                       std::__iterator_category(first)); }

  template <typename _InputIterator, typename _Size, typename _ForwardIterator>
  inline pair<_InputIterator, _ForwardIterator>
  __uninitialized_copy_n_pair(_InputIterator first, _Size __n,
                              _ForwardIterator result) {
    return std::__uninitialized_copy_n_pair(first, __n, result,
                                            std::__iterator_category(first));
  }

#endif

#if __cplusplus >= 201703L
#define __cpp_lib_raw_memory_algorithms 201606L

  template <typename _ForwardIterator>
  inline void
  uninitialized_default_construct(_ForwardIterator first,
                                  _ForwardIterator last) {
    __uninitialized_default_novalue(first, last);
  }

  template <typename _ForwardIterator, typename _Size>
  inline _ForwardIterator
  uninitialized_default_construct_n(_ForwardIterator first, _Size count) {
    return __uninitialized_default_novalue_n(first, count);
  }

  template <typename _ForwardIterator>
  inline void
  uninitialized_value_construct(_ForwardIterator first,
                                _ForwardIterator last) {
    return __uninitialized_default(first, last);
  }

  template <typename _ForwardIterator, typename _Size>
  inline _ForwardIterator
  uninitialized_value_construct_n(_ForwardIterator first, _Size count) {
    return __uninitialized_default_n(first, count);
  }

  template <typename _InputIterator, typename _ForwardIterator>
  inline _ForwardIterator
  uninitialized_move(_InputIterator first, _InputIterator last,
                     _ForwardIterator result) {
    return std::uninitialized_copy(_GLIBCXX_MAKE_MOVE_ITERATOR(first),
                                   _GLIBCXX_MAKE_MOVE_ITERATOR(last), result);
  }

  template <typename _InputIterator, typename _Size, typename _ForwardIterator>
  inline pair<_InputIterator, _ForwardIterator>
  uninitialized_move_n(_InputIterator first, _Size count,
                       _ForwardIterator result) {
    auto __res = std::__uninitialized_copy_n_pair(_GLIBCXX_MAKE_MOVE_ITERATOR(first),
                                                  count, result);
    return {__res.first.base(), __res.second};
  }
#endif // C++17

  template <typename _Tp, typename _Up, typename _Allocator>
  inline void
  __relocate_object_a(_Tp * dst, _Up * ori, _Allocator & __alloc)
      noexcept(noexcept(std::allocator_traits<_Allocator>::construct(__alloc, dst, std::move(*ori)))
               && noexcept(std::allocator_traits<_Allocator>::destroy(__alloc, std::__addressof(*ori)))) {
    typedef std::allocator_traits<_Allocator> __traits;
    __traits::construct(__alloc, dst, std::move(*ori));
    __traits::destroy(__alloc, std::__addressof(*ori));
  }

  // This class may be specialized for specific types.
  // Also known as is_trivially_relocatable.
  template <typename _Tp, typename = void>
  struct __is_bitwise_relocatable : is_trivial<_Tp> {};

  template <typename _Tp, typename _Up>
  inline __enable_if_t<std::__is_bitwise_relocatable<_Tp>::value, _Tp*>
  __relocate_a_1(_Tp * first, _Tp * last,
                 _Tp * result, allocator<_Up>&) noexcept {
    ptrdiff_t count = last - first;
    if (count > 0)
      __builtin_memmove(result, first, count * sizeof(_Tp));
    return result + count;
  }

  template <typename _InputIterator, typename _ForwardIterator,
            typename _Allocator>
  inline _ForwardIterator
  __relocate_a_1(_InputIterator first, _InputIterator last,
                 _ForwardIterator result, _Allocator & __alloc) {
    typedef typename iterator_traits<_InputIterator>::value_type _ValueType;
    typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType2;
    static_assert(std::is_same<_ValueType, _ValueType2>::value, "relocation is only possible for values of the same type");

    _ForwardIterator cur = result;
    for (; first != last; ++first, (void)++cur)
      std::__relocate_object_a(std::__addressof(*cur),
                               std::__addressof(*first), __alloc);
    return cur;
  }

  template <typename _InputIterator, typename _ForwardIterator, typename _Allocator>
  inline _ForwardIterator
  __relocate_a(_InputIterator first, _InputIterator last,
               _ForwardIterator result, _Allocator & __alloc) {
    return __relocate_a_1(std::__niter_base(first),
                          std::__niter_base(last),
                          std::__niter_base(result), __alloc);
  }

  _GLIBCXX_END_NAMESPACE_VERSION
} // namespace )

#endif /* _STL_UNINITIALIZED_H */
