// <forward_list> -*- C++ -*-

// Copyright (C) 2010-2018 Free Software Foundation, Inc.
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

/** @file debug/forward_list
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_FORWARD_LIST
#define _GLIBCXX_DEBUG_FORWARD_LIST 1

#pragma GCC system_header

#include <forward_list>
#include <debug/safe_sequence.h>
#include <debug/safe_container.h>
#include <debug/safe_iterator.h>

// Special validity check for forward_list ranges.
#define __glibcxx_check_valid_fl_range(_First,_Last,_Dist)         \\
_GLIBCXX_DEBUG_VERIFY(_First._M_valid_range(_Last, _Dist, false),  \\
                 _M_message(__gnu_debug::__msg_valid_range)        \\
                 ._M_iterator(_First, #_First)                     \\
                 ._M_iterator(_Last, #_Last))

namespace __gnu_debug
{
  /// Special iterators swap and invalidation for forward_list because of the
  /// before_begin iterator.
  template<typename _SafeSequence>
    class _Safe_forward_list
    : public _Safe_sequence<_SafeSequence>
    {
      _SafeSequence&
      _M_this() noexcept
      { return *static_cast<_SafeSequence*>(this); }

      static void
      _M_swap_aux(_Safe_sequence_base& __lhs,
             _Safe_iterator_base*& __lhs_iterators,
             _Safe_sequence_base& __rhs,
             _Safe_iterator_base*& __rhs_iterators);

      void _M_swap_single(_Safe_sequence_base&) noexcept;

    protected:
      void
      _M_invalidate_all()
      {
   using _Base_const_iterator = __decltype(_M_this()._M_base().cend());
   this->_M_invalidate_if([this](_Base_const_iterator __it)
   {
     return __it != _M_this()._M_base().cbefore_begin()
       && __it != _M_this()._M_base().cend(); });
      }

      void _M_swap(_Safe_sequence_base&) noexcept;
    };

   template<typename _SafeSequence>
    void
    _Safe_forward_list<_SafeSequence>::
    _M_swap_aux(_Safe_sequence_base& __lhs,
           _Safe_iterator_base*& __lhs_iterators,
           _Safe_sequence_base& __rhs,
           _Safe_iterator_base*& __rhs_iterators)
    {
      using const_iterator = typename _SafeSequence::const_iterator;
      _Safe_iterator_base* __bbegin_its = 0;
      _Safe_iterator_base* __last_bbegin = 0;
      _SafeSequence& __rseq = static_cast<_SafeSequence&>(__rhs);

      for (_Safe_iterator_base* __iter = __lhs_iterators; __iter;)
   {
     // Even iterator is cast to const_iterator, not a problem.
     _Safe_iterator_base* __victim_base = __iter;
     const_iterator* __victim =
       static_cast<const_iterator*>(__victim_base);
     __iter = __iter->_M_next;
     if (__victim->base() == __rseq._M_base().cbefore_begin())
       {
         __victim->_M_unlink();
         if (__lhs_iterators == __victim_base)
           __lhs_iterators = __victim_base->_M_next;
         if (__bbegin_its)
           {
             __victim_base->_M_next = __bbegin_its;
             __bbegin_its->_M_prior = __victim_base;
           }
         else
           __last_bbegin = __victim_base;
         __bbegin_its = __victim_base;
       }
     else
       __victim_base->_M_sequence = std::__addressof(__lhs);
   }

      if (__bbegin_its)
   {
     if (__rhs_iterators)
       {
         __rhs_iterators->_M_prior = __last_bbegin;
         __last_bbegin->_M_next = __rhs_iterators;
       }
     __rhs_iterators = __bbegin_its;
   }
    }

   template<typename _SafeSequence>
    void
    _Safe_forward_list<_SafeSequence>::
    _M_swap_single(_Safe_sequence_base& __other) noexcept
    {
      std::swap(_M_this()._M_iterators, __other._M_iterators);
      std::swap(_M_this()._M_const_iterators, __other._M_const_iterators);
      // Useless, always 1 on forward_list
      //std::swap(_M_this()_M_version, __other._M_version);
      _Safe_iterator_base* __this_its = _M_this()._M_iterators;
      _M_swap_aux(__other, __other._M_iterators,
             _M_this(), _M_this()._M_iterators);
      _Safe_iterator_base* __this_const_its = _M_this()._M_const_iterators;
      _M_swap_aux(__other, __other._M_const_iterators,
             _M_this(), _M_this()._M_const_iterators);
      _M_swap_aux(_M_this(), __this_its,
             __other, __other._M_iterators);
      _M_swap_aux(_M_this(), __this_const_its,
             __other, __other._M_const_iterators);
    }

  /* Special forward_list _M_swap version that does not swap the
   * before-begin ownership.*/
   template<typename _SafeSequence>
    void
    _Safe_forward_list<_SafeSequence>::
    _M_swap(_Safe_sequence_base& __other) noexcept
    {
      // We need to lock both sequences to swap
      using namespace __gnu_cxx;
      __mutex *__this_mutex = &_M_this()._M_get_mutex();
      __mutex *__other_mutex =
   &static_cast<_SafeSequence&>(__other)._M_get_mutex();
      if (__this_mutex == __other_mutex)
   {
     __scoped_lock __lock(*__this_mutex);
     _M_swap_single(__other);
   }
      else
   {
     __scoped_lock __l1(__this_mutex < __other_mutex
                        ? *__this_mutex : *__other_mutex);
     __scoped_lock __l2(__this_mutex < __other_mutex
                        ? *__other_mutex : *__this_mutex);
     _M_swap_single(__other);
   }
    }
}

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __debug
{
  /// Class std::forward_list with safety/checking/debug instrumentation.
  template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class forward_list
    : public __gnu_debug::_Safe_container<
   forward_list<_Tp, _Alloc>, _Alloc, __gnu_debug::_Safe_forward_list>,
      public _GLIBCXX_STD_C::forward_list<_Tp, _Alloc>
    {
      typedef _GLIBCXX_STD_C::forward_list<_Tp, _Alloc>            _Base;
      typedef __gnu_debug::_Safe_container<
   forward_list, _Alloc, __gnu_debug::_Safe_forward_list>  _Safe;

      typedef typename _Base::iterator             _Base_iterator;
      typedef typename _Base::const_iterator       _Base_const_iterator;

    public:
      typedef typename _Base::reference            reference;
      typedef typename _Base::const_reference      const_reference;

      typedef __gnu_debug::_Safe_iterator<
   _Base_iterator, forward_list>           iterator;
      typedef __gnu_debug::_Safe_iterator<
   _Base_const_iterator, forward_list>     const_iterator;

      typedef typename _Base::size_type            size_type;
      typedef typename _Base::difference_type      difference_type;

      typedef _Tp                          value_type;
      typedef typename _Base::allocator_type       allocator_type;
      typedef typename _Base::pointer              pointer;
      typedef typename _Base::const_pointer        const_pointer;

      // 23.2.3.1 construct/copy/destroy:

      forward_list() = default;

      explicit
      forward_list(const allocator_type& __al) noexcept
      : _Base(__al) { }

      forward_list(const forward_list& __list, const allocator_type& __al)
      : _Base(__list, __al)
      { }

      forward_list(forward_list&& __list, const allocator_type& __al)
   : _Safe(std::move(__list._M_safe()), __al),
     _Base(std::move(__list._M_base()), __al)
      { }

      explicit
      forward_list(size_type __n, const allocator_type& __al = allocator_type())
      : _Base(__n, __al)
      { }

      forward_list(size_type __n, const _Tp& __value,
              const allocator_type& __al = allocator_type())
      : _Base(__n, __value, __al)
      { }

      template<typename _InputIterator,
          typename = std::_RequireInputIter<_InputIterator>>
   forward_list(_InputIterator __first, _InputIterator __last,
                const allocator_type& __al = allocator_type())
   : _Base(__gnu_debug::__base(__gnu_debug::__check_valid_range(__first,
                                                                __last)),
           __gnu_debug::__base(__last), __al)
   { }

      forward_list(const forward_list&) = default;

      forward_list(forward_list&&) = default;

      forward_list(std::initializer_list<_Tp> __il,
              const allocator_type& __al = allocator_type())
      : _Base(__il, __al)
      { }

      ~forward_list() = default;

      forward_list&
      operator=(const forward_list&) = default;

      forward_list&
      operator=(forward_list&&) = default;

      forward_list&
      operator=(std::initializer_list<_Tp> __il)
      {
   _M_base() = __il;
   this->_M_invalidate_all();
   return *this;
      }

      template<typename _InputIterator,
          typename = std::_RequireInputIter<_InputIterator>>
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
      assign(size_type __n, const _Tp& __val)
      {
   _Base::assign(__n, __val);
   this->_M_invalidate_all();
      }

      void
      assign(std::initializer_list<_Tp> __il)
      {
   _Base::assign(__il);
   this->_M_invalidate_all();
      }

      using _Base::get_allocator;

      // iterators:

      iterator
      before_begin() noexcept
      { return iterator(_Base::before_begin(), this); }

      const_iterator
      before_begin() const noexcept
      { return const_iterator(_Base::before_begin(), this); }

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
      { return const_iterator(_Base::cbegin(), this); }

      const_iterator
      cbefore_begin() const noexcept
      { return const_iterator(_Base::cbefore_begin(), this); }

      const_iterator
      cend() const noexcept
      { return const_iterator(_Base::cend(), this); }

      using _Base::empty;
      using _Base::max_size;

      // element access:

      reference
      front()
      {
   __glibcxx_check_nonempty();
   return _Base::front();
      }

      const_reference
      front() const
      {
   __glibcxx_check_nonempty();
   return _Base::front();
      }

      // modi\xef\xac\x81ers:

      using _Base::emplace_front;
      using _Base::push_front;

      void
      pop_front()
      {
   __glibcxx_check_nonempty();
   this->_M_invalidate_if([this](_Base_const_iterator __it)
     { return __it == this->_M_base().cbegin(); });
   _Base::pop_front();
      }

      template<typename... _Args>
   iterator
   emplace_after(const_iterator __pos, _Args&&... __args)
   {
     __glibcxx_check_insert_after(__pos);
     return iterator(_Base::emplace_after(__pos.base(),
                                   std::forward<_Args>(__args)...),
                     this);
           }

      iterator
      insert_after(const_iterator __pos, const _Tp& __val)
      {
   __glibcxx_check_insert_after(__pos);
   return iterator(_Base::insert_after(__pos.base(), __val), this);
      }

      iterator
      insert_after(const_iterator __pos, _Tp&& __val)
      {
   __glibcxx_check_insert_after(__pos);
   return iterator(_Base::insert_after(__pos.base(), std::move(__val)),
                   this);
      }

      iterator
      insert_after(const_iterator __pos, size_type __n, const _Tp& __val)
      {
   __glibcxx_check_insert_after(__pos);
   return iterator(_Base::insert_after(__pos.base(), __n, __val),
                   this);
      }

      template<typename _InputIterator,
          typename = std::_RequireInputIter<_InputIterator>>
   iterator
   insert_after(const_iterator __pos,
                _InputIterator __first, _InputIterator __last)
   {
     typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
     __glibcxx_check_insert_range_after(__pos, __first, __last, __dist);

     if (__dist.second >= __gnu_debug::__dp_sign)
       return
         {
           _Base::insert_after(__pos.base(),
                               __gnu_debug::__unsafe(__first),
                               __gnu_debug::__unsafe(__last)),
           this
         };
     else
       return { _Base::insert_after(__pos.base(), __first, __last), this };
   }

      iterator
      insert_after(const_iterator __pos, std::initializer_list<_Tp> __il)
      {
   __glibcxx_check_insert_after(__pos);
   return iterator(_Base::insert_after(__pos.base(), __il), this);
      }

    private:
      _Base_iterator
      _M_erase_after(_Base_const_iterator __pos)
      {
   _Base_const_iterator __next = std::next(__pos);
   this->_M_invalidate_if([__next](_Base_const_iterator __it)
     { return __it == __next; });
   return _Base::erase_after(__pos);
      }
    public:
      iterator
      erase_after(const_iterator __pos)
      {
   __glibcxx_check_erase_after(__pos);
   return iterator(_M_erase_after(__pos.base()), this);
      }

      iterator
      erase_after(const_iterator __pos, const_iterator __last)
      {
   __glibcxx_check_erase_range_after(__pos, __last);
   for (_Base_const_iterator __victim = std::next(__pos.base());
       __victim != __last.base(); ++__victim)
     {
       _GLIBCXX_DEBUG_VERIFY(__victim != _Base::end(),
                             _M_message(__gnu_debug::__msg_valid_range2)
                             ._M_sequence(*this, "this")
                             ._M_iterator(__pos, "pos")
                             ._M_iterator(__last, "last"));
       this->_M_invalidate_if([__victim](_Base_const_iterator __it)
         { return __it == __victim; });
     }
   return iterator(_Base::erase_after(__pos.base(), __last.base()), this);
      }

      void
      swap(forward_list& __list)
   noexcept( noexcept(declval<_Base&>().swap(__list)) )
      {
   _Safe::_M_swap(__list);
   _Base::swap(__list);
      }

      void
      resize(size_type __sz)
      {
   this->_M_detach_singular();

   // if __sz < size(), invalidate all iterators in [begin+__sz, end()
   _Base_iterator __victim = _Base::begin();
   _Base_iterator __end = _Base::end();
   for (size_type __i = __sz; __victim != __end && __i > 0; --__i)
     ++__victim;

   for (; __victim != __end; ++__victim)
     {
       this->_M_invalidate_if([__victim](_Base_const_iterator __it)
         { return __it == __victim; });
     }

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
      resize(size_type __sz, const value_type& __val)
      {
   this->_M_detach_singular();

   // if __sz < size(), invalidate all iterators in [begin+__sz, end())
   _Base_iterator __victim = _Base::begin();
   _Base_iterator __end = _Base::end();
   for (size_type __i = __sz; __victim != __end && __i > 0; --__i)
     ++__victim;

   for (; __victim != __end; ++__victim)
     {
       this->_M_invalidate_if([__victim](_Base_const_iterator __it)
         { return __it == __victim; });
     }

   __try
     {
       _Base::resize(__sz, __val);
     }
   __catch(...)
     {
       this->_M_revalidate_singular();
       __throw_exception_again;
     }
      }

      void
      clear() noexcept
      {
   _Base::clear();
   this->_M_invalidate_all();
      }

      // 23.2.3.5 forward_list operations:
      void
      splice_after(const_iterator __pos, forward_list&& __list)
      {
   __glibcxx_check_insert_after(__pos);
   _GLIBCXX_DEBUG_VERIFY(std::__addressof(__list) != this,
                         _M_message(__gnu_debug::__msg_self_splice)
                         ._M_sequence(*this, "this"));
   _GLIBCXX_DEBUG_VERIFY(__list.get_allocator() == this->get_allocator(),
                         _M_message(__gnu_debug::__msg_splice_alloc)
                         ._M_sequence(*this)
                         ._M_sequence(__list, "__list"));
   this->_M_transfer_from_if(__list, [&__list](_Base_const_iterator __it)
     {
       return __it != __list._M_base().cbefore_begin()
              && __it != __list._M_base().end();
     });
   _Base::splice_after(__pos.base(), std::move(__list._M_base()));
      }

      void
      splice_after(const_iterator __pos, forward_list& __list)
      { splice_after(__pos, std::move(__list)); }

      void
      splice_after(const_iterator __pos, forward_list&& __list,
              const_iterator __i)
      {
   __glibcxx_check_insert_after(__pos);
   _GLIBCXX_DEBUG_VERIFY(__i._M_before_dereferenceable(),
                         _M_message(__gnu_debug::__msg_splice_bad)
                         ._M_iterator(__i, "__i"));
   _GLIBCXX_DEBUG_VERIFY(__i._M_attached_to(std::__addressof(__list)),
                         _M_message(__gnu_debug::__msg_splice_other)
                         ._M_iterator(__i, "__i")
                         ._M_sequence(__list, "__list"));
   _GLIBCXX_DEBUG_VERIFY(__list.get_allocator() == this->get_allocator(),
                         _M_message(__gnu_debug::__msg_splice_alloc)
                         ._M_sequence(*this)
                         ._M_sequence(__list, "__list"));

   // _GLIBCXX_RESOLVE_LIB_DEFECTS
   // 250. splicing invalidates iterators
   _Base_const_iterator __next = std::next(__i.base());
   this->_M_transfer_from_if(__list, [__next](_Base_const_iterator __it)
     { return __it == __next; });
   _Base::splice_after(__pos.base(), std::move(__list._M_base()),
                       __i.base());
      }

      void
      splice_after(const_iterator __pos, forward_list& __list,
              const_iterator __i)
      { splice_after(__pos, std::move(__list), __i); }

      void
      splice_after(const_iterator __pos, forward_list&& __list,
              const_iterator __before, const_iterator __last)
      {
   typename __gnu_debug::_Distance_traits<const_iterator>::__type __dist;
   auto __listptr = std::__addressof(__list);
   __glibcxx_check_insert_after(__pos);
   __glibcxx_check_valid_fl_range(__before, __last, __dist);
   _GLIBCXX_DEBUG_VERIFY(__before._M_attached_to(__listptr),
                         _M_message(__gnu_debug::__msg_splice_other)
                         ._M_sequence(__list, "list")
                         ._M_iterator(__before, "before"));
   _GLIBCXX_DEBUG_VERIFY(__before._M_dereferenceable()
                         || __before._M_is_before_begin(),
                         _M_message(__gnu_debug::__msg_valid_range2)
                         ._M_sequence(__list, "list")
                         ._M_iterator(__before, "before")
                         ._M_iterator(__last, "last"));
   _GLIBCXX_DEBUG_VERIFY(__before != __last,
                         _M_message(__gnu_debug::__msg_valid_range2)
                         ._M_sequence(__list, "list")
                         ._M_iterator(__before, "before")
                         ._M_iterator(__last, "last"));
   _GLIBCXX_DEBUG_VERIFY(__list.get_allocator() == this->get_allocator(),
                         _M_message(__gnu_debug::__msg_splice_alloc)
                         ._M_sequence(*this)
                         ._M_sequence(__list, "__list"));

   for (_Base_const_iterator __tmp = std::next(__before.base());
        __tmp != __last.base(); ++__tmp)
     {
       _GLIBCXX_DEBUG_VERIFY(__tmp != __list._M_base().end(),
                             _M_message(__gnu_debug::__msg_valid_range2)
                             ._M_sequence(__list, "list")
                             ._M_iterator(__before, "before")
                             ._M_iterator(__last, "last"));
       _GLIBCXX_DEBUG_VERIFY(__listptr != this || __tmp != __pos.base(),
                             _M_message(__gnu_debug::__msg_splice_overlap)
                             ._M_iterator(__tmp, "position")
                             ._M_iterator(__before, "before")
                             ._M_iterator(__last, "last"));
       // _GLIBCXX_RESOLVE_LIB_DEFECTS
       // 250. splicing invalidates iterators
       this->_M_transfer_from_if(__list, [__tmp](_Base_const_iterator __it)
         { return __it == __tmp; });
     }

   _Base::splice_after(__pos.base(), std::move(__list._M_base()),
                       __before.base(), __last.base());
      }

      void
      splice_after(const_iterator __pos, forward_list& __list,
              const_iterator __before, const_iterator __last)
      { splice_after(__pos, std::move(__list), __before, __last); }

      void
      remove(const _Tp& __val)
      {
   _Base_iterator __x = _Base::before_begin();
   _Base_iterator __old = __x++;
   while (__x != _Base::end())
     {
       if (*__x == __val)
         __x = _M_erase_after(__old);
       else
         __old = __x++;
     }
      }

      template<typename _Pred>
   void
   remove_if(_Pred __pred)
   {
     _Base_iterator __x = _Base::before_begin();
     _Base_iterator __old = __x++;
     while (__x != _Base::end())
       {
         if (__pred(*__x))
           __x = _M_erase_after(__old);
         else
           __old = __x++;
       }
   }

      void
      unique()
      {
   _Base_iterator __first = _Base::begin();
   _Base_iterator __last = _Base::end();
   if (__first == __last)
     return;
   _Base_iterator __next = std::next(__first);
   while (__next != __last)
     {
       if (*__first == *__next)
         __next = _M_erase_after(__first);
       else
         __first = __next++;
     }
      }

      template<typename _BinPred>
   void
   unique(_BinPred __binary_pred)
   {
     _Base_iterator __first = _Base::begin();
     _Base_iterator __last = _Base::end();
     if (__first == __last)
       return;
     _Base_iterator __next = std::next(__first);
     while (__next != __last)
       {
         if (__binary_pred(*__first, *__next))
           __next = _M_erase_after(__first);
         else
           __first = __next++;
       }
   }

      void
      merge(forward_list&& __list)
      {
   if (this != std::__addressof(__list))
   {
     __glibcxx_check_sorted(_Base::begin(), _Base::end());
     __glibcxx_check_sorted(__list._M_base().begin(),
                            __list._M_base().end());
     this->_M_transfer_from_if(__list, [&__list](_Base_const_iterator __it)
       {
         return __it != __list._M_base().cbefore_begin()
                && __it != __list._M_base().cend();
       });
     _Base::merge(std::move(__list._M_base()));
   }
      }

      void
      merge(forward_list& __list)
      { merge(std::move(__list)); }

      template<typename _Comp>
   void
   merge(forward_list&& __list, _Comp __comp)
   {
     if (this != std::__addressof(__list))
     {
       __glibcxx_check_sorted_pred(_Base::begin(), _Base::end(), __comp);
       __glibcxx_check_sorted_pred(__list._M_base().begin(),
                                   __list._M_base().end(), __comp);
       this->_M_transfer_from_if(__list,
                                 [&__list](_Base_const_iterator __it)
         {
           return __it != __list._M_base().cbefore_begin()
                  && __it != __list._M_base().cend();
         });
       _Base::merge(std::move(__list._M_base()), __comp);
     }
   }

      template<typename _Comp>
   void
   merge(forward_list& __list, _Comp __comp)
   { merge(std::move(__list), __comp); }

      using _Base::sort;
      using _Base::reverse;

      _Base&
      _M_base() noexcept { return *this; }

      const _Base&
      _M_base() const noexcept { return *this; }
    };

#if __cpp_deduction_guides >= 201606
  template<typename _InputIterator, typename _ValT
        = typename iterator_traits<_InputIterator>::value_type,
      typename _Allocator = allocator<_ValT>,
      typename = _RequireInputIter<_InputIterator>,
      typename = _RequireAllocator<_Allocator>>
    forward_list(_InputIterator, _InputIterator, _Allocator = _Allocator())
      -> forward_list<_ValT, _Allocator>;
#endif

  template<typename _Tp, typename _Alloc>
    bool
    operator==(const forward_list<_Tp, _Alloc>& __lx,
          const forward_list<_Tp, _Alloc>& __ly)
    { return __lx._M_base() == __ly._M_base(); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator<(const forward_list<_Tp, _Alloc>& __lx,
         const forward_list<_Tp, _Alloc>& __ly)
    { return __lx._M_base() < __ly._M_base(); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator!=(const forward_list<_Tp, _Alloc>& __lx,
          const forward_list<_Tp, _Alloc>& __ly)
    { return !(__lx == __ly); }

  /// Based on operator<
  template<typename _Tp, typename _Alloc>
    inline bool
    operator>(const forward_list<_Tp, _Alloc>& __lx,
         const forward_list<_Tp, _Alloc>& __ly)
    { return (__ly < __lx); }

  /// Based on operator<
  template<typename _Tp, typename _Alloc>
    inline bool
    operator>=(const forward_list<_Tp, _Alloc>& __lx,
          const forward_list<_Tp, _Alloc>& __ly)
    { return !(__lx < __ly); }

  /// Based on operator<
  template<typename _Tp, typename _Alloc>
    inline bool
    operator<=(const forward_list<_Tp, _Alloc>& __lx,
          const forward_list<_Tp, _Alloc>& __ly)
    { return !(__ly < __lx); }

  /// See std::forward_list::swap().
  template<typename _Tp, typename _Alloc>
    inline void
    swap(forward_list<_Tp, _Alloc>& __lx, forward_list<_Tp, _Alloc>& __ly)
    noexcept(noexcept(__lx.swap(__ly)))
    { __lx.swap(__ly); }

} // namespace __debug
} // namespace std

namespace __gnu_debug
{
  template<typename _Tp, typename _Alloc>
    struct _BeforeBeginHelper<std::__debug::forward_list<_Tp, _Alloc> >
    {
      typedef std::__debug::forward_list<_Tp, _Alloc> _Sequence;

      template<typename _Iterator>
   static bool
   _S_Is(const _Safe_iterator<_Iterator, _Sequence>& __it)
   {
     return
       __it.base() == __it._M_get_sequence()->_M_base().before_begin();
   }

      template<typename _Iterator>
   static bool
   _S_Is_Beginnest(const _Safe_iterator<_Iterator, _Sequence>& __it)
   { return _S_Is(__it); }
    };

  template<typename _Tp, typename _Alloc>
    struct _Sequence_traits<std::__debug::forward_list<_Tp, _Alloc> >
    {
      typedef typename std::__debug::forward_list<_Tp, _Alloc>::iterator _It;

      static typename _Distance_traits<_It>::__type
      _S_size(const std::__debug::forward_list<_Tp, _Alloc>& __seq)
      {
   return __seq.empty()
     ? std::make_pair(0, __dp_exact) : std::make_pair(1, __dp_equality);
      }
    };

#ifndef _GLIBCXX_DEBUG_PEDANTIC
  template<class _Tp, class _Alloc>
    struct _Insert_range_from_self_is_safe<
      std::__debug::forward_list<_Tp, _Alloc> >
    { enum { __value = 1 }; };
#endif
}

#endif
