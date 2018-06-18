// Debugging string implementation -*- C++ -*-

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

/** @file debug/string
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_STRING
#define _GLIBCXX_DEBUG_STRING 1

#pragma GCC system_header

#include <string>
#include <debug/safe_sequence.h>
#include <debug/safe_container.h>
#include <debug/safe_iterator.h>

namespace __gnu_debug
{
/// Class std::basic_string with safety/checking/debug instrumentation.
template<typename _CharT, typename _Traits = std::char_traits<_CharT>,
    typename _Allocator = std::allocator<_CharT> >
  class basic_string
  : public __gnu_debug::_Safe_container<
      basic_string<_CharT, _Traits, _Allocator>,
      _Allocator, _Safe_sequence, bool(_GLIBCXX_USE_CXX11_ABI)>,
    public std::basic_string<_CharT, _Traits, _Allocator>
  {
    typedef std::basic_string<_CharT, _Traits, _Allocator> _Base;
    typedef __gnu_debug::_Safe_container<
      basic_string, _Allocator, _Safe_sequence, bool(_GLIBCXX_USE_CXX11_ABI)>
      _Safe;

  public:
    // types:
    typedef _Traits                                        traits_type;
    typedef typename _Traits::char_type                    value_type;
    typedef _Allocator                                     allocator_type;
    typedef typename _Base::size_type                      size_type;
    typedef typename _Base::difference_type                difference_type;
    typedef typename _Base::reference                      reference;
    typedef typename _Base::const_reference                const_reference;
    typedef typename _Base::pointer                        pointer;
    typedef typename _Base::const_pointer          const_pointer;

    typedef __gnu_debug::_Safe_iterator<
      typename _Base::iterator, basic_string>              iterator;
    typedef __gnu_debug::_Safe_iterator<
      typename _Base::const_iterator, basic_string>        const_iterator;

    typedef std::reverse_iterator<iterator>                reverse_iterator;
    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;

    using _Base::npos;

    basic_string()
    _GLIBCXX_NOEXCEPT_IF(std::is_nothrow_default_constructible<_Base>::value)
    : _Base() { }

    // 21.3.1 construct/copy/destroy:
    explicit
    basic_string(const _Allocator& __a) _GLIBCXX_NOEXCEPT
    : _Base(__a) { }

#if __cplusplus < 201103L
    basic_string(const basic_string& __str)
    : _Base(__str) { }

    ~basic_string() { }
#else
    basic_string(const basic_string&) = default;
    basic_string(basic_string&&) = default;

    basic_string(std::initializer_list<_CharT> __l,
            const _Allocator& __a = _Allocator())
    : _Base(__l, __a)
    { }

#if _GLIBCXX_USE_CXX11_ABI
    basic_string(const basic_string& __s, const _Allocator& __a)
    : _Base(__s, __a) { }

    basic_string(basic_string&& __s, const _Allocator& __a)
    : _Base(std::move(__s), __a) { }
#endif

    ~basic_string() = default;

    // Provides conversion from a normal-mode string to a debug-mode string
    basic_string(_Base&& __base) noexcept
    : _Base(std::move(__base)) { }
#endif // C++11

    // Provides conversion from a normal-mode string to a debug-mode string
    basic_string(const _Base& __base)
    : _Base(__base) { }

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 42. string ctors specify wrong default allocator
    basic_string(const basic_string& __str, size_type __pos,
            size_type __n = _Base::npos,
            const _Allocator& __a = _Allocator())
    : _Base(__str, __pos, __n, __a) { }

    basic_string(const _CharT* __s, size_type __n,
              const _Allocator& __a = _Allocator())
    : _Base(__gnu_debug::__check_string(__s, __n), __n, __a) { }

    basic_string(const _CharT* __s, const _Allocator& __a = _Allocator())
    : _Base(__gnu_debug::__check_string(__s), __a)
    { this->assign(__s); }

    basic_string(size_type __n, _CharT __c,
              const _Allocator& __a = _Allocator())
    : _Base(__n, __c, __a) { }

    template<typename _InputIterator>
      basic_string(_InputIterator __begin, _InputIterator __end,
              const _Allocator& __a = _Allocator())
      : _Base(__gnu_debug::__base(__gnu_debug::__check_valid_range(__begin,
                                                              __end)),
         __gnu_debug::__base(__end), __a) { }

#if __cplusplus < 201103L
    basic_string&
    operator=(const basic_string& __str)
    {
      this->_M_safe() = __str;
      _M_base() = __str;
      return *this;
    }
#else
    basic_string&
    operator=(const basic_string&) = default;

    basic_string&
    operator=(basic_string&&) = default;
#endif

    basic_string&
    operator=(const _CharT* __s)
    {
      __glibcxx_check_string(__s);
      _M_base() = __s;
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    operator=(_CharT __c)
    {
      _M_base() = __c;
      this->_M_invalidate_all();
      return *this;
    }

#if __cplusplus >= 201103L
    basic_string&
    operator=(std::initializer_list<_CharT> __l)
    {
      _M_base() = __l;
      this->_M_invalidate_all();
      return *this;
    }
#endif // C++11

    // 21.3.2 iterators:
    iterator
    begin() // _GLIBCXX_NOEXCEPT
    { return iterator(_Base::begin(), this); }

    const_iterator
    begin() const _GLIBCXX_NOEXCEPT
    { return const_iterator(_Base::begin(), this); }

    iterator
    end() // _GLIBCXX_NOEXCEPT
    { return iterator(_Base::end(), this); }

    const_iterator
    end() const _GLIBCXX_NOEXCEPT
    { return const_iterator(_Base::end(), this); }

    reverse_iterator
    rbegin() // _GLIBCXX_NOEXCEPT
    { return reverse_iterator(end()); }

    const_reverse_iterator
    rbegin() const _GLIBCXX_NOEXCEPT
    { return const_reverse_iterator(end()); }

    reverse_iterator
    rend() // _GLIBCXX_NOEXCEPT
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

    // 21.3.3 capacity:
    using _Base::size;
    using _Base::length;
    using _Base::max_size;

    void
    resize(size_type __n, _CharT __c)
    {
      _Base::resize(__n, __c);
      this->_M_invalidate_all();
    }

    void
    resize(size_type __n)
    { this->resize(__n, _CharT()); }

#if __cplusplus >= 201103L
    void
    shrink_to_fit() noexcept
    {
      if (capacity() > size())
   {
     __try
       {
         reserve(0);
         this->_M_invalidate_all();
       }
     __catch(...)
       { }
   }
    }
#endif

    using _Base::capacity;
    using _Base::reserve;

    void
    clear() // _GLIBCXX_NOEXCEPT
    {
      _Base::clear();
      this->_M_invalidate_all();
    }

    using _Base::empty;

    // 21.3.4 element access:
    const_reference
    operator[](size_type __pos) const _GLIBCXX_NOEXCEPT
    {
      _GLIBCXX_DEBUG_VERIFY(__pos <= this->size(),
                       _M_message(__gnu_debug::__msg_subscript_oob)
                       ._M_sequence(*this, "this")
                       ._M_integer(__pos, "__pos")
                       ._M_integer(this->size(), "size"));
      return _M_base()[__pos];
    }

    reference
    operator[](size_type __pos) // _GLIBCXX_NOEXCEPT
    {
#if __cplusplus < 201103L && defined(_GLIBCXX_DEBUG_PEDANTIC)
      __glibcxx_check_subscript(__pos);
#else
      // as an extension v3 allows s[s.size()] when s is non-const.
      _GLIBCXX_DEBUG_VERIFY(__pos <= this->size(),
                       _M_message(__gnu_debug::__msg_subscript_oob)
                       ._M_sequence(*this, "this")
                       ._M_integer(__pos, "__pos")
                       ._M_integer(this->size(), "size"));
#endif
      return _M_base()[__pos];
    }

    using _Base::at;

#if __cplusplus >= 201103L
    using _Base::front;
    using _Base::back;
#endif

    // 21.3.5 modifiers:
    basic_string&
    operator+=(const basic_string& __str)
    {
      _M_base() += __str;
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    operator+=(const _CharT* __s)
    {
      __glibcxx_check_string(__s);
      _M_base() += __s;
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    operator+=(_CharT __c)
    {
      _M_base() += __c;
      this->_M_invalidate_all();
      return *this;
    }

#if __cplusplus >= 201103L
    basic_string&
    operator+=(std::initializer_list<_CharT> __l)
    {
      _M_base() += __l;
      this->_M_invalidate_all();
      return *this;
    }
#endif // C++11

    basic_string&
    append(const basic_string& __str)
    {
      _Base::append(__str);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    append(const basic_string& __str, size_type __pos, size_type __n)
    {
      _Base::append(__str, __pos, __n);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    append(const _CharT* __s, size_type __n)
    {
      __glibcxx_check_string_len(__s, __n);
      _Base::append(__s, __n);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    append(const _CharT* __s)
    {
      __glibcxx_check_string(__s);
      _Base::append(__s);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    append(size_type __n, _CharT __c)
    {
      _Base::append(__n, __c);
      this->_M_invalidate_all();
      return *this;
    }

    template<typename _InputIterator>
      basic_string&
      append(_InputIterator __first, _InputIterator __last)
      {
   typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
   __glibcxx_check_valid_range2(__first, __last, __dist);

   if (__dist.second >= __dp_sign)
     _Base::append(__gnu_debug::__unsafe(__first),
                   __gnu_debug::__unsafe(__last));
   else
     _Base::append(__first, __last);

   this->_M_invalidate_all();
   return *this;
      }

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 7. string clause minor problems
    void
    push_back(_CharT __c)
    {
      _Base::push_back(__c);
      this->_M_invalidate_all();
    }

    basic_string&
    assign(const basic_string& __x)
    {
      _Base::assign(__x);
      this->_M_invalidate_all();
      return *this;
    }

#if __cplusplus >= 201103L
    basic_string&
    assign(basic_string&& __x)
    noexcept(noexcept(std::declval<_Base&>().assign(std::move(__x))))
    {
      _Base::assign(std::move(__x));
      this->_M_invalidate_all();
      return *this;
    }
#endif // C++11

    basic_string&
    assign(const basic_string& __str, size_type __pos, size_type __n)
    {
      _Base::assign(__str, __pos, __n);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    assign(const _CharT* __s, size_type __n)
    {
      __glibcxx_check_string_len(__s, __n);
      _Base::assign(__s, __n);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    assign(const _CharT* __s)
    {
      __glibcxx_check_string(__s);
      _Base::assign(__s);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    assign(size_type __n, _CharT __c)
    {
      _Base::assign(__n, __c);
      this->_M_invalidate_all();
      return *this;
    }

    template<typename _InputIterator>
      basic_string&
      assign(_InputIterator __first, _InputIterator __last)
      {
   typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
   __glibcxx_check_valid_range2(__first, __last, __dist);

   if (__dist.second >= __dp_sign)
     _Base::assign(__gnu_debug::__unsafe(__first),
                   __gnu_debug::__unsafe(__last));
   else
     _Base::assign(__first, __last);

   this->_M_invalidate_all();
   return *this;
      }

#if __cplusplus >= 201103L
    basic_string&
    assign(std::initializer_list<_CharT> __l)
    {
      _Base::assign(__l);
      this->_M_invalidate_all();
      return *this;
    }
#endif // C++11

    basic_string&
    insert(size_type __pos1, const basic_string& __str)
    {
      _Base::insert(__pos1, __str);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    insert(size_type __pos1, const basic_string& __str,
      size_type __pos2, size_type __n)
    {
      _Base::insert(__pos1, __str, __pos2, __n);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    insert(size_type __pos, const _CharT* __s, size_type __n)
    {
      __glibcxx_check_string(__s);
      _Base::insert(__pos, __s, __n);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    insert(size_type __pos, const _CharT* __s)
    {
      __glibcxx_check_string(__s);
      _Base::insert(__pos, __s);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    insert(size_type __pos, size_type __n, _CharT __c)
    {
      _Base::insert(__pos, __n, __c);
      this->_M_invalidate_all();
      return *this;
    }

    iterator
    insert(iterator __p, _CharT __c)
    {
      __glibcxx_check_insert(__p);
      typename _Base::iterator __res = _Base::insert(__p.base(), __c);
      this->_M_invalidate_all();
      return iterator(__res, this);
    }

    void
    insert(iterator __p, size_type __n, _CharT __c)
    {
      __glibcxx_check_insert(__p);
      _Base::insert(__p.base(), __n, __c);
      this->_M_invalidate_all();
    }

    template<typename _InputIterator>
      void
      insert(iterator __p, _InputIterator __first, _InputIterator __last)
      {
   typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
   __glibcxx_check_insert_range2(__p, __first, __last, __dist);

   if (__dist.second >= __dp_sign)
     _Base::insert(__p.base(), __gnu_debug::__unsafe(__first),
                               __gnu_debug::__unsafe(__last));
   else
     _Base::insert(__p.base(), __first, __last);

   this->_M_invalidate_all();
      }

#if __cplusplus >= 201103L
    void
    insert(iterator __p, std::initializer_list<_CharT> __l)
    {
      __glibcxx_check_insert(__p);
      _Base::insert(__p.base(), __l);
      this->_M_invalidate_all();
    }
#endif // C++11

    basic_string&
    erase(size_type __pos = 0, size_type __n = _Base::npos)
    {
      _Base::erase(__pos, __n);
      this->_M_invalidate_all();
      return *this;
    }

    iterator
    erase(iterator __position)
    {
      __glibcxx_check_erase(__position);
      typename _Base::iterator __res = _Base::erase(__position.base());
      this->_M_invalidate_all();
      return iterator(__res, this);
    }

    iterator
    erase(iterator __first, iterator __last)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 151. can't currently clear() empty container
      __glibcxx_check_erase_range(__first, __last);
      typename _Base::iterator __res = _Base::erase(__first.base(),
                                               __last.base());
      this->_M_invalidate_all();
      return iterator(__res, this);
    }

#if __cplusplus >= 201103L
    void
    pop_back() // noexcept
    {
      __glibcxx_check_nonempty();
      _Base::pop_back();
      this->_M_invalidate_all();
    }
#endif // C++11

    basic_string&
    replace(size_type __pos1, size_type __n1, const basic_string& __str)
    {
      _Base::replace(__pos1, __n1, __str);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    replace(size_type __pos1, size_type __n1, const basic_string& __str,
       size_type __pos2, size_type __n2)
    {
      _Base::replace(__pos1, __n1, __str, __pos2, __n2);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    replace(size_type __pos, size_type __n1, const _CharT* __s,
       size_type __n2)
    {
      __glibcxx_check_string_len(__s, __n2);
      _Base::replace(__pos, __n1, __s, __n2);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    replace(size_type __pos, size_type __n1, const _CharT* __s)
    {
      __glibcxx_check_string(__s);
      _Base::replace(__pos, __n1, __s);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    replace(size_type __pos, size_type __n1, size_type __n2, _CharT __c)
    {
      _Base::replace(__pos, __n1, __n2, __c);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    replace(iterator __i1, iterator __i2, const basic_string& __str)
    {
      __glibcxx_check_erase_range(__i1, __i2);
      _Base::replace(__i1.base(), __i2.base(), __str);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    replace(iterator __i1, iterator __i2, const _CharT* __s, size_type __n)
    {
      __glibcxx_check_erase_range(__i1, __i2);
      __glibcxx_check_string_len(__s, __n);
      _Base::replace(__i1.base(), __i2.base(), __s, __n);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    replace(iterator __i1, iterator __i2, const _CharT* __s)
    {
      __glibcxx_check_erase_range(__i1, __i2);
      __glibcxx_check_string(__s);
      _Base::replace(__i1.base(), __i2.base(), __s);
      this->_M_invalidate_all();
      return *this;
    }

    basic_string&
    replace(iterator __i1, iterator __i2, size_type __n, _CharT __c)
    {
      __glibcxx_check_erase_range(__i1, __i2);
      _Base::replace(__i1.base(), __i2.base(), __n, __c);
      this->_M_invalidate_all();
      return *this;
    }

    template<typename _InputIterator>
      basic_string&
      replace(iterator __i1, iterator __i2,
         _InputIterator __j1, _InputIterator __j2)
      {
   __glibcxx_check_erase_range(__i1, __i2);

   typename __gnu_debug::_Distance_traits<_InputIterator>::__type __dist;
   __glibcxx_check_valid_range2(__j1, __j2, __dist);

   if (__dist.second >= __dp_sign)
     _Base::replace(__i1.base(), __i2.base(),
                    __gnu_debug::__unsafe(__j1),
                    __gnu_debug::__unsafe(__j2));
   else
     _Base::replace(__i1.base(), __i2.base(), __j1, __j2);

   this->_M_invalidate_all();
   return *this;
      }

#if __cplusplus >= 201103L
      basic_string& replace(iterator __i1, iterator __i2,
                       std::initializer_list<_CharT> __l)
      {
   __glibcxx_check_erase_range(__i1, __i2);
   _Base::replace(__i1.base(), __i2.base(), __l);
   this->_M_invalidate_all();
   return *this;
      }
#endif // C++11

    size_type
    copy(_CharT* __s, size_type __n, size_type __pos = 0) const
    {
      __glibcxx_check_string_len(__s, __n);
      return _Base::copy(__s, __n, __pos);
    }

    void
    swap(basic_string& __x)
    _GLIBCXX_NOEXCEPT_IF(std::__is_nothrow_swappable<_Base>::value)
    {
      _Safe::_M_swap(__x);
      _Base::swap(__x);
    }

    // 21.3.6 string operations:
    const _CharT*
    c_str() const _GLIBCXX_NOEXCEPT
    {
      const _CharT* __res = _Base::c_str();
      this->_M_invalidate_all();
      return __res;
    }

    const _CharT*
    data() const _GLIBCXX_NOEXCEPT
    {
      const _CharT* __res = _Base::data();
      this->_M_invalidate_all();
      return __res;
    }

    using _Base::get_allocator;

    size_type
    find(const basic_string& __str, size_type __pos = 0) const
      _GLIBCXX_NOEXCEPT
    { return _Base::find(__str, __pos); }

    size_type
    find(const _CharT* __s, size_type __pos, size_type __n) const
    {
      __glibcxx_check_string(__s);
      return _Base::find(__s, __pos, __n);
    }

    size_type
    find(const _CharT* __s, size_type __pos = 0) const
    {
      __glibcxx_check_string(__s);
      return _Base::find(__s, __pos);
    }

    size_type
    find(_CharT __c, size_type __pos = 0) const _GLIBCXX_NOEXCEPT
    { return _Base::find(__c, __pos); }

    size_type
    rfind(const basic_string& __str, size_type __pos = _Base::npos) const
      _GLIBCXX_NOEXCEPT
    { return _Base::rfind(__str, __pos); }

    size_type
    rfind(const _CharT* __s, size_type __pos, size_type __n) const
    {
      __glibcxx_check_string_len(__s, __n);
      return _Base::rfind(__s, __pos, __n);
    }

    size_type
    rfind(const _CharT* __s, size_type __pos = _Base::npos) const
    {
      __glibcxx_check_string(__s);
      return _Base::rfind(__s, __pos);
    }

    size_type
    rfind(_CharT __c, size_type __pos = _Base::npos) const _GLIBCXX_NOEXCEPT
    { return _Base::rfind(__c, __pos); }

    size_type
    find_first_of(const basic_string& __str, size_type __pos = 0) const
      _GLIBCXX_NOEXCEPT
    { return _Base::find_first_of(__str, __pos); }

    size_type
    find_first_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
      __glibcxx_check_string(__s);
      return _Base::find_first_of(__s, __pos, __n);
    }

    size_type
    find_first_of(const _CharT* __s, size_type __pos = 0) const
    {
      __glibcxx_check_string(__s);
      return _Base::find_first_of(__s, __pos);
    }

    size_type
    find_first_of(_CharT __c, size_type __pos = 0) const _GLIBCXX_NOEXCEPT
    { return _Base::find_first_of(__c, __pos); }

    size_type
    find_last_of(const basic_string& __str,
            size_type __pos = _Base::npos) const _GLIBCXX_NOEXCEPT
    { return _Base::find_last_of(__str, __pos); }

    size_type
    find_last_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
      __glibcxx_check_string(__s);
      return _Base::find_last_of(__s, __pos, __n);
    }

    size_type
    find_last_of(const _CharT* __s, size_type __pos = _Base::npos) const
    {
      __glibcxx_check_string(__s);
      return _Base::find_last_of(__s, __pos);
    }

    size_type
    find_last_of(_CharT __c, size_type __pos = _Base::npos) const
      _GLIBCXX_NOEXCEPT
    { return _Base::find_last_of(__c, __pos); }

    size_type
    find_first_not_of(const basic_string& __str, size_type __pos = 0) const
      _GLIBCXX_NOEXCEPT
    { return _Base::find_first_not_of(__str, __pos); }

    size_type
    find_first_not_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
      __glibcxx_check_string_len(__s, __n);
      return _Base::find_first_not_of(__s, __pos, __n);
    }

    size_type
    find_first_not_of(const _CharT* __s, size_type __pos = 0) const
    {
      __glibcxx_check_string(__s);
      return _Base::find_first_not_of(__s, __pos);
    }

    size_type
    find_first_not_of(_CharT __c, size_type __pos = 0) const _GLIBCXX_NOEXCEPT
    { return _Base::find_first_not_of(__c, __pos); }

    size_type
    find_last_not_of(const basic_string& __str,
                             size_type __pos = _Base::npos) const
      _GLIBCXX_NOEXCEPT
    { return _Base::find_last_not_of(__str, __pos); }

    size_type
    find_last_not_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
      __glibcxx_check_string(__s);
      return _Base::find_last_not_of(__s, __pos, __n);
    }

    size_type
    find_last_not_of(const _CharT* __s, size_type __pos = _Base::npos) const
    {
      __glibcxx_check_string(__s);
      return _Base::find_last_not_of(__s, __pos);
    }

    size_type
    find_last_not_of(_CharT __c, size_type __pos = _Base::npos) const
      _GLIBCXX_NOEXCEPT
    { return _Base::find_last_not_of(__c, __pos); }

    basic_string
    substr(size_type __pos = 0, size_type __n = _Base::npos) const
    { return basic_string(_Base::substr(__pos, __n)); }

    int
    compare(const basic_string& __str) const
    { return _Base::compare(__str); }

    int
    compare(size_type __pos1, size_type __n1,
             const basic_string& __str) const
    { return _Base::compare(__pos1, __n1, __str); }

    int
    compare(size_type __pos1, size_type __n1, const basic_string& __str,
         size_type __pos2, size_type __n2) const
    { return _Base::compare(__pos1, __n1, __str, __pos2, __n2); }

    int
    compare(const _CharT* __s) const
    {
      __glibcxx_check_string(__s);
      return _Base::compare(__s);
    }

    //  _GLIBCXX_RESOLVE_LIB_DEFECTS
    //  5. string::compare specification questionable
    int
    compare(size_type __pos1, size_type __n1, const _CharT* __s) const
    {
      __glibcxx_check_string(__s);
      return _Base::compare(__pos1, __n1, __s);
    }

    //  _GLIBCXX_RESOLVE_LIB_DEFECTS
    //  5. string::compare specification questionable
    int
    compare(size_type __pos1, size_type __n1,const _CharT* __s,
       size_type __n2) const
    {
      __glibcxx_check_string_len(__s, __n2);
      return _Base::compare(__pos1, __n1, __s, __n2);
    }

    _Base&
    _M_base() _GLIBCXX_NOEXCEPT            { return *this; }

    const _Base&
    _M_base() const _GLIBCXX_NOEXCEPT      { return *this; }

    using _Safe::_M_invalidate_all;
  };

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline basic_string<_CharT,_Traits,_Allocator>
    operator+(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
         const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { return basic_string<_CharT,_Traits,_Allocator>(__lhs) += __rhs; }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline basic_string<_CharT,_Traits,_Allocator>
    operator+(const _CharT* __lhs,
         const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    {
      __glibcxx_check_string(__lhs);
      return basic_string<_CharT,_Traits,_Allocator>(__lhs) += __rhs;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline basic_string<_CharT,_Traits,_Allocator>
    operator+(_CharT __lhs,
         const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { return basic_string<_CharT,_Traits,_Allocator>(1, __lhs) += __rhs; }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline basic_string<_CharT,_Traits,_Allocator>
    operator+(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
         const _CharT* __rhs)
    {
      __glibcxx_check_string(__rhs);
      return basic_string<_CharT,_Traits,_Allocator>(__lhs) += __rhs;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline basic_string<_CharT,_Traits,_Allocator>
    operator+(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
         _CharT __rhs)
    { return basic_string<_CharT,_Traits,_Allocator>(__lhs) += __rhs; }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator==(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
          const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { return __lhs._M_base() == __rhs._M_base(); }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator==(const _CharT* __lhs,
          const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    {
      __glibcxx_check_string(__lhs);
      return __lhs == __rhs._M_base();
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator==(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
          const _CharT* __rhs)
    {
      __glibcxx_check_string(__rhs);
      return __lhs._M_base() == __rhs;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator!=(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
          const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { return __lhs._M_base() != __rhs._M_base(); }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator!=(const _CharT* __lhs,
          const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    {
      __glibcxx_check_string(__lhs);
      return __lhs != __rhs._M_base();
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator!=(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
          const _CharT* __rhs)
    {
      __glibcxx_check_string(__rhs);
      return __lhs._M_base() != __rhs;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator<(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
         const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { return __lhs._M_base() < __rhs._M_base(); }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator<(const _CharT* __lhs,
         const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    {
      __glibcxx_check_string(__lhs);
      return __lhs < __rhs._M_base();
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator<(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
         const _CharT* __rhs)
    {
      __glibcxx_check_string(__rhs);
      return __lhs._M_base() < __rhs;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator<=(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
          const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { return __lhs._M_base() <= __rhs._M_base(); }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator<=(const _CharT* __lhs,
          const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    {
      __glibcxx_check_string(__lhs);
      return __lhs <= __rhs._M_base();
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator<=(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
          const _CharT* __rhs)
    {
      __glibcxx_check_string(__rhs);
      return __lhs._M_base() <= __rhs;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator>=(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
          const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { return __lhs._M_base() >= __rhs._M_base(); }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator>=(const _CharT* __lhs,
          const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    {
      __glibcxx_check_string(__lhs);
      return __lhs >= __rhs._M_base();
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator>=(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
          const _CharT* __rhs)
    {
      __glibcxx_check_string(__rhs);
      return __lhs._M_base() >= __rhs;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator>(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
         const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { return __lhs._M_base() > __rhs._M_base(); }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator>(const _CharT* __lhs,
         const basic_string<_CharT,_Traits,_Allocator>& __rhs)
    {
      __glibcxx_check_string(__lhs);
      return __lhs > __rhs._M_base();
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    inline bool
    operator>(const basic_string<_CharT,_Traits,_Allocator>& __lhs,
         const _CharT* __rhs)
    {
      __glibcxx_check_string(__rhs);
      return __lhs._M_base() > __rhs;
    }

  // 21.3.7.8:
  template<typename _CharT, typename _Traits, typename _Allocator>
    inline void
    swap(basic_string<_CharT,_Traits,_Allocator>& __lhs,
    basic_string<_CharT,_Traits,_Allocator>& __rhs)
    { __lhs.swap(__rhs); }

  template<typename _CharT, typename _Traits, typename _Allocator>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
          const basic_string<_CharT, _Traits, _Allocator>& __str)
    { return __os << __str._M_base(); }

  template<typename _CharT, typename _Traits, typename _Allocator>
    std::basic_istream<_CharT,_Traits>&
    operator>>(std::basic_istream<_CharT,_Traits>& __is,
          basic_string<_CharT,_Traits,_Allocator>& __str)
    {
      std::basic_istream<_CharT,_Traits>& __res = __is >> __str._M_base();
      __str._M_invalidate_all();
      return __res;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    std::basic_istream<_CharT,_Traits>&
    getline(std::basic_istream<_CharT,_Traits>& __is,
       basic_string<_CharT,_Traits,_Allocator>& __str, _CharT __delim)
    {
      std::basic_istream<_CharT,_Traits>& __res = getline(__is,
                                                     __str._M_base(),
                                                   __delim);
      __str._M_invalidate_all();
      return __res;
    }

  template<typename _CharT, typename _Traits, typename _Allocator>
    std::basic_istream<_CharT,_Traits>&
    getline(std::basic_istream<_CharT,_Traits>& __is,
       basic_string<_CharT,_Traits,_Allocator>& __str)
    {
      std::basic_istream<_CharT,_Traits>& __res = getline(__is,
                                                     __str._M_base());
      __str._M_invalidate_all();
      return __res;
    }

  typedef basic_string<char>    string;

#ifdef _GLIBCXX_USE_WCHAR_T
  typedef basic_string<wchar_t> wstring;
#endif

  template<typename _CharT, typename _Traits, typename _Allocator>
    struct _Insert_range_from_self_is_safe<
      __gnu_debug::basic_string<_CharT, _Traits, _Allocator> >
      { enum { __value = 1 }; };

} // namespace __gnu_debug

#endif
