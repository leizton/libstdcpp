// <system_error> -*- C++ -*-

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

/** @file include/system_error
 *  This is a Standard C++ Library header.
 */

#ifndef _GLIBCXX_SYSTEM_ERROR
#define _GLIBCXX_SYSTEM_ERROR 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#include <bits/c++config.h>
#include <bits/error_constants.h>
#include <iosfwd>
#include <stdexcept>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  class error_code;
  class error_condition;
  class system_error;

  /// is_error_code_enum
  template<typename _Tp>
    struct is_error_code_enum : public false_type { };

  /// is_error_condition_enum
  template<typename _Tp>
    struct is_error_condition_enum : public false_type { };

  template<>
    struct is_error_condition_enum<errc>
    : public true_type { };

#if __cplusplus > 201402L
  template <typename _Tp>
    inline constexpr bool is_error_code_enum_v =
      is_error_code_enum<_Tp>::value;
  template <typename _Tp>
    inline constexpr bool is_error_condition_enum_v =
      is_error_condition_enum<_Tp>::value;
#endif // C++17
  inline namespace _V2 {

  /// error_category
  class error_category
  {
  public:
    constexpr error_category() noexcept = default;

    virtual ~error_category();

    error_category(const error_category&) = delete;
    error_category& operator=(const error_category&) = delete;

    virtual const char*
    name() const noexcept = 0;

    // We need two different virtual functions here, one returning a
    // COW string and one returning an SSO string. Their positions in the
    // vtable must be consistent for dynamic dispatch to work, but which one
    // the name "message()" finds depends on which ABI the caller is using.
#if _GLIBCXX_USE_CXX11_ABI
  private:
    _GLIBCXX_DEFAULT_ABI_TAG
    virtual __cow_string
    _M_message(int) const;

  public:
    _GLIBCXX_DEFAULT_ABI_TAG
    virtual string
    message(int) const = 0;
#else
    virtual string
    message(int) const = 0;

  private:
    virtual __sso_string
    _M_message(int) const;
#endif

  public:
    virtual error_condition
    default_error_condition(int __i) const noexcept;

    virtual bool
    equivalent(int __i, const error_condition& __cond) const noexcept;

    virtual bool
    equivalent(const error_code& __code, int __i) const noexcept;

    bool
    operator<(const error_category& __other) const noexcept
    { return less<const error_category*>()(this, &__other); }

    bool
    operator==(const error_category& __other) const noexcept
    { return this == &__other; }

    bool
    operator!=(const error_category& __other) const noexcept
    { return this != &__other; }
  };

  // DR 890.
  _GLIBCXX_CONST const error_category& system_category() noexcept;
  _GLIBCXX_CONST const error_category& generic_category() noexcept;

  } // end inline namespace

  error_code make_error_code(errc) noexcept;

  template<typename _Tp>
    struct hash;

  /// error_code
  // Implementation-specific error identification
  struct error_code
  {
    error_code() noexcept
    : _M_value(0), _M_cat(&system_category()) { }

    error_code(int __v, const error_category& __cat) noexcept
    : _M_value(__v), _M_cat(&__cat) { }

    template<typename _ErrorCodeEnum, typename = typename
        enable_if<is_error_code_enum<_ErrorCodeEnum>::value>::type>
      error_code(_ErrorCodeEnum __e) noexcept
      { *this = make_error_code(__e); }

    void
    assign(int __v, const error_category& __cat) noexcept
    {
      _M_value = __v;
      _M_cat = &__cat;
    }

    void
    clear() noexcept
    { assign(0, system_category()); }

    // DR 804.
    template<typename _ErrorCodeEnum>
      typename enable_if<is_error_code_enum<_ErrorCodeEnum>::value,
                    error_code&>::type
      operator=(_ErrorCodeEnum __e) noexcept
      { return *this = make_error_code(__e); }

    int
    value() const noexcept { return _M_value; }

    const error_category&
    category() const noexcept { return *_M_cat; }

    error_condition
    default_error_condition() const noexcept;

    _GLIBCXX_DEFAULT_ABI_TAG
    string
    message() const
    { return category().message(value()); }

    explicit operator bool() const noexcept
    { return _M_value != 0; }

    // DR 804.
  private:
    friend class hash<error_code>;

    int                            _M_value;
    const error_category*  _M_cat;
  };

  // 19.4.2.6 non-member functions
  inline error_code
  make_error_code(errc __e) noexcept
  { return error_code(static_cast<int>(__e), generic_category()); }

  inline bool
  operator<(const error_code& __lhs, const error_code& __rhs) noexcept
  {
    return (__lhs.category() < __rhs.category()
       || (__lhs.category() == __rhs.category()
           && __lhs.value() < __rhs.value()));
  }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __e)
    { return (__os << __e.category().name() << ':' << __e.value()); }

  error_condition make_error_condition(errc) noexcept;

  /// error_condition
  // Portable error identification
  struct error_condition
  {
    error_condition() noexcept
    : _M_value(0), _M_cat(&generic_category()) { }

    error_condition(int __v, const error_category& __cat) noexcept
    : _M_value(__v), _M_cat(&__cat) { }

    template<typename _ErrorConditionEnum, typename = typename
    enable_if<is_error_condition_enum<_ErrorConditionEnum>::value>::type>
      error_condition(_ErrorConditionEnum __e) noexcept
      { *this = make_error_condition(__e); }

    void
    assign(int __v, const error_category& __cat) noexcept
    {
      _M_value = __v;
      _M_cat = &__cat;
    }

    // DR 804.
    template<typename _ErrorConditionEnum>
      typename enable_if<is_error_condition_enum
                    <_ErrorConditionEnum>::value, error_condition&>::type
      operator=(_ErrorConditionEnum __e) noexcept
      { return *this = make_error_condition(__e); }

    void
    clear() noexcept
    { assign(0, generic_category()); }

    // 19.4.3.4 observers
    int
    value() const noexcept { return _M_value; }

    const error_category&
    category() const noexcept { return *_M_cat; }

    _GLIBCXX_DEFAULT_ABI_TAG
    string
    message() const
    { return category().message(value()); }

    explicit operator bool() const noexcept
    { return _M_value != 0; }

    // DR 804.
  private:
    int                    _M_value;
    const error_category*  _M_cat;
  };

  // 19.4.3.6 non-member functions
  inline error_condition
  make_error_condition(errc __e) noexcept
  { return error_condition(static_cast<int>(__e), generic_category()); }

  inline bool
  operator<(const error_condition& __lhs,
       const error_condition& __rhs) noexcept
  {
    return (__lhs.category() < __rhs.category()
       || (__lhs.category() == __rhs.category()
           && __lhs.value() < __rhs.value()));
  }

  // 19.4.4 Comparison operators
  inline bool
  operator==(const error_code& __lhs, const error_code& __rhs) noexcept
  { return (__lhs.category() == __rhs.category()
       && __lhs.value() == __rhs.value()); }

  inline bool
  operator==(const error_code& __lhs, const error_condition& __rhs) noexcept
  {
    return (__lhs.category().equivalent(__lhs.value(), __rhs)
       || __rhs.category().equivalent(__lhs, __rhs.value()));
  }

  inline bool
  operator==(const error_condition& __lhs, const error_code& __rhs) noexcept
  {
    return (__rhs.category().equivalent(__rhs.value(), __lhs)
       || __lhs.category().equivalent(__rhs, __lhs.value()));
  }

  inline bool
  operator==(const error_condition& __lhs,
        const error_condition& __rhs) noexcept
  {
    return (__lhs.category() == __rhs.category()
       && __lhs.value() == __rhs.value());
  }

  inline bool
  operator!=(const error_code& __lhs, const error_code& __rhs) noexcept
  { return !(__lhs == __rhs); }

  inline bool
  operator!=(const error_code& __lhs, const error_condition& __rhs) noexcept
  { return !(__lhs == __rhs); }

  inline bool
  operator!=(const error_condition& __lhs, const error_code& __rhs) noexcept
  { return !(__lhs == __rhs); }

  inline bool
  operator!=(const error_condition& __lhs,
        const error_condition& __rhs) noexcept
  { return !(__lhs == __rhs); }


  /**
   *  @brief Thrown to indicate error code of underlying system.
   *
   *  @ingroup exceptions
   */
  class system_error : public std::runtime_error
  {
  private:
    error_code     _M_code;

  public:
    system_error(error_code __ec = error_code())
    : runtime_error(__ec.message()), _M_code(__ec) { }

    system_error(error_code __ec, const string& __what)
    : runtime_error(__what + ": " + __ec.message()), _M_code(__ec) { }

    system_error(error_code __ec, const char* __what)
    : runtime_error(__what + (": " + __ec.message())), _M_code(__ec) { }

    system_error(int __v, const error_category& __ecat, const char* __what)
    : system_error(error_code(__v, __ecat), __what) { }

    system_error(int __v, const error_category& __ecat)
    : runtime_error(error_code(__v, __ecat).message()),
      _M_code(__v, __ecat) { }

    system_error(int __v, const error_category& __ecat, const string& __what)
    : runtime_error(__what + ": " + error_code(__v, __ecat).message()),
      _M_code(__v, __ecat) { }

    virtual ~system_error() noexcept;

    const error_code&
    code() const noexcept { return _M_code; }
  };

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#include <bits/functional_hash.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#ifndef _GLIBCXX_COMPATIBILITY_CXX0X
  // DR 1182.
  /// std::hash specialization for error_code.
  template<>
    struct hash<error_code>
    : public __hash_base<size_t, error_code>
    {
      size_t
      operator()(const error_code& __e) const noexcept
      {
   const size_t __tmp = std::_Hash_impl::hash(__e._M_value);
   return std::_Hash_impl::__hash_combine(__e._M_cat, __tmp);
      }
    };
#endif // _GLIBCXX_COMPATIBILITY_CXX0X

#if __cplusplus > 201402L
  // DR 2686.
  /// std::hash specialization for error_condition.
  template<>
    struct hash<error_condition>
    : public __hash_base<size_t, error_condition>
    {
      size_t
      operator()(const error_condition& __e) const noexcept
      {
   const size_t __tmp = std::_Hash_impl::hash(__e.value());
   return std::_Hash_impl::__hash_combine(__e.category(), __tmp);
      }
    };
#endif

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif // C++11

#endif // _GLIBCXX_SYSTEM_ERROR
