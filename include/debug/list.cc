// Debugging list implementation -*- C++ -*-

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

/** @file debug/list
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_LIST
#define _GLIBCXX_DEBUG_LIST 1

#pragma GCC system_header

#include <list>
#include <debug/safe_sequence.h>
#include <debug/safe_container.h>
#include <debug/safe_iterator.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __debug
{
  /// Class std::list with safety/checking/debug instrumentation.
  template<typename _Tp, typename _Allocator = std::allocator<_Tp> >
    class list
    : public __gnu_debug::_Safe_container<
   list<_Tp, _Allocator>, _Allocator,
   __gnu_debug::_Safe_node_sequence>,
      public _GLIBCXX_STD_C::list<_Tp, _Allocator>
    {
      typedef _GLIBCXX_STD_C::list<_Tp, _Allocator>                _Base;
      typedef __gnu_debug::_Safe_container<
   list, _Allocator, __gnu_debug::_Safe_node_sequence>     _Safe;

      typedef typename _Base::iterator             _Base_iterator;
      typedef typename _Base::const_iterator       _Base_const_iterator;
      typedef __gnu_debug::_Equal_to<_Base_const_iterator> _Equal;
      typedef __gnu_debug::_Not_equal_to<_Base_const_iterator>  _Not_equal;

    public:
      typedef typename _Base::reference                    reference;
      typedef typename _Base::const_reference              const_reference;

      typedef __gnu_debug::_Safe_iterator<_Base_iterator, list>
                                                   iterator;
      typedef __gnu_debug::_Safe_iterator<_Base_const_iterator, list>
                                                   const_iterator;

      typedef typename _Base::size_type                    size_type;
      typedef typename _Base::difference_type              difference_type;

      typedef _Tp                                  value_type;
      typedef _Allocator                           allocator_type;
      typedef typename _Base::pointer                      pointer;
      typedef typename _Base::const_pointer                const_pointer;
      typedef std::reverse_iterator<iterator>              reverse_iterator;
      typedef std::reverse_iterator<const_iterator>        const_reverse_iterator;

      // 23.2.2.1 construct/copy/destroy:

#if __cplusplus < 201103L
      list()
      : _Base() { }

      list(const list& __x)
      : _Base(__x) { }

      ~list() { }
#else
      list() = default;
      list(const list&) = default;
      list(list&&) = default;

      list(initializer_list<value_type> __l,
      const allocator_type& __a = allocator_type())
      : _Base(__l, __a) { }

      ~list() = default;

      list(const list& __x, const allocator_type& __a)
      : _Base(__x, __a) { }

      list(list&& __x, const allocator_type& __a)
      : _Base(std::move(__x), __a) { }
#endif

      explicit
      list(const _Allocator& __a) _GLIBCXX_NOEXCEPT
      : _Base(__a) { }

#if __cplusplus >= 201103L
      explicit
      list(size_type __n, const allocator_type& __a = allocator_type())
      : _Base(__n, __a) { }

      list(size_type __n, const _Tp& __value,
      const _Allocator& __a = _Allocator())
      : _Base(__n, __value, __a) { }
#else
      explicit
      list(size_type __n, const _Tp& __value = _Tp(),
      const _Allocator& __a = _Allocator())
      : _Base(__n, __value, __a) { }
#endif

#if __cplusplus >= 201103L
      template<class _InputIterator,
          typename = std::_RequireInputIter<_InputIterator>>
#else
      template<class _InputIterator>
#endif
   list(_InputIterator __first, _InputIterator __last,
        const _Allocator& __a = _Allocator())
   : _Base(__gnu_debug::__base(__gnu_debug::__check_valid_range(__first,
                                                                __last)),
           __gnu_debug::__base(__last), __a)
   { }

      list(const _Base& __x)
      : _Base(__x) { }

#if __cplusplus < 201103L
      list&
      operator=(const list& __x)
      {
   this->_M_safe() = __x;
   _M_base() = __x;
   return *this;
      }
#else
      list&
      operator=(const list&) = default;

      list&
      operator=(list&&) = default;

      list&
      operator=(initializer_list<value_type> __l)
      {
   this->_M_invalidate_all();
   _M_base() = __l;
   return *this;
      }

      void
      assign(initializer_list<value_type> __l)
      {
   _Base::assign(__l);
   this->_M_invalidate_all();
      }
#endif

#if __cplusplus >= 201103L
      template<class _InputIterator,
          typename = std::_RequireInputIter<_InputIterator>>
#else
      template<class _InputIterator>
#endif
   void
   assign(_InputIterator __first, _InputIterator __last)
   {
     typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
     __glibcxx_check_valid_range2(__first, __last, __dist);

     if (__dist.second >= __gnu_debug::__dp_sign)
       _Base::assign(__gnu_debug::__unsafe(__first),
                     __gnu_debug::__unsafe(__last));
     else
       _Base::assign(__first, __last);

     this->_M_invalidate_all();
   }

      void
      assign(size_type __n, const _Tp& __t)
      {
   _Base::assign(__n, __t);
   this->_M_invalidate_all();
      }

      using _Base::get_allocator;

      // iterators:
      iterator
      begin() _GLIBCXX_NOEXCEPT
      { return iterator(_Base::begin(), this); }

      const_iterator
      begin() const _GLIBCXX_NOEXCEPT
      { return const_iterator(_Base::begin(), this); }

      iterator
      end() _GLIBCXX_NOEXCEPT
      { return iterator(_Base::end(), this); }

      const_iterator
      end() const _GLIBCXX_NOEXCEPT
      { return const_iterator(_Base::end(), this); }

      reverse_iterator
      rbegin() _GLIBCXX_NOEXCEPT
      { return reverse_iterator(end()); }

      const_reverse_iterator
      rbegin() const _GLIBCXX_NOEXCEPT
      { return const_reverse_iterator(end()); }

      reverse_iterator
      rend() _GLIBCXX_NOEXCEPT
      { return reverse_iterator(begin()); }

      const_reverse_iterator
      rend() const _GLIBCXX_NOEXCEPT
      { return const_reverse_iterator(begin()); }

#if __cplusplus >= 201103L
      const_iterator
      cbegin() const noexcept
      { return const_iterator(_Base::begin(), this); }

      const_iterator
      cend() const noexcept
      { return const_iterator(_Base::end(), this); }

      const_reverse_iterator
      crbegin() const noexcept
      { return const_reverse_iterator(end()); }

      const_reverse_iterator
      crend() const noexcept
      { return const_reverse_iterator(begin()); }
#endif

      // 23.2.2.2 capacity:
      using _Base::empty;
      using _Base::size;
      using _Base::max_size;

#if __cplusplus >= 201103L
      void
      resize(size_type __sz)
      {
   this->_M_detach_singular();

   // if __sz < size(), invalidate all iterators in [begin + __sz, end())
   _Base_iterator __victim = _Base::begin();
   _Base_iterator __end = _Base::end();
   for (size_type __i = __sz; __victim != __end && __i > 0; --__i)
     ++__victim;

   for (; __victim != __end; ++__victim)
     this->_M_invalidate_if(_Equal(__victim));

   __try
     {
       _Base::resize(__sz);
     }
   __catch(...)
     {
       this->_M_revalidate_singular();
       __throw_exception_again;
     }
      }

      void
      resize(size_type __sz, const _Tp& __c)
      {
   this->_M_detach_singular();

   // if __sz < size(), invalidate all iterators in [begin + __sz, end())
   _Base_iterator __victim = _Base::begin();
   _Base_iterator __end = _Base::end();
   for (size_type __i = __sz; __victim != __end && __i > 0; --__i)
     ++__victim;

   for (; __victim != __end; ++__victim)
     this->_M_invalidate_if(_Equal(__victim));

   __try
     {
       _Base::resize(__sz, __c);
     }
   __catch(...)
     {
       this->_M_revalidate_singular();
       __throw_exception_again;
     }
      }
#else
      void
      resize(size_type __sz, _Tp __c = _Tp())
      {
   this->_M_detach_singular();

   // if __sz < size(), invalidate all iterators in [begin + __sz, end())
   _Base_iterator __victim = _Base::begin();
   _Base_iterator __end = _Base::end();
   for (size_type __i = __sz; __victim != __end && __i > 0; --__i)
     ++__victim;

   for (; __victim != __end; ++__victim)
     this->_M_invalidate_if(_Equal(__victim));

   __try
     {
       _Base::resize(__sz, __c);
     }
   __catch(...)
     {
       this->_M_revalidate_singular();
       __throw_exception_again;
     }
      }
#endif

      // element access:
      reference
      front() _GLIBCXX_NOEXCEPT
      {
   __glibcxx_check_nonempty();
   return _Base::front();
      }

      const_reference
      front() const _GLIBCXX_NOEXCEPT
      {
   __glibcxx_check_nonempty();
   return _Base::front();
      }

      reference
      back() _GLIBCXX_NOEXCEPT
      {
   __glibcxx_check_nonempty();
   return _Base::back();
      }

      const_reference
      back() const _GLIBCXX_NOEXCEPT
      {
   __glibcxx_check_nonempty();
   return _Base::back();
      }

      // 23.2.2.3 modifiers:
      using _Base::push_front;

#if __cplusplus >= 201103L
      using _Base::emplace_front;
#endif

      void
      pop_front() _GLIBCXX_NOEXCEPT
      {
   __glibcxx_check_nonempty();
   this->_M_invalidate_if(_Equal(_Base::begin()));
   _Base::pop_front();
      }

      using _Base::push_back;

#if __cplusplus >= 201103L
      using _Base::emplace_back;
#endif

      void
      pop_back() _GLIBCXX_NOEXCEPT
      {
   __glibcxx_check_nonempty();
   this->_M_invalidate_if(_Equal(--_Base::end()));
   _Base::pop_back();
      }

#if __cplusplus >= 201103L
      template<typename... _Args>
   iterator
   emplace(const_iterator __position, _Args&&... __args)
   {
     __glibcxx_check_insert(__position);
     return iterator(_Base::emplace(__position.base(),
                                   std::forward<_Args>(__args)...), this);
   }
#endif

     iterator
#if __cplusplus >= 201103L
     insert(const_iterator __position, const _Tp& __x)
#else
     insert(iterator __position, const _Tp& __x)
#endif
     {
       __glibcxx_check_insert(__position);
       return iterator(_Base::insert(__position.base(), __x), this);
     }

#if __cplusplus >= 201103L
      iterator
      insert(const_iterator __position, _Tp&& __x)
      { return emplace(__position, std::move(__x)); }

      iterator
      insert(const_iterator __p, initializer_list<value_type> __l)
      {
   __glibcxx_check_insert(__p);
   return iterator(_Base::insert(__p.base(), __l), this);
      }
#endif

#if __cplusplus >= 201103L
      iterator
      insert(const_iterator __position, size_type __n, const _Tp& __x)
      {
   __glibcxx_check_insert(__position);
   return iterator(_Base::insert(__position.base(), __n, __x), this);
      }
#else
      void
      insert(iterator __position, size_type __n, const _Tp& __x)
      {
   __glibcxx_check_insert(__position);
   _Base::insert(__position.base(), __n, __x);
      }
#endif

#if __cplusplus >= 201103L
      template<class _InputIterator,
          typename = std::_RequireInputIter<_InputIterator>>
   iterator
   insert(const_iterator __position, _InputIterator __first,
          _InputIterator __last)
   {
     typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
     __glibcxx_check_insert_range(__position, __first, __last, __dist);
     if (__dist.second >= __gnu_debug::__dp_sign)
       return
         {
           _Base::insert(__position.base(),
                         __gnu_debug::__unsafe(__first),
                         __gnu_debug::__unsafe(__last)),
             this
         };
     else
       return { _Base::insert(__position.base(), __first, __last), this };
   }
#else
      template<class _InputIterator>
   void
   insert(iterator __position, _InputIterator __first,
          _InputIterator __last)
   {
     typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
     __glibcxx_check_insert_range(__position, __first, __last, __dist);

     if (__dist.second >= __gnu_debug::__dp_sign)
       _Base::insert(__position.base(), __gnu_debug::__unsafe(__first),
                                        __gnu_debug::__unsafe(__last));
     else
       _Base::insert(__position.base(), __first, __last);
   }
#endif

    private:
      _Base_iterator
#if __cplusplus >= 201103L
      _M_erase(_Base_const_iterator __position) noexcept
#else
      _M_erase(_Base_iterator __position)
#endif
      {
   this->_M_invalidate_if(_Equal(__position));
   return _Base::erase(__position);
      }

    public:
      iterator
#if __cplusplus >= 201103L
      erase(const_iterator __position) noexcept
#else
      erase(iterator __position)
#endif
      {
   __glibcxx_check_erase(__position);
   return iterator(_M_erase(__position.base()), this);
      }

      iterator
#if __cplusplus >= 201103L
      erase(const_iterator __first, const_iterator __last) noexcept
#else
      erase(iterator __first, iterator __last)
#endif
      {
   // _GLIBCXX_RESOLVE_LIB_DEFECTS
   // 151. can't currently clear() empty container
   __glibcxx_check_erase_range(__first, __last);
   for (_Base_const_iterator __victim = __first.base();
        __victim != __last.base(); ++__victim)
     {
       _GLIBCXX_DEBUG_VERIFY(__victim != _Base::end(),
                             _M_message(__gnu_debug::__msg_valid_range)
                             ._M_iterator(__first, "position")
                             ._M_iterator(__last, "last"));
       this->_M_invalidate_if(_Equal(__victim));
     }
   return iterator(_Base::erase(__first.base(), __last.base()), this);
      }

      void
      swap(list& __x)
      _GLIBCXX_NOEXCEPT_IF( noexcept(declval<_Base&>().swap(__x)) )
      {
   _Safe::_M_swap(__x);
   _Base::swap(__x);
      }

      void
      clear() _GLIBCXX_NOEXCEPT
      {
   _Base::clear();
   this->_M_invalidate_all();
      }

      // 23.2.2.4 list operations:
      void
#if __cplusplus >= 201103L
      splice(const_iterator __position, list&& __x) noexcept
#else
      splice(iterator __position, list& __x)
#endif
      {
   _GLIBCXX_DEBUG_VERIFY(std::__addressof(__x) != this,
                         _M_message(__gnu_debug::__msg_self_splice)
                         ._M_sequence(*this, "this"));
   this->_M_transfer_from_if(__x, _Not_equal(__x._M_base().end()));
   _Base::splice(__position.base(), _GLIBCXX_MOVE(__x._M_base()));
      }

#if __cplusplus >= 201103L
      void
      splice(const_iterator __position, list& __x) noexcept
      { splice(__position, std::move(__x)); }
#endif

      void
#if __cplusplus >= 201103L
      splice(const_iterator __position, list&& __x, const_iterator __i) noexcept
#else
      splice(iterator __position, list& __x, iterator __i)
#endif
      {
   __glibcxx_check_insert(__position);

   // We used to perform the splice_alloc check:  not anymore, redundant
   // after implementing the relevant bits of N1599.

   _GLIBCXX_DEBUG_VERIFY(__i._M_dereferenceable(),
                         _M_message(__gnu_debug::__msg_splice_bad)
                         ._M_iterator(__i, "__i"));
   _GLIBCXX_DEBUG_VERIFY(__i._M_attached_to(std::__addressof(__x)),
                         _M_message(__gnu_debug::__msg_splice_other)
                        ._M_iterator(__i, "__i")._M_sequence(__x, "__x"));

   // _GLIBCXX_RESOLVE_LIB_DEFECTS
   // 250. splicing invalidates iterators
   this->_M_transfer_from_if(__x, _Equal(__i.base()));
   _Base::splice(__position.base(), _GLIBCXX_MOVE(__x._M_base()),
                 __i.base());
      }

#if __cplusplus >= 201103L
      void
      splice(const_iterator __position, list& __x, const_iterator __i) noexcept
      { splice(__position, std::move(__x), __i); }
#endif

      void
#if __cplusplus >= 201103L
      splice(const_iterator __position, list&& __x, const_iterator __first,
        const_iterator __last) noexcept
#else
      splice(iterator __position, list& __x, iterator __first,
        iterator __last)
#endif
      {
   __glibcxx_check_insert(__position);
   __glibcxx_check_valid_range(__first, __last);
   _GLIBCXX_DEBUG_VERIFY(__first._M_attached_to(std::__addressof(__x)),
                         _M_message(__gnu_debug::__msg_splice_other)
                         ._M_sequence(__x, "x")
                         ._M_iterator(__first, "first"));

   // We used to perform the splice_alloc check:  not anymore, redundant
   // after implementing the relevant bits of N1599.

   for (_Base_const_iterator __tmp = __first.base();
        __tmp != __last.base(); ++__tmp)
     {
       _GLIBCXX_DEBUG_VERIFY(__tmp != _Base::end(),
                             _M_message(__gnu_debug::__msg_valid_range)
                             ._M_iterator(__first, "first")
                             ._M_iterator(__last, "last"));
       _GLIBCXX_DEBUG_VERIFY(std::__addressof(__x) != this
                             || __tmp != __position.base(),
                           _M_message(__gnu_debug::__msg_splice_overlap)
                             ._M_iterator(__tmp, "position")
                             ._M_iterator(__first, "first")
                             ._M_iterator(__last, "last"));
       // _GLIBCXX_RESOLVE_LIB_DEFECTS
       // 250. splicing invalidates iterators
       this->_M_transfer_from_if(__x, _Equal(__tmp));
     }

   _Base::splice(__position.base(), _GLIBCXX_MOVE(__x._M_base()),
                 __first.base(), __last.base());
      }

#if __cplusplus >= 201103L
      void
      splice(const_iterator __position, list& __x,
        const_iterator __first, const_iterator __last) noexcept
      { splice(__position, std::move(__x), __first, __last); }
#endif

      void
      remove(const _Tp& __value)
      {
   for (_Base_iterator __x = _Base::begin(); __x != _Base::end(); )
     {
       if (*__x == __value)
         __x = _M_erase(__x);
       else
         ++__x;
     }
      }

      template<class _Predicate>
   void
   remove_if(_Predicate __pred)
   {
     for (_Base_iterator __x = _Base::begin(); __x != _Base::end(); )
       {
         if (__pred(*__x))
           __x = _M_erase(__x);
         else
           ++__x;
       }
   }

      void
      unique()
      {
   _Base_iterator __first = _Base::begin();
   _Base_iterator __last = _Base::end();
   if (__first == __last)
     return;
   _Base_iterator __next = __first; ++__next;
   while (__next != __last)
     {
       if (*__first == *__next)
         __next = _M_erase(__next);
       else
         __first = __next++;
     }
      }

      template<class _BinaryPredicate>
   void
   unique(_BinaryPredicate __binary_pred)
   {
     _Base_iterator __first = _Base::begin();
     _Base_iterator __last = _Base::end();
     if (__first == __last)
       return;
     _Base_iterator __next = __first; ++__next;
     while (__next != __last)
       {
         if (__binary_pred(*__first, *__next))
           __next = _M_erase(__next);
         else
           __first = __next++;
       }
   }

      void
#if __cplusplus >= 201103L
      merge(list&& __x)
#else
      merge(list& __x)
#endif
      {
   // _GLIBCXX_RESOLVE_LIB_DEFECTS
   // 300. list::merge() specification incomplete
   if (this != std::__addressof(__x))
     {
       __glibcxx_check_sorted(_Base::begin(), _Base::end());
       __glibcxx_check_sorted(__x.begin().base(), __x.end().base());
       this->_M_transfer_from_if(__x, _Not_equal(__x._M_base().end()));
       _Base::merge(_GLIBCXX_MOVE(__x._M_base()));
     }
      }

#if __cplusplus >= 201103L
      void
      merge(list& __x)
      { merge(std::move(__x)); }
#endif

      template<class _Compare>
   void
#if __cplusplus >= 201103L
   merge(list&& __x, _Compare __comp)
#else
   merge(list& __x, _Compare __comp)
#endif
   {
     // _GLIBCXX_RESOLVE_LIB_DEFECTS
     // 300. list::merge() specification incomplete
     if (this != std::__addressof(__x))
       {
         __glibcxx_check_sorted_pred(_Base::begin(), _Base::end(),
                                     __comp);
         __glibcxx_check_sorted_pred(__x.begin().base(), __x.end().base(),
                                     __comp);
         this->_M_transfer_from_if(__x, _Not_equal(__x._M_base().end()));
         _Base::merge(_GLIBCXX_MOVE(__x._M_base()), __comp);
       }
   }

#if __cplusplus >= 201103L
      template<typename _Compare>
   void
   merge(list& __x, _Compare __comp)
   { merge(std::move(__x), __comp); }
#endif

      void
      sort() { _Base::sort(); }

      template<typename _StrictWeakOrdering>
   void
   sort(_StrictWeakOrdering __pred) { _Base::sort(__pred); }

      using _Base::reverse;

      _Base&
      _M_base() _GLIBCXX_NOEXCEPT  { return *this; }

      const _Base&
      _M_base() const _GLIBCXX_NOEXCEPT    { return *this; }
    };

#if __cpp_deduction_guides >= 201606
  template<typename _InputIterator, typename _ValT
        = typename iterator_traits<_InputIterator>::value_type,
      typename _Allocator = allocator<_ValT>,
      typename = _RequireInputIter<_InputIterator>,
      typename = _RequireAllocator<_Allocator>>
    list(_InputIterator, _InputIterator, _Allocator = _Allocator())
      -> list<_ValT, _Allocator>;
#endif

  template<typename _Tp, typename _Alloc>
    inline bool
    operator==(const list<_Tp, _Alloc>& __lhs,
          const list<_Tp, _Alloc>& __rhs)
    { return __lhs._M_base() == __rhs._M_base(); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator!=(const list<_Tp, _Alloc>& __lhs,
          const list<_Tp, _Alloc>& __rhs)
    { return __lhs._M_base() != __rhs._M_base(); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator<(const list<_Tp, _Alloc>& __lhs,
         const list<_Tp, _Alloc>& __rhs)
    { return __lhs._M_base() < __rhs._M_base(); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator<=(const list<_Tp, _Alloc>& __lhs,
          const list<_Tp, _Alloc>& __rhs)
    { return __lhs._M_base() <= __rhs._M_base(); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator>=(const list<_Tp, _Alloc>& __lhs,
          const list<_Tp, _Alloc>& __rhs)
    { return __lhs._M_base() >= __rhs._M_base(); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator>(const list<_Tp, _Alloc>& __lhs,
         const list<_Tp, _Alloc>& __rhs)
    { return __lhs._M_base() > __rhs._M_base(); }

  template<typename _Tp, typename _Alloc>
    inline void
    swap(list<_Tp, _Alloc>& __lhs, list<_Tp, _Alloc>& __rhs)
    _GLIBCXX_NOEXCEPT_IF(noexcept(__lhs.swap(__rhs)))
    { __lhs.swap(__rhs); }

} // namespace __debug
} // namespace std

namespace __gnu_debug
{
#ifndef _GLIBCXX_USE_CXX11_ABI
  // If not using C++11 list::size() is not in O(1) so we do not use it.
  template<typename _Tp, typename _Alloc>
    struct _Sequence_traits<std::__debug::list<_Tp, _Alloc> >
    {
      typedef typename std::__debug::list<_Tp, _Alloc>::iterator _It;

      static typename _Distance_traits<_It>::__type
      _S_size(const std::__debug::list<_Tp, _Alloc>& __seq)
      {
   return __seq.empty()
     ? std::make_pair(0, __dp_exact) : std::make_pair(1, __dp_equality);
      }
    };
#endif

#ifndef _GLIBCXX_DEBUG_PEDANTIC
  template<class _Tp, class _Alloc>
    struct _Insert_range_from_self_is_safe<std::__debug::list<_Tp, _Alloc> >
    { enum { __value = 1 }; };
#endif
}

#endif
