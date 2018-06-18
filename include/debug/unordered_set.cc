// Debugging unordered_set/unordered_multiset implementation -*- C++ -*-

// Copyright (C) 2003-2018 Free Software Foundation, Inc.
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

/** @file debug/unordered_set
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_UNORDERED_SET
#define _GLIBCXX_DEBUG_UNORDERED_SET 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else
# include <unordered_set>

#include <debug/safe_unordered_container.h>
#include <debug/safe_container.h>
#include <debug/safe_iterator.h>
#include <debug/safe_local_iterator.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __debug
{
  /// Class std::unordered_set with safety/checking/debug instrumentation.
  template<typename _Value,
      typename _Hash = std::hash<_Value>,
      typename _Pred = std::equal_to<_Value>,
      typename _Alloc = std::allocator<_Value> >
    class unordered_set
    : public __gnu_debug::_Safe_container<
   unordered_set<_Value, _Hash, _Pred, _Alloc>, _Alloc,
   __gnu_debug::_Safe_unordered_container>,
      public _GLIBCXX_STD_C::unordered_set<_Value, _Hash, _Pred, _Alloc>
    {
      typedef _GLIBCXX_STD_C::unordered_set<
   _Value, _Hash, _Pred, _Alloc>                                   _Base;
      typedef __gnu_debug::_Safe_container<
   unordered_set, _Alloc, __gnu_debug::_Safe_unordered_container>  _Safe;

      typedef typename _Base::const_iterator          _Base_const_iterator;
      typedef typename _Base::iterator                _Base_iterator;
      typedef typename _Base::const_local_iterator _Base_const_local_iterator;
      typedef typename _Base::local_iterator          _Base_local_iterator;

    public:
      typedef typename _Base::size_type                    size_type;
      typedef typename _Base::hasher                       hasher;
      typedef typename _Base::key_equal                    key_equal;
      typedef typename _Base::allocator_type               allocator_type;

      typedef typename _Base::key_type                     key_type;
      typedef typename _Base::value_type           value_type;

      typedef __gnu_debug::_Safe_iterator<
   _Base_iterator, unordered_set>                  iterator;
      typedef __gnu_debug::_Safe_iterator<
   _Base_const_iterator, unordered_set>            const_iterator;
      typedef __gnu_debug::_Safe_local_iterator<
   _Base_local_iterator, unordered_set>            local_iterator;
      typedef __gnu_debug::_Safe_local_iterator<
   _Base_const_local_iterator, unordered_set>      const_local_iterator;

      unordered_set() = default;

      explicit
      unordered_set(size_type __n,
               const hasher& __hf = hasher(),
               const key_equal& __eql = key_equal(),
               const allocator_type& __a = allocator_type())
      : _Base(__n, __hf, __eql, __a) { }

      template<typename _InputIterator>
   unordered_set(_InputIterator __first, _InputIterator __last,
                 size_type __n = 0,
                 const hasher& __hf = hasher(),
                 const key_equal& __eql = key_equal(),
                 const allocator_type& __a = allocator_type())
   : _Base(__gnu_debug::__base(__gnu_debug::__check_valid_range(__first,
                                                                __last)),
           __gnu_debug::__base(__last), __n,
           __hf, __eql, __a) { }

      unordered_set(const unordered_set&) = default;

      unordered_set(const _Base& __x)
      : _Base(__x) { }

      unordered_set(unordered_set&&) = default;

      explicit
      unordered_set(const allocator_type& __a)
      : _Base(__a) { }

      unordered_set(const unordered_set& __uset,
               const allocator_type& __a)
      : _Base(__uset, __a) { }

      unordered_set(unordered_set&& __uset,
               const allocator_type& __a)
      : _Safe(std::move(__uset._M_safe()), __a),
   _Base(std::move(__uset._M_base()), __a) { }

      unordered_set(initializer_list<value_type> __l,
               size_type __n = 0,
               const hasher& __hf = hasher(),
               const key_equal& __eql = key_equal(),
               const allocator_type& __a = allocator_type())
      : _Base(__l, __n, __hf, __eql, __a) { }

      unordered_set(size_type __n, const allocator_type& __a)
   : unordered_set(__n, hasher(), key_equal(), __a)
      { }

      unordered_set(size_type __n, const hasher& __hf,
               const allocator_type& __a)
   : unordered_set(__n, __hf, key_equal(), __a)
      { }

      template<typename _InputIterator>
   unordered_set(_InputIterator __first, _InputIterator __last,
                 size_type __n,
                 const allocator_type& __a)
     : unordered_set(__first, __last, __n, hasher(), key_equal(), __a)
   { }

      template<typename _InputIterator>
   unordered_set(_InputIterator __first, _InputIterator __last,
                 size_type __n, const hasher& __hf,
                 const allocator_type& __a)
     : unordered_set(__first, __last, __n, __hf, key_equal(), __a)
   { }

      unordered_set(initializer_list<value_type> __l,
               size_type __n,
               const allocator_type& __a)
   : unordered_set(__l, __n, hasher(), key_equal(), __a)
      { }

      unordered_set(initializer_list<value_type> __l,
               size_type __n, const hasher& __hf,
               const allocator_type& __a)
   : unordered_set(__l, __n, __hf, key_equal(), __a)
      { }

      ~unordered_set() = default;

      unordered_set&
      operator=(const unordered_set&) = default;

      unordered_set&
      operator=(unordered_set&&) = default;

      unordered_set&
      operator=(initializer_list<value_type> __l)
      {
   _M_base() = __l;
   this->_M_invalidate_all();
   return *this;
      }

      void
      swap(unordered_set& __x)
   noexcept( noexcept(declval<_Base&>().swap(__x)) )
      {
   _Safe::_M_swap(__x);
   _Base::swap(__x);
      }

      void
      clear() noexcept
      {
   _Base::clear();
   this->_M_invalidate_all();
      }

      iterator
      begin() noexcept
      { return iterator(_Base::begin(), this); }

      const_iterator
      begin() const noexcept
      { return const_iterator(_Base::begin(), this); }

      iterator
      end() noexcept
      { return iterator(_Base::end(), this); }

      const_iterator
      end() const noexcept
      { return const_iterator(_Base::end(), this); }

      const_iterator
      cbegin() const noexcept
      { return const_iterator(_Base::begin(), this); }

      const_iterator
      cend() const noexcept
      { return const_iterator(_Base::end(), this); }

      // local versions
      local_iterator
      begin(size_type __b)
      {
   __glibcxx_check_bucket_index(__b);
   return local_iterator(_Base::begin(__b), this);
      }

      local_iterator
      end(size_type __b)
      {
   __glibcxx_check_bucket_index(__b);
   return local_iterator(_Base::end(__b), this);
      }

      const_local_iterator
      begin(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return const_local_iterator(_Base::begin(__b), this);
      }

      const_local_iterator
      end(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return const_local_iterator(_Base::end(__b), this);
      }

      const_local_iterator
      cbegin(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return const_local_iterator(_Base::cbegin(__b), this);
      }

      const_local_iterator
      cend(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return const_local_iterator(_Base::cend(__b), this);
      }

      size_type
      bucket_size(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return _Base::bucket_size(__b);
      }

      float
      max_load_factor() const noexcept
      { return _Base::max_load_factor(); }

      void
      max_load_factor(float __f)
      {
   __glibcxx_check_max_load_factor(__f);
   _Base::max_load_factor(__f);
      }

      template<typename... _Args>
   std::pair<iterator, bool>
   emplace(_Args&&... __args)
   {
     size_type __bucket_count = this->bucket_count();
     std::pair<_Base_iterator, bool> __res
       = _Base::emplace(std::forward<_Args>(__args)...);
     _M_check_rehashed(__bucket_count);
     return std::make_pair(iterator(__res.first, this), __res.second);
   }

      template<typename... _Args>
   iterator
   emplace_hint(const_iterator __hint, _Args&&... __args)
   {
     __glibcxx_check_insert(__hint);
     size_type __bucket_count = this->bucket_count();
     _Base_iterator __it = _Base::emplace_hint(__hint.base(),
                                   std::forward<_Args>(__args)...);
     _M_check_rehashed(__bucket_count);
     return iterator(__it, this);
   }

      std::pair<iterator, bool>
      insert(const value_type& __obj)
      {
   size_type __bucket_count = this->bucket_count();
   std::pair<_Base_iterator, bool> __res
     = _Base::insert(__obj);
   _M_check_rehashed(__bucket_count);
   return std::make_pair(iterator(__res.first, this), __res.second);
      }

      iterator
      insert(const_iterator __hint, const value_type& __obj)
      {
   __glibcxx_check_insert(__hint);
   size_type __bucket_count = this->bucket_count();
   _Base_iterator __it = _Base::insert(__hint.base(), __obj);
   _M_check_rehashed(__bucket_count);
   return iterator(__it, this);
      }

      std::pair<iterator, bool>
      insert(value_type&& __obj)
      {
   size_type __bucket_count = this->bucket_count();
   std::pair<_Base_iterator, bool> __res
     = _Base::insert(std::move(__obj));
   _M_check_rehashed(__bucket_count);
   return std::make_pair(iterator(__res.first, this), __res.second);
      }

      iterator
      insert(const_iterator __hint, value_type&& __obj)
      {
   __glibcxx_check_insert(__hint);
   size_type __bucket_count = this->bucket_count();
   _Base_iterator __it = _Base::insert(__hint.base(), std::move(__obj));
   _M_check_rehashed(__bucket_count);
   return iterator(__it, this);
      }

      void
      insert(std::initializer_list<value_type> __l)
      {
   size_type __bucket_count = this->bucket_count();
   _Base::insert(__l);
   _M_check_rehashed(__bucket_count);
      }

      template<typename _InputIterator>
   void
   insert(_InputIterator __first, _InputIterator __last)
   {
     typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
     __glibcxx_check_valid_range2(__first, __last, __dist);
     size_type __bucket_count = this->bucket_count();

     if (__dist.second >= __gnu_debug::__dp_sign)
       _Base::insert(__gnu_debug::__unsafe(__first),
                     __gnu_debug::__unsafe(__last));
     else
       _Base::insert(__first, __last);

     _M_check_rehashed(__bucket_count);
   }

#if __cplusplus > 201402L
      using node_type = typename _Base::node_type;
      using insert_return_type = _Node_insert_return<iterator, node_type>;

      node_type
      extract(const_iterator __position)
      {
   __glibcxx_check_erase(__position);
   _Base_const_iterator __victim = __position.base();
   this->_M_invalidate_if(
       [__victim](_Base_const_iterator __it) { return __it == __victim; }
       );
   this->_M_invalidate_local_if(
       [__victim](_Base_const_local_iterator __it) {
           return __it._M_curr() == __victim._M_cur;
       });
   return _Base::extract(__position.base());
      }

      node_type
      extract(const key_type& __key)
      {
   const auto __position = find(__key);
   if (__position != end())
     return extract(__position);
   return {};
      }

      insert_return_type
      insert(node_type&& __nh)
      {
   auto __ret = _Base::insert(std::move(__nh));
   iterator __pos = iterator(__ret.position, this);
   return { __pos, __ret.inserted, std::move(__ret.node) };
      }

      iterator
      insert(const_iterator __hint, node_type&& __nh)
      {
   __glibcxx_check_insert(__hint);
   return iterator(_Base::insert(__hint.base(), std::move(__nh)), this);
      }

      using _Base::merge;
#endif // C++17

      iterator
      find(const key_type& __key)
      { return iterator(_Base::find(__key), this); }

      const_iterator
      find(const key_type& __key) const
      { return const_iterator(_Base::find(__key), this); }

      std::pair<iterator, iterator>
      equal_range(const key_type& __key)
      {
   std::pair<_Base_iterator, _Base_iterator> __res
     = _Base::equal_range(__key);
   return std::make_pair(iterator(__res.first, this),
                         iterator(__res.second, this));
      }

      std::pair<const_iterator, const_iterator>
      equal_range(const key_type& __key) const
      {
   std::pair<_Base_const_iterator, _Base_const_iterator>
     __res = _Base::equal_range(__key);
   return std::make_pair(const_iterator(__res.first, this),
                         const_iterator(__res.second, this));
      }

      size_type
      erase(const key_type& __key)
      {
   size_type __ret(0);
   _Base_iterator __victim(_Base::find(__key));
   if (__victim != _Base::end())
     {
       this->_M_invalidate_if(
                       [__victim](_Base_const_iterator __it)
                       { return __it == __victim; });
       this->_M_invalidate_local_if(
                       [__victim](_Base_const_local_iterator __it)
                       { return __it._M_curr() == __victim._M_cur; });
       size_type __bucket_count = this->bucket_count();
       _Base::erase(__victim);
       _M_check_rehashed(__bucket_count);
       __ret = 1;
     }
   return __ret;
      }

      iterator
      erase(const_iterator __it)
      {
   __glibcxx_check_erase(__it);
   _Base_const_iterator __victim = __it.base();
   this->_M_invalidate_if(
                   [__victim](_Base_const_iterator __it)
                   { return __it == __victim; });
   this->_M_invalidate_local_if(
                   [__victim](_Base_const_local_iterator __it)
                   { return __it._M_curr() == __victim._M_cur; });
   size_type __bucket_count = this->bucket_count();
   _Base_iterator __next = _Base::erase(__it.base());
   _M_check_rehashed(__bucket_count);
   return iterator(__next, this);
      }

      iterator
      erase(iterator __it)
      { return erase(const_iterator(__it)); }

      iterator
      erase(const_iterator __first, const_iterator __last)
      {
   __glibcxx_check_erase_range(__first, __last);
   for (_Base_const_iterator __tmp = __first.base();
        __tmp != __last.base(); ++__tmp)
     {
       _GLIBCXX_DEBUG_VERIFY(__tmp != _Base::end(),
                             _M_message(__gnu_debug::__msg_valid_range)
                             ._M_iterator(__first, "first")
                             ._M_iterator(__last, "last"));
       this->_M_invalidate_if(
                       [__tmp](_Base_const_iterator __it)
                       { return __it == __tmp; });
       this->_M_invalidate_local_if(
                       [__tmp](_Base_const_local_iterator __it)
                       { return __it._M_curr() == __tmp._M_cur; });
     }
   size_type __bucket_count = this->bucket_count();
   _Base_iterator __next = _Base::erase(__first.base(),
                                        __last.base());
   _M_check_rehashed(__bucket_count);
   return iterator(__next, this);
      }

      _Base&
      _M_base() noexcept { return *this; }

      const _Base&
      _M_base() const noexcept { return *this; }

    private:
      void
      _M_check_rehashed(size_type __prev_count)
      {
   if (__prev_count != this->bucket_count())
     this->_M_invalidate_locals();
      }
    };

#if __cpp_deduction_guides >= 201606

  template<typename _InputIterator,
      typename _Hash =
      hash<typename iterator_traits<_InputIterator>::value_type>,
      typename _Pred =
      equal_to<typename iterator_traits<_InputIterator>::value_type>,
      typename _Allocator =
      allocator<typename iterator_traits<_InputIterator>::value_type>,
      typename = _RequireInputIter<_InputIterator>,
      typename = _RequireAllocator<_Allocator>>
    unordered_set(_InputIterator, _InputIterator,
             unordered_set<int>::size_type = {},
             _Hash = _Hash(), _Pred = _Pred(), _Allocator = _Allocator())
    -> unordered_set<typename iterator_traits<_InputIterator>::value_type,
                _Hash, _Pred, _Allocator>;

  template<typename _Tp, typename _Hash = hash<_Tp>,
      typename _Pred = equal_to<_Tp>,
      typename _Allocator = allocator<_Tp>,
      typename = _RequireAllocator<_Allocator>>
    unordered_set(initializer_list<_Tp>,
             unordered_set<int>::size_type = {},
             _Hash = _Hash(), _Pred = _Pred(), _Allocator = _Allocator())
    -> unordered_set<_Tp, _Hash, _Pred, _Allocator>;

  template<typename _InputIterator, typename _Allocator,
      typename = _RequireInputIter<_InputIterator>,
      typename = _RequireAllocator<_Allocator>>
    unordered_set(_InputIterator, _InputIterator,
             unordered_set<int>::size_type, _Allocator)
    -> unordered_set<typename iterator_traits<_InputIterator>::value_type,
                hash<
                  typename iterator_traits<_InputIterator>::value_type>,
                equal_to<
                  typename iterator_traits<_InputIterator>::value_type>,
                _Allocator>;

  template<typename _InputIterator, typename _Hash, typename _Allocator,
      typename = _RequireInputIter<_InputIterator>,
      typename = _RequireAllocator<_Allocator>>
    unordered_set(_InputIterator, _InputIterator,
             unordered_set<int>::size_type,
             _Hash, _Allocator)
    -> unordered_set<typename iterator_traits<_InputIterator>::value_type,
                _Hash,
                equal_to<
                  typename iterator_traits<_InputIterator>::value_type>,
                _Allocator>;

  template<typename _Tp, typename _Allocator,
      typename = _RequireAllocator<_Allocator>>
    unordered_set(initializer_list<_Tp>,
             unordered_set<int>::size_type, _Allocator)
    -> unordered_set<_Tp, hash<_Tp>, equal_to<_Tp>, _Allocator>;

  template<typename _Tp, typename _Hash, typename _Allocator,
      typename = _RequireAllocator<_Allocator>>
    unordered_set(initializer_list<_Tp>,
             unordered_set<int>::size_type, _Hash, _Allocator)
    -> unordered_set<_Tp, _Hash, equal_to<_Tp>, _Allocator>;

#endif

  template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
    inline void
    swap(unordered_set<_Value, _Hash, _Pred, _Alloc>& __x,
    unordered_set<_Value, _Hash, _Pred, _Alloc>& __y)
    noexcept(noexcept(__x.swap(__y)))
    { __x.swap(__y); }

  template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
    inline bool
    operator==(const unordered_set<_Value, _Hash, _Pred, _Alloc>& __x,
          const unordered_set<_Value, _Hash, _Pred, _Alloc>& __y)
    { return __x._M_base() == __y._M_base(); }

  template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
    inline bool
    operator!=(const unordered_set<_Value, _Hash, _Pred, _Alloc>& __x,
          const unordered_set<_Value, _Hash, _Pred, _Alloc>& __y)
    { return !(__x == __y); }


  /// Class std::unordered_multiset with safety/checking/debug instrumentation.
  template<typename _Value,
      typename _Hash = std::hash<_Value>,
      typename _Pred = std::equal_to<_Value>,
      typename _Alloc = std::allocator<_Value> >
    class unordered_multiset
    : public __gnu_debug::_Safe_container<
   unordered_multiset<_Value, _Hash, _Pred, _Alloc>, _Alloc,
   __gnu_debug::_Safe_unordered_container>,
      public _GLIBCXX_STD_C::unordered_multiset<_Value, _Hash, _Pred, _Alloc>
    {
      typedef _GLIBCXX_STD_C::unordered_multiset<
   _Value, _Hash, _Pred, _Alloc>                           _Base;
      typedef __gnu_debug::_Safe_container<unordered_multiset,
   _Alloc, __gnu_debug::_Safe_unordered_container>         _Safe;
      typedef typename _Base::const_iterator       _Base_const_iterator;
      typedef typename _Base::iterator             _Base_iterator;
      typedef typename _Base::const_local_iterator
                                           _Base_const_local_iterator;
      typedef typename _Base::local_iterator       _Base_local_iterator;

    public:
      typedef typename _Base::size_type                    size_type;
      typedef typename _Base::hasher                       hasher;
      typedef typename _Base::key_equal                    key_equal;
      typedef typename _Base::allocator_type               allocator_type;

      typedef typename _Base::key_type                     key_type;
      typedef typename _Base::value_type           value_type;

      typedef __gnu_debug::_Safe_iterator<
   _Base_iterator, unordered_multiset>             iterator;
      typedef __gnu_debug::_Safe_iterator<
   _Base_const_iterator, unordered_multiset>       const_iterator;
      typedef __gnu_debug::_Safe_local_iterator<
   _Base_local_iterator, unordered_multiset>       local_iterator;
      typedef __gnu_debug::_Safe_local_iterator<
   _Base_const_local_iterator, unordered_multiset> const_local_iterator;

      unordered_multiset() = default;

      explicit
      unordered_multiset(size_type __n,
                    const hasher& __hf = hasher(),
                    const key_equal& __eql = key_equal(),
                    const allocator_type& __a = allocator_type())
      : _Base(__n, __hf, __eql, __a) { }

      template<typename _InputIterator>
   unordered_multiset(_InputIterator __first, _InputIterator __last,
                      size_type __n = 0,
                      const hasher& __hf = hasher(),
                      const key_equal& __eql = key_equal(),
                      const allocator_type& __a = allocator_type())
   : _Base(__gnu_debug::__base(__gnu_debug::__check_valid_range(__first,
                                                                __last)),
           __gnu_debug::__base(__last), __n,
           __hf, __eql, __a) { }

      unordered_multiset(const unordered_multiset&) = default;

      unordered_multiset(const _Base& __x)
      : _Base(__x) { }

      unordered_multiset(unordered_multiset&&) = default;

      explicit
      unordered_multiset(const allocator_type& __a)
      : _Base(__a) { }

      unordered_multiset(const unordered_multiset& __uset,
                    const allocator_type& __a)
      : _Base(__uset, __a) { }

      unordered_multiset(unordered_multiset&& __uset,
                    const allocator_type& __a)
      : _Safe(std::move(__uset._M_safe()), __a),
   _Base(std::move(__uset._M_base()), __a) { }

      unordered_multiset(initializer_list<value_type> __l,
                    size_type __n = 0,
                    const hasher& __hf = hasher(),
                    const key_equal& __eql = key_equal(),
                    const allocator_type& __a = allocator_type())
      : _Base(__l, __n, __hf, __eql, __a) { }

      unordered_multiset(size_type __n, const allocator_type& __a)
   : unordered_multiset(__n, hasher(), key_equal(), __a)
      { }

      unordered_multiset(size_type __n, const hasher& __hf,
                    const allocator_type& __a)
   : unordered_multiset(__n, __hf, key_equal(), __a)
      { }

      template<typename _InputIterator>
   unordered_multiset(_InputIterator __first, _InputIterator __last,
                      size_type __n,
                      const allocator_type& __a)
     : unordered_multiset(__first, __last, __n, hasher(), key_equal(), __a)
   { }

      template<typename _InputIterator>
   unordered_multiset(_InputIterator __first, _InputIterator __last,
                      size_type __n, const hasher& __hf,
                      const allocator_type& __a)
     : unordered_multiset(__first, __last, __n, __hf, key_equal(), __a)
   { }

      unordered_multiset(initializer_list<value_type> __l,
                    size_type __n,
                    const allocator_type& __a)
   : unordered_multiset(__l, __n, hasher(), key_equal(), __a)
      { }

      unordered_multiset(initializer_list<value_type> __l,
                    size_type __n, const hasher& __hf,
                    const allocator_type& __a)
   : unordered_multiset(__l, __n, __hf, key_equal(), __a)
      { }

      ~unordered_multiset() = default;

      unordered_multiset&
      operator=(const unordered_multiset&) = default;

      unordered_multiset&
      operator=(unordered_multiset&&) = default;

      unordered_multiset&
      operator=(initializer_list<value_type> __l)
      {
   this->_M_base() = __l;
   this->_M_invalidate_all();
   return *this;
      }

      void
      swap(unordered_multiset& __x)
   noexcept( noexcept(declval<_Base&>().swap(__x)) )
      {
   _Safe::_M_swap(__x);
   _Base::swap(__x);
      }

      void
      clear() noexcept
      {
   _Base::clear();
   this->_M_invalidate_all();
      }

      iterator
      begin() noexcept
      { return iterator(_Base::begin(), this); }

      const_iterator
      begin() const noexcept
      { return const_iterator(_Base::begin(), this); }

      iterator
      end() noexcept
      { return iterator(_Base::end(), this); }

      const_iterator
      end() const noexcept
      { return const_iterator(_Base::end(), this); }

      const_iterator
      cbegin() const noexcept
      { return const_iterator(_Base::begin(), this); }

      const_iterator
      cend() const noexcept
      { return const_iterator(_Base::end(), this); }

      // local versions
      local_iterator
      begin(size_type __b)
      {
   __glibcxx_check_bucket_index(__b);
   return local_iterator(_Base::begin(__b), this);
      }

      local_iterator
      end(size_type __b)
      {
   __glibcxx_check_bucket_index(__b);
   return local_iterator(_Base::end(__b), this);
      }

      const_local_iterator
      begin(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return const_local_iterator(_Base::begin(__b), this);
      }

      const_local_iterator
      end(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return const_local_iterator(_Base::end(__b), this);
      }

      const_local_iterator
      cbegin(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return const_local_iterator(_Base::cbegin(__b), this);
      }

      const_local_iterator
      cend(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return const_local_iterator(_Base::cend(__b), this);
      }

      size_type
      bucket_size(size_type __b) const
      {
   __glibcxx_check_bucket_index(__b);
   return _Base::bucket_size(__b);
      }

      float
      max_load_factor() const noexcept
      { return _Base::max_load_factor(); }

      void
      max_load_factor(float __f)
      {
   __glibcxx_check_max_load_factor(__f);
   _Base::max_load_factor(__f);
      }

      template<typename... _Args>
   iterator
   emplace(_Args&&... __args)
   {
     size_type __bucket_count = this->bucket_count();
     _Base_iterator __it
       = _Base::emplace(std::forward<_Args>(__args)...);
     _M_check_rehashed(__bucket_count);
     return iterator(__it, this);
   }

      template<typename... _Args>
   iterator
   emplace_hint(const_iterator __hint, _Args&&... __args)
   {
     __glibcxx_check_insert(__hint);
     size_type __bucket_count = this->bucket_count();
     _Base_iterator __it = _Base::emplace_hint(__hint.base(),
                                   std::forward<_Args>(__args)...);
     _M_check_rehashed(__bucket_count);
     return iterator(__it, this);
   }

      iterator
      insert(const value_type& __obj)
      {
   size_type __bucket_count = this->bucket_count();
   _Base_iterator __it = _Base::insert(__obj);
   _M_check_rehashed(__bucket_count);
   return iterator(__it, this);
      }

      iterator
      insert(const_iterator __hint, const value_type& __obj)
      {
   __glibcxx_check_insert(__hint);
   size_type __bucket_count = this->bucket_count();
   _Base_iterator __it = _Base::insert(__hint.base(), __obj);
   _M_check_rehashed(__bucket_count);
   return iterator(__it, this);
      }

      iterator
      insert(value_type&& __obj)
      {
   size_type __bucket_count = this->bucket_count();
   _Base_iterator __it = _Base::insert(std::move(__obj));
   _M_check_rehashed(__bucket_count);
   return iterator(__it, this);
      }

      iterator
      insert(const_iterator __hint, value_type&& __obj)
      {
   __glibcxx_check_insert(__hint);
   size_type __bucket_count = this->bucket_count();
   _Base_iterator __it = _Base::insert(__hint.base(), std::move(__obj));
   _M_check_rehashed(__bucket_count);
   return iterator(__it, this);
      }

      void
      insert(std::initializer_list<value_type> __l)
      {
   size_type __bucket_count = this->bucket_count();
   _Base::insert(__l);
   _M_check_rehashed(__bucket_count);
      }

      template<typename _InputIterator>
   void
   insert(_InputIterator __first, _InputIterator __last)
   {
     typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
     __glibcxx_check_valid_range2(__first, __last, __dist);
     size_type __bucket_count = this->bucket_count();

     if (__dist.second >= __gnu_debug::__dp_sign)
       _Base::insert(__gnu_debug::__unsafe(__first),
                     __gnu_debug::__unsafe(__last));
     else
       _Base::insert(__first, __last);

     _M_check_rehashed(__bucket_count);
   }

#if __cplusplus > 201402L
      using node_type = typename _Base::node_type;

      node_type
      extract(const_iterator __position)
      {
   __glibcxx_check_erase(__position);
   _Base_const_iterator __victim = __position.base();
   this->_M_invalidate_if(
       [__victim](_Base_const_iterator __it) { return __it == __victim; }
       );
   this->_M_invalidate_local_if(
       [__victim](_Base_const_local_iterator __it) {
           return __it._M_curr() == __victim._M_cur;
       });
   return _Base::extract(__position.base());
      }

      node_type
      extract(const key_type& __key)
      {
   const auto __position = find(__key);
   if (__position != end())
     return extract(__position);
   return {};
      }

      iterator
      insert(node_type&& __nh)
      { return iterator(_Base::insert(std::move(__nh)), this); }

      iterator
      insert(const_iterator __hint, node_type&& __nh)
      {
   __glibcxx_check_insert(__hint);
   return iterator(_Base::insert(__hint.base(), std::move(__nh)), this);
      }

      using _Base::merge;
#endif // C++17

      iterator
      find(const key_type& __key)
      { return iterator(_Base::find(__key), this); }

      const_iterator
      find(const key_type& __key) const
      { return const_iterator(_Base::find(__key), this); }

      std::pair<iterator, iterator>
      equal_range(const key_type& __key)
      {
   std::pair<_Base_iterator, _Base_iterator> __res
     = _Base::equal_range(__key);
   return std::make_pair(iterator(__res.first, this),
                         iterator(__res.second, this));
      }

      std::pair<const_iterator, const_iterator>
      equal_range(const key_type& __key) const
      {
   std::pair<_Base_const_iterator, _Base_const_iterator>
     __res = _Base::equal_range(__key);
   return std::make_pair(const_iterator(__res.first, this),
                         const_iterator(__res.second, this));
      }

      size_type
      erase(const key_type& __key)
      {
   size_type __ret(0);
   std::pair<_Base_iterator, _Base_iterator> __pair =
     _Base::equal_range(__key);
   for (_Base_iterator __victim = __pair.first; __victim != __pair.second;)
     {
       this->_M_invalidate_if([__victim](_Base_const_iterator __it)
                       { return __it == __victim; });
       this->_M_invalidate_local_if(
                       [__victim](_Base_const_local_iterator __it)
                       { return __it._M_curr() == __victim._M_cur; });
       _Base::erase(__victim++);
       ++__ret;
     }
   return __ret;
      }

      iterator
      erase(const_iterator __it)
      {
   __glibcxx_check_erase(__it);
   _Base_const_iterator __victim = __it.base();
   this->_M_invalidate_if([__victim](_Base_const_iterator __it)
                   { return __it == __victim; });
   this->_M_invalidate_local_if(
                   [__victim](_Base_const_local_iterator __it)
                   { return __it._M_curr() == __victim._M_cur; });
   return iterator(_Base::erase(__it.base()), this);
      }

      iterator
      erase(iterator __it)
      { return erase(const_iterator(__it)); }

      iterator
      erase(const_iterator __first, const_iterator __last)
      {
   __glibcxx_check_erase_range(__first, __last);
   for (_Base_const_iterator __tmp = __first.base();
        __tmp != __last.base(); ++__tmp)
     {
       _GLIBCXX_DEBUG_VERIFY(__tmp != _Base::end(),
                             _M_message(__gnu_debug::__msg_valid_range)
                             ._M_iterator(__first, "first")
                             ._M_iterator(__last, "last"));
       this->_M_invalidate_if([__tmp](_Base_const_iterator __it)
                       { return __it == __tmp; });
       this->_M_invalidate_local_if(
                       [__tmp](_Base_const_local_iterator __it)
                       { return __it._M_curr() == __tmp._M_cur; });
     }
   return iterator(_Base::erase(__first.base(),
                                __last.base()), this);
      }

      _Base&
      _M_base() noexcept   { return *this; }

      const _Base&
      _M_base() const noexcept     { return *this; }

    private:
      void
      _M_check_rehashed(size_type __prev_count)
      {
   if (__prev_count != this->bucket_count())
     this->_M_invalidate_locals();
      }
    };

#if __cpp_deduction_guides >= 201606

  template<typename _InputIterator,
      typename _Hash =
      hash<typename iterator_traits<_InputIterator>::value_type>,
      typename _Pred =
      equal_to<typename iterator_traits<_InputIterator>::value_type>,
      typename _Allocator =
      allocator<typename iterator_traits<_InputIterator>::value_type>,
      typename = _RequireInputIter<_InputIterator>,
      typename = _RequireAllocator<_Allocator>>
    unordered_multiset(_InputIterator, _InputIterator,
                  unordered_multiset<int>::size_type = {},
                  _Hash = _Hash(), _Pred = _Pred(),
                  _Allocator = _Allocator())
    -> unordered_multiset<typename iterator_traits<_InputIterator>::value_type,
                          _Hash, _Pred, _Allocator>;

  template<typename _Tp, typename _Hash = hash<_Tp>,
      typename _Pred = equal_to<_Tp>,
      typename _Allocator = allocator<_Tp>,
      typename = _RequireAllocator<_Allocator>>
    unordered_multiset(initializer_list<_Tp>,
                  unordered_multiset<int>::size_type = {},
                  _Hash = _Hash(), _Pred = _Pred(),
                  _Allocator = _Allocator())
    -> unordered_multiset<_Tp, _Hash, _Pred, _Allocator>;

  template<typename _InputIterator, typename _Allocator,
      typename = _RequireInputIter<_InputIterator>,
      typename = _RequireAllocator<_Allocator>>
    unordered_multiset(_InputIterator, _InputIterator,
                  unordered_multiset<int>::size_type, _Allocator)
    -> unordered_multiset<typename iterator_traits<_InputIterator>::value_type,
                     hash<typename
                          iterator_traits<_InputIterator>::value_type>,
                     equal_to<typename
                              iterator_traits<_InputIterator>::value_type>,
                     _Allocator>;

  template<typename _InputIterator, typename _Hash, typename _Allocator,
      typename = _RequireInputIter<_InputIterator>,
      typename = _RequireAllocator<_Allocator>>
    unordered_multiset(_InputIterator, _InputIterator,
                  unordered_multiset<int>::size_type,
                  _Hash, _Allocator)
    -> unordered_multiset<typename
                     iterator_traits<_InputIterator>::value_type,
                     _Hash,
                     equal_to<
                       typename
                       iterator_traits<_InputIterator>::value_type>,
                     _Allocator>;

  template<typename _Tp, typename _Allocator,
      typename = _RequireAllocator<_Allocator>>
    unordered_multiset(initializer_list<_Tp>,
                  unordered_multiset<int>::size_type, _Allocator)
    -> unordered_multiset<_Tp, hash<_Tp>, equal_to<_Tp>, _Allocator>;

  template<typename _Tp, typename _Hash, typename _Allocator,
      typename = _RequireAllocator<_Allocator>>
    unordered_multiset(initializer_list<_Tp>,
                  unordered_multiset<int>::size_type, _Hash, _Allocator)
    -> unordered_multiset<_Tp, _Hash, equal_to<_Tp>, _Allocator>;

#endif

  template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
    inline void
    swap(unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __x,
    unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __y)
    noexcept(noexcept(__x.swap(__y)))
    { __x.swap(__y); }

  template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
    inline bool
    operator==(const unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __x,
          const unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __y)
    { return __x._M_base() == __y._M_base(); }

  template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
    inline bool
    operator!=(const unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __x,
          const unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __y)
    { return !(__x == __y); }

} // namespace __debug
} // namespace std

#endif // C++11

#endif
