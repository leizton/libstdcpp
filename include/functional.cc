// <functional> -*- C++ -*-

// Copyright (C) 2001-2018 Free Software Foundation, Inc.
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
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/** @file include/functional
 *  This is a Standard C++ Library header.
 */

#ifndef _GLIBCXX_FUNCTIONAL
#define _GLIBCXX_FUNCTIONAL 1

#pragma GCC system_header

#include <bits/c++config.h>
#include <bits/stl_function.h>

#if __cplusplus >= 201103L

#include <new>
#include <tuple>
#include <type_traits>
#include <bits/functional_hash.h>
#include <bits/invoke.h>
#include <bits/refwrap.h>      // std::reference_wrapper and _Mem_fn_traits
#include <bits/std_function.h> // std::function
#if __cplusplus > 201402L
#include <unordered_map>
#include <vector>
#include <array>
#include <utility>
#include <bits/stl_algo.h>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
  _GLIBCXX_BEGIN_NAMESPACE_VERSION

#if __cplusplus > 201402L
#define __cpp_lib_invoke 201411

  /// Invoke a callable object.
  template <typename _Callable, typename... _Args>
  inline invoke_result_t<_Callable, _Args...>
  invoke(_Callable && __fn, _Args && ... __args) noexcept(is_nothrow_invocable_v<_Callable, _Args...>)
  {
    return std::__invoke(std::forward<_Callable>(__fn),
                         std::forward<_Args>(__args)...);
  }
#endif

  template <typename _MemFunPtr,
            bool __is_mem_fn = is_member_function_pointer<_MemFunPtr>::value>
  class _Mem_fn_base
      : public _Mem_fn_traits<_MemFunPtr>::__maybe_type
  {
    using _Traits = _Mem_fn_traits<_MemFunPtr>;

    using _Arity = typename _Traits::__arity;
    using _Varargs = typename _Traits::__vararg;

    template <typename _Func, typename... _BoundArgs>
    friend struct _Bind_check_arity;

    _MemFunPtr _M_pmf;

  public:
    using result_type = typename _Traits::__result_type;

    explicit constexpr _Mem_fn_base(_MemFunPtr __pmf) noexcept : _M_pmf(__pmf) {}

    template <typename... _Args>
    auto
    operator()(_Args &&... __args) const
        noexcept(noexcept(
            std::__invoke(_M_pmf, std::forward<_Args>(__args)...)))
            -> decltype(std::__invoke(_M_pmf, std::forward<_Args>(__args)...))
    {
      return std::__invoke(_M_pmf, std::forward<_Args>(__args)...);
    }
  };

  // Partial specialization for member object pointers.
  template <typename _MemObjPtr>
  class _Mem_fn_base<_MemObjPtr, false>
  {
    using _Arity = integral_constant<size_t, 0>;
    using _Varargs = false_type;

    template <typename _Func, typename... _BoundArgs>
    friend struct _Bind_check_arity;

    _MemObjPtr _M_pm;

  public:
    explicit constexpr _Mem_fn_base(_MemObjPtr __pm) noexcept : _M_pm(__pm) {}

    template <typename _Tp>
    auto
    operator()(_Tp &&__obj) const
        noexcept(noexcept(std::__invoke(_M_pm, std::forward<_Tp>(__obj))))
            -> decltype(std::__invoke(_M_pm, std::forward<_Tp>(__obj)))
    {
      return std::__invoke(_M_pm, std::forward<_Tp>(__obj));
    }
  };

  template <typename _MemberPointer>
  struct _Mem_fn; // undefined

  template <typename _Res, typename _Class>
  struct _Mem_fn<_Res _Class::*>
      : _Mem_fn_base<_Res _Class::*>
  {
    using _Mem_fn_base<_Res _Class::*>::_Mem_fn_base;
  };

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2048.  Unnecessary mem_fn overloads
  /**
   *  @brief Returns a function object that forwards to the member
   *  pointer @a pm.
   *  @ingroup functors
   */
  template <typename _Tp, typename _Class>
  inline _Mem_fn<_Tp _Class::*>
  mem_fn(_Tp _Class::*__pm) noexcept
  {
    return _Mem_fn<_Tp _Class::*>(__pm);
  }

  /**
   *  @brief Determines if the given type _Tp is a function object that
   *  should be treated as a subexpression when evaluating calls to
   *  function objects returned by bind().
   *
   *  C++11 [func.bind.isbind].
   *  @ingroup binders
   */
  template <typename _Tp>
  struct is_bind_expression
      : public false_type
  {
  };

  /**
   *  @brief Determines if the given type _Tp is a placeholder in a
   *  bind() expression and, if so, which placeholder it is.
   *
   *  C++11 [func.bind.isplace].
   *  @ingroup binders
   */
  template <typename _Tp>
  struct is_placeholder
      : public integral_constant<int, 0>
  {
  };

#if __cplusplus > 201402L
  template <typename _Tp>
  inline constexpr bool is_bind_expression_v = is_bind_expression<_Tp>::value;
  template <typename _Tp>
  inline constexpr int is_placeholder_v = is_placeholder<_Tp>::value;
#endif // C++17

  /** @brief The type of placeholder objects defined by libstdc++.
   *  @ingroup binders
   */
  template <int _Num>
  struct _Placeholder
  {
  };

  /** @namespace std::placeholders
   *  @brief ISO C++11 entities sub-namespace for functional.
   *  @ingroup binders
   */
  namespace placeholders
  {
  /* Define a large number of placeholders. There is no way to
   * simplify this with variadic templates, because we're introducing
   * unique names for each.
   */
  extern const _Placeholder<1> _1;
  extern const _Placeholder<2> _2;
  extern const _Placeholder<3> _3;
  extern const _Placeholder<4> _4;
  extern const _Placeholder<5> _5;
  extern const _Placeholder<6> _6;
  extern const _Placeholder<7> _7;
  extern const _Placeholder<8> _8;
  extern const _Placeholder<9> _9;
  extern const _Placeholder<10> _10;
  extern const _Placeholder<11> _11;
  extern const _Placeholder<12> _12;
  extern const _Placeholder<13> _13;
  extern const _Placeholder<14> _14;
  extern const _Placeholder<15> _15;
  extern const _Placeholder<16> _16;
  extern const _Placeholder<17> _17;
  extern const _Placeholder<18> _18;
  extern const _Placeholder<19> _19;
  extern const _Placeholder<20> _20;
  extern const _Placeholder<21> _21;
  extern const _Placeholder<22> _22;
  extern const _Placeholder<23> _23;
  extern const _Placeholder<24> _24;
  extern const _Placeholder<25> _25;
  extern const _Placeholder<26> _26;
  extern const _Placeholder<27> _27;
  extern const _Placeholder<28> _28;
  extern const _Placeholder<29> _29;
  } // namespace placeholders

  /**
   *  Partial specialization of is_placeholder that provides the placeholder
   *  number for the placeholder objects defined by libstdc++.
   *  @ingroup binders
   */
  template <int _Num>
  struct is_placeholder<_Placeholder<_Num>>
      : public integral_constant<int, _Num>
  {
  };

  template <int _Num>
  struct is_placeholder<const _Placeholder<_Num>>
      : public integral_constant<int, _Num>
  {
  };

  // Like tuple_element_t but SFINAE-friendly.
  template <std::size_t __i, typename _Tuple>
  using _Safe_tuple_element_t = typename enable_if<(__i < tuple_size<_Tuple>::value),
                                                   tuple_element<__i, _Tuple>>::type::type;

  /**
   *  Maps an argument to bind() into an actual argument to the bound
   *  function object [func.bind.bind]/10. Only the first parameter should
   *  be specified: the rest are used to determine among the various
   *  implementations. Note that, although this class is a function
   *  object, it isn't entirely normal because it takes only two
   *  parameters regardless of the number of parameters passed to the
   *  bind expression. The first parameter is the bound argument and
   *  the second parameter is a tuple containing references to the
   *  rest of the arguments.
   */
  template <typename _Arg,
            bool _IsBindExp = is_bind_expression<_Arg>::value,
            bool _IsPlaceholder = (is_placeholder<_Arg>::value > 0)>
  class _Mu;

  /**
   *  If the argument is reference_wrapper<_Tp>, returns the
   *  underlying reference.
   *  C++11 [func.bind.bind] p10 bullet 1.
   */
  template <typename _Tp>
  class _Mu<reference_wrapper<_Tp>, false, false>
  {
  public:
    /* Note: This won't actually work for const volatile
       * reference_wrappers, because reference_wrapper::get() is const
       * but not volatile-qualified. This might be a defect in the TR.
       */
    template <typename _CVRef, typename _Tuple>
    _Tp &
    operator()(_CVRef &__arg, _Tuple &) const volatile
    {
      return __arg.get();
    }
  };

  /**
   *  If the argument is a bind expression, we invoke the underlying
   *  function object with the same cv-qualifiers as we are given and
   *  pass along all of our arguments (unwrapped).
   *  C++11 [func.bind.bind] p10 bullet 2.
   */
  template <typename _Arg>
  class _Mu<_Arg, true, false>
  {
  public:
    template <typename _CVArg, typename... _Args>
    auto
    operator()(_CVArg &__arg,
               tuple<_Args...> &__tuple) const volatile
        -> decltype(__arg(declval<_Args>()...))
    {
      // Construct an index tuple and forward to __call
      typedef typename _Build_index_tuple<sizeof...(_Args)>::__type
          _Indexes;
      return this->__call(__arg, __tuple, _Indexes());
    }

  private:
    // Invokes the underlying function object __arg by unpacking all
    // of the arguments in the tuple.
    template <typename _CVArg, typename... _Args, std::size_t... _Indexes>
    auto
    __call(_CVArg &__arg, tuple<_Args...> &__tuple,
           const _Index_tuple<_Indexes...> &) const volatile
        -> decltype(__arg(declval<_Args>()...))
    {
      return __arg(std::get<_Indexes>(std::move(__tuple))...);
    }
  };

  /**
   *  If the argument is a placeholder for the Nth argument, returns
   *  a reference to the Nth argument to the bind function object.
   *  C++11 [func.bind.bind] p10 bullet 3.
   */
  template <typename _Arg>
  class _Mu<_Arg, false, true>
  {
  public:
    template <typename _Tuple>
    _Safe_tuple_element_t<(is_placeholder<_Arg>::value - 1), _Tuple> &&
    operator()(const volatile _Arg &, _Tuple &__tuple) const volatile
    {
      return ::std::get<(is_placeholder<_Arg>::value - 1)>(std::move(__tuple));
    }
  };

  /**
   *  If the argument is just a value, returns a reference to that
   *  value. The cv-qualifiers on the reference are determined by the caller.
   *  C++11 [func.bind.bind] p10 bullet 4.
   */
  template <typename _Arg>
  class _Mu<_Arg, false, false>
  {
  public:
    template <typename _CVArg, typename _Tuple>
    _CVArg &&
    operator()(_CVArg &&__arg, _Tuple &) const volatile
    {
      return std::forward<_CVArg>(__arg);
    }
  };

  // std::get<I> for volatile-qualified tuples
  template <std::size_t _Ind, typename... _Tp>
  inline auto
  __volget(volatile tuple<_Tp...> & __tuple)
      ->__tuple_element_t<_Ind, tuple<_Tp...>> volatile &
  {
    return std::get<_Ind>(const_cast<tuple<_Tp...> &>(__tuple));
  }

  // std::get<I> for const-volatile-qualified tuples
  template <std::size_t _Ind, typename... _Tp>
  inline auto
  __volget(const volatile tuple<_Tp...> &__tuple)
      ->__tuple_element_t<_Ind, tuple<_Tp...>> const volatile &
  {
    return std::get<_Ind>(const_cast<const tuple<_Tp...> &>(__tuple));
  }

  /// Type of the function object returned from bind().
  template <typename _Signature>
  struct _Bind;

  template <typename _Functor, typename... _Bound_args>
  class _Bind<_Functor(_Bound_args...)> : public _Weak_result_type<_Functor>
  {
    typedef typename _Build_index_tuple<sizeof...(_Bound_args)>::__type _Bound_indexes;

    _Functor _M_f;
    tuple<_Bound_args...> _M_bound_args;

    // Call unqualified
    template <typename _Result, typename... _Args, std::size_t... _Indexes>
    _Result
    __call(tuple<_Args...> &&__args, _Index_tuple<_Indexes...>)
    {
      return std::__invoke(_M_f,
                           _Mu<_Bound_args>()(std::get<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as const
    template <typename _Result, typename... _Args, std::size_t... _Indexes>
    _Result
    __call_c(tuple<_Args...> &&__args, _Index_tuple<_Indexes...>) const
    {
      return std::__invoke(_M_f,
                           _Mu<_Bound_args>()(std::get<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as volatile
    template <typename _Result, typename... _Args, std::size_t... _Indexes>
    _Result
    __call_v(tuple<_Args...> &&__args,
             _Index_tuple<_Indexes...>) volatile
    {
      return std::__invoke(_M_f,
                           _Mu<_Bound_args>()(__volget<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as const volatile
    template <typename _Result, typename... _Args, std::size_t... _Indexes>
    _Result
    __call_c_v(tuple<_Args...> &&__args,
               _Index_tuple<_Indexes...>) const volatile
    {
      return std::__invoke(_M_f,
                           _Mu<_Bound_args>()(__volget<_Indexes>(_M_bound_args), __args)...);
    }

    template <typename _BoundArg, typename _CallArgs>
    using _Mu_type = decltype(
        _Mu<typename remove_cv<_BoundArg>::type>()(
            std::declval<_BoundArg &>(), std::declval<_CallArgs &>()));

    template <typename _Fn, typename _CallArgs, typename... _BArgs>
    using _Res_type_impl = typename result_of<_Fn &(_Mu_type<_BArgs, _CallArgs> &&...)>::type;

    template <typename _CallArgs>
    using _Res_type = _Res_type_impl<_Functor, _CallArgs, _Bound_args...>;

    template <typename _CallArgs>
    using __dependent = typename enable_if<bool(tuple_size<_CallArgs>::value + 1), _Functor>::type;

    template <typename _CallArgs, template <class> class __cv_quals>
    using _Res_type_cv = _Res_type_impl<
        typename __cv_quals<__dependent<_CallArgs>>::type,
        _CallArgs,
        typename __cv_quals<_Bound_args>::type...>;

  public:
    template <typename... _Args>
    explicit _Bind(const _Functor &__f, _Args &&... __args)
        : _M_f(__f), _M_bound_args(std::forward<_Args>(__args)...)
    {
    }

    template <typename... _Args>
    explicit _Bind(_Functor &&__f, _Args &&... __args)
        : _M_f(std::move(__f)), _M_bound_args(std::forward<_Args>(__args)...)
    {
    }

    _Bind(const _Bind &) = default;

    _Bind(_Bind &&__b)
        : _M_f(std::move(__b._M_f)), _M_bound_args(std::move(__b._M_bound_args))
    {
    }

    // Call unqualified
    template <typename... _Args,
              typename _Result = _Res_type<tuple<_Args...>>>
    _Result
    operator()(_Args &&... __args)
    {
      return this->__call<_Result>(
          std::forward_as_tuple(std::forward<_Args>(__args)...),
          _Bound_indexes());
    }

    // Call as const
    template <typename... _Args,
              typename _Result = _Res_type_cv<tuple<_Args...>, add_const>>
    _Result
    operator()(_Args &&... __args) const
    {
      return this->__call_c<_Result>(
          std::forward_as_tuple(std::forward<_Args>(__args)...),
          _Bound_indexes());
    }

#if __cplusplus > 201402L
#define _GLIBCXX_DEPR_BIND \ \
    [[deprecated("std::bind does not support volatile in C++17")]]
#else
#define _GLIBCXX_DEPR_BIND
#endif
    // Call as volatile
    template <typename... _Args,
              typename _Result = _Res_type_cv<tuple<_Args...>, add_volatile>>
    _GLIBCXX_DEPR_BIND
        _Result
        operator()(_Args &&... __args) volatile
    {
      return this->__call_v<_Result>(
          std::forward_as_tuple(std::forward<_Args>(__args)...),
          _Bound_indexes());
    }

    // Call as const volatile
    template <typename... _Args,
              typename _Result = _Res_type_cv<tuple<_Args...>, add_cv>>
    _GLIBCXX_DEPR_BIND
        _Result
        operator()(_Args &&... __args) const volatile
    {
      return this->__call_c_v<_Result>(
          std::forward_as_tuple(std::forward<_Args>(__args)...),
          _Bound_indexes());
    }
  };

  /// Type of the function object returned from bind<R>().
  template <typename _Result, typename _Signature>
  struct _Bind_result;

  template <typename _Result, typename _Functor, typename... _Bound_args>
  class _Bind_result<_Result, _Functor(_Bound_args...)>
  {
    typedef typename _Build_index_tuple<sizeof...(_Bound_args)>::__type
        _Bound_indexes;

    _Functor _M_f;
    tuple<_Bound_args...> _M_bound_args;

    // sfinae types
    template <typename _Res>
    using __enable_if_void = typename enable_if<is_void<_Res>{}>::type;

    template <typename _Res>
    using __disable_if_void = typename enable_if<!is_void<_Res>{}, _Result>::type;

    // Call unqualified
    template <typename _Res, typename... _Args, std::size_t... _Indexes>
    __disable_if_void<_Res>
    __call(tuple<_Args...> &&__args, _Index_tuple<_Indexes...>)
    {
      return std::__invoke(_M_f, _Mu<_Bound_args>()(std::get<_Indexes>(_M_bound_args), __args)...);
    }

    // Call unqualified, return void
    template <typename _Res, typename... _Args, std::size_t... _Indexes>
    __enable_if_void<_Res>
    __call(tuple<_Args...> &&__args, _Index_tuple<_Indexes...>)
    {
      std::__invoke(_M_f, _Mu<_Bound_args>()(std::get<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as const
    template <typename _Res, typename... _Args, std::size_t... _Indexes>
    __disable_if_void<_Res>
    __call(tuple<_Args...> &&__args, _Index_tuple<_Indexes...>) const
    {
      return std::__invoke(_M_f, _Mu<_Bound_args>()(std::get<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as const, return void
    template <typename _Res, typename... _Args, std::size_t... _Indexes>
    __enable_if_void<_Res>
    __call(tuple<_Args...> &&__args, _Index_tuple<_Indexes...>) const
    {
      std::__invoke(_M_f, _Mu<_Bound_args>()(std::get<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as volatile
    template <typename _Res, typename... _Args, std::size_t... _Indexes>
    __disable_if_void<_Res>
    __call(tuple<_Args...> &&__args, _Index_tuple<_Indexes...>) volatile
    {
      return std::__invoke(_M_f, _Mu<_Bound_args>()(__volget<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as volatile, return void
    template <typename _Res, typename... _Args, std::size_t... _Indexes>
    __enable_if_void<_Res>
    __call(tuple<_Args...> &&__args, _Index_tuple<_Indexes...>) volatile
    {
      std::__invoke(_M_f, _Mu<_Bound_args>()(__volget<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as const volatile
    template <typename _Res, typename... _Args, std::size_t... _Indexes>
    __disable_if_void<_Res>
    __call(tuple<_Args...> &&__args,
           _Index_tuple<_Indexes...>) const volatile
    {
      return std::__invoke(_M_f, _Mu<_Bound_args>()(__volget<_Indexes>(_M_bound_args), __args)...);
    }

    // Call as const volatile, return void
    template <typename _Res, typename... _Args, std::size_t... _Indexes>
    __enable_if_void<_Res>
    __call(tuple<_Args...> &&__args,
           _Index_tuple<_Indexes...>) const volatile
    {
      std::__invoke(_M_f, _Mu<_Bound_args>()(__volget<_Indexes>(_M_bound_args), __args)...);
    }

  public:
    typedef _Result result_type;

    template <typename... _Args>
    explicit _Bind_result(const _Functor &__f, _Args &&... __args)
        : _M_f(__f), _M_bound_args(std::forward<_Args>(__args)...)
    {
    }

    template <typename... _Args>
    explicit _Bind_result(_Functor &&__f, _Args &&... __args)
        : _M_f(std::move(__f)), _M_bound_args(std::forward<_Args>(__args)...)
    {
    }

    _Bind_result(const _Bind_result &) = default;

    _Bind_result(_Bind_result &&__b)
        : _M_f(std::move(__b._M_f)), _M_bound_args(std::move(__b._M_bound_args))
    {
    }

    // Call unqualified
    template <typename... _Args>
    result_type
    operator()(_Args &&... __args)
    {
      return this->__call<_Result>(
          std::forward_as_tuple(std::forward<_Args>(__args)...),
          _Bound_indexes());
    }

    // Call as const
    template <typename... _Args>
    result_type
    operator()(_Args &&... __args) const
    {
      return this->__call<_Result>(
          std::forward_as_tuple(std::forward<_Args>(__args)...),
          _Bound_indexes());
    }

    // Call as volatile
    template <typename... _Args>
    _GLIBCXX_DEPR_BIND
        result_type
        operator()(_Args &&... __args) volatile
    {
      return this->__call<_Result>(
          std::forward_as_tuple(std::forward<_Args>(__args)...),
          _Bound_indexes());
    }

    // Call as const volatile
    template <typename... _Args>
    _GLIBCXX_DEPR_BIND
        result_type
        operator()(_Args &&... __args) const volatile
    {
      return this->__call<_Result>(
          std::forward_as_tuple(std::forward<_Args>(__args)...),
          _Bound_indexes());
    }
  };
#undef _GLIBCXX_DEPR_BIND

  /**
   *  @brief Class template _Bind is always a bind expression.
   *  @ingroup binders
   */
  template <typename _Signature>
  struct is_bind_expression<_Bind<_Signature>>
      : public true_type
  {
  };

  /**
   *  @brief Class template _Bind is always a bind expression.
   *  @ingroup binders
   */
  template <typename _Signature>
  struct is_bind_expression<const _Bind<_Signature>>
      : public true_type
  {
  };

  /**
   *  @brief Class template _Bind is always a bind expression.
   *  @ingroup binders
   */
  template <typename _Signature>
  struct is_bind_expression<volatile _Bind<_Signature>>
      : public true_type
  {
  };

  /**
   *  @brief Class template _Bind is always a bind expression.
   *  @ingroup binders
   */
  template <typename _Signature>
  struct is_bind_expression<const volatile _Bind<_Signature>>
      : public true_type
  {
  };

  /**
   *  @brief Class template _Bind_result is always a bind expression.
   *  @ingroup binders
   */
  template <typename _Result, typename _Signature>
  struct is_bind_expression<_Bind_result<_Result, _Signature>>
      : public true_type
  {
  };

  /**
   *  @brief Class template _Bind_result is always a bind expression.
   *  @ingroup binders
   */
  template <typename _Result, typename _Signature>
  struct is_bind_expression<const _Bind_result<_Result, _Signature>>
      : public true_type
  {
  };

  /**
   *  @brief Class template _Bind_result is always a bind expression.
   *  @ingroup binders
   */
  template <typename _Result, typename _Signature>
  struct is_bind_expression<volatile _Bind_result<_Result, _Signature>>
      : public true_type
  {
  };

  /**
   *  @brief Class template _Bind_result is always a bind expression.
   *  @ingroup binders
   */
  template <typename _Result, typename _Signature>
  struct is_bind_expression<const volatile _Bind_result<_Result, _Signature>>
      : public true_type
  {
  };

  template <typename _Func, typename... _BoundArgs>
  struct _Bind_check_arity
  {
  };

  template <typename _Ret, typename... _Args, typename... _BoundArgs>
  struct _Bind_check_arity<_Ret (*)(_Args...), _BoundArgs...>
  {
    static_assert(sizeof...(_BoundArgs) == sizeof...(_Args),
                  "Wrong number of arguments for function");
  };

  template <typename _Ret, typename... _Args, typename... _BoundArgs>
  struct _Bind_check_arity<_Ret (*)(_Args......), _BoundArgs...>
  {
    static_assert(sizeof...(_BoundArgs) >= sizeof...(_Args),
                  "Wrong number of arguments for function");
  };

  template <typename _Tp, typename _Class, typename... _BoundArgs>
  struct _Bind_check_arity<_Tp _Class::*, _BoundArgs...>
  {
    using _Arity = typename _Mem_fn<_Tp _Class::*>::_Arity;
    using _Varargs = typename _Mem_fn<_Tp _Class::*>::_Varargs;
    static_assert(_Varargs::value
                      ? sizeof...(_BoundArgs) >= _Arity::value + 1
                      : sizeof...(_BoundArgs) == _Arity::value + 1,
                  "Wrong number of arguments for pointer-to-member");
  };

  // Trait type used to remove std::bind() from overload set via SFINAE
  // when first argument has integer type, so that std::bind() will
  // not be a better match than ::bind() from the BSD Sockets API.
  template <typename _Tp, typename _Tp2 = typename decay<_Tp>::type>
  using __is_socketlike = __or_<is_integral<_Tp2>, is_enum<_Tp2>>;

  template <bool _SocketLike, typename _Func, typename... _BoundArgs>
  struct _Bind_helper
      : _Bind_check_arity<typename decay<_Func>::type, _BoundArgs...>
  {
    typedef typename decay<_Func>::type __func_type;
    typedef _Bind<__func_type(typename decay<_BoundArgs>::type...)> type;
  };

  // Partial specialization for is_socketlike == true, does not define
  // nested type so std::bind() will not participate in overload resolution
  // when the first argument might be a socket file descriptor.
  template <typename _Func, typename... _BoundArgs>
  struct _Bind_helper<true, _Func, _BoundArgs...>
  {
  };

  /**
   *  @brief Function template for std::bind.
   *  @ingroup binders
   */
  template <typename _Func, typename... _BoundArgs>
  inline typename _Bind_helper<__is_socketlike<_Func>::value, _Func, _BoundArgs...>::type
  bind(_Func && __f, _BoundArgs && ... __args)
  {
    typedef _Bind_helper<false, _Func, _BoundArgs...> __helper_type;
    return typename __helper_type::type(std::forward<_Func>(__f),
                                        std::forward<_BoundArgs>(__args)...);
  }

  template <typename _Result, typename _Func, typename... _BoundArgs>
  struct _Bindres_helper
      : _Bind_check_arity<typename decay<_Func>::type, _BoundArgs...>
  {
    typedef typename decay<_Func>::type __functor_type;
    typedef _Bind_result<_Result,
                         __functor_type(typename decay<_BoundArgs>::type...)>
        type;
  };

  /**
   *  @brief Function template for std::bind<R>.
   *  @ingroup binders
   */
  template <typename _Result, typename _Func, typename... _BoundArgs>
  inline
      typename _Bindres_helper<_Result, _Func, _BoundArgs...>::type
      bind(_Func && __f, _BoundArgs && ... __args)
  {
    typedef _Bindres_helper<_Result, _Func, _BoundArgs...> __helper_type;
    return typename __helper_type::type(std::forward<_Func>(__f),
                                        std::forward<_BoundArgs>(__args)...);
  }

#if __cplusplus >= 201402L
  /// Generalized negator.
  template <typename _Fn>
  class _Not_fn
  {
    template <typename _Fn2, typename... _Args>
    using __inv_res_t = typename __invoke_result<_Fn2, _Args...>::type;

    template <typename _Tp>
    static decltype(!std::declval<_Tp>())
    _S_not() noexcept(noexcept(!std::declval<_Tp>()));

  public:
    template <typename _Fn2>
    _Not_fn(_Fn2 &&__fn, int)
        : _M_fn(std::forward<_Fn2>(__fn)) {}

    _Not_fn(const _Not_fn &__fn) = default;
    _Not_fn(_Not_fn &&__fn) = default;
    ~_Not_fn() = default;

    // Macro to define operator() with given cv-qualifiers ref-qualifiers,
    // forwarding _M_fn and the function arguments with the same qualifiers,
    // and deducing the return type and exception-specification.
#define _GLIBCXX_NOT_FN_CALL_OP(_QUALS)                                                                                                \
  \ template <typename... _Args>                                  \ decltype(_S_not<__inv_res_t<_Fn _QUALS, _Args...>>())           \  \
  operator()(_Args &&... __args) _QUALS                            \ noexcept(noexcept(_S_not<__inv_res_t<_Fn _QUALS, _Args...>>())) \ \
  {                                                                                                                                    \
    \ return !std::__invoke(std::forward<_Fn _QUALS>(_M_fn),      \ std::forward<_Args>(__args)...);                                   \
    \                                                                                                                                  \
  }
    _GLIBCXX_NOT_FN_CALL_OP(&)
    _GLIBCXX_NOT_FN_CALL_OP(const &)
    _GLIBCXX_NOT_FN_CALL_OP(&&)
    _GLIBCXX_NOT_FN_CALL_OP(const &&)
#undef _GLIBCXX_NOT_FN_CALL

  private:
    _Fn _M_fn;
  };

  template <typename _Tp, typename _Pred>
  struct __is_byte_like : false_type
  {
  };

  template <typename _Tp>
  struct __is_byte_like<_Tp, equal_to<_Tp>>
      : __bool_constant<sizeof(_Tp) == 1 && is_integral<_Tp>::value>
  {
  };

  template <typename _Tp>
  struct __is_byte_like<_Tp, equal_to<void>>
      : __bool_constant<sizeof(_Tp) == 1 && is_integral<_Tp>::value>
  {
  };

#if __cplusplus >= 201703L
  // Declare std::byte (full definition is in <cstddef>).
  enum class byte : unsigned char;

  template <>
  struct __is_byte_like<byte, equal_to<byte>>
      : true_type
  {
  };

  template <>
  struct __is_byte_like<byte, equal_to<void>>
      : true_type
  {
  };

#define __cpp_lib_not_fn 201603
  /// [func.not_fn] Function template not_fn
  template <typename _Fn>
  inline auto
      not_fn(_Fn && __fn) noexcept(std::is_nothrow_constructible<std::decay_t<_Fn>, _Fn &&>::value)
  {
    return _Not_fn<std::decay_t<_Fn>>{std::forward<_Fn>(__fn), 0};
  }

  // Searchers
#define __cpp_lib_boyer_moore_searcher 201603

  template <typename _ForwardIterator1, typename _BinaryPredicate = equal_to<>>
  class default_searcher
  {
  public:
    default_searcher(_ForwardIterator1 __pat_first,
                     _ForwardIterator1 __pat_last,
                     _BinaryPredicate __pred = _BinaryPredicate())
        : _M_m(__pat_first, __pat_last, std::move(__pred))
    {
    }

    template <typename _ForwardIterator2>
    pair<_ForwardIterator2, _ForwardIterator2>
    operator()(_ForwardIterator2 __first, _ForwardIterator2 __last) const
    {
      _ForwardIterator2 __first_ret =
          std::search(__first, __last, std::get<0>(_M_m), std::get<1>(_M_m),
                      std::get<2>(_M_m));
      auto __ret = std::make_pair(__first_ret, __first_ret);
      if (__ret.first != __last)
        std::advance(__ret.second, std::distance(std::get<0>(_M_m),
                                                 std::get<1>(_M_m)));
      return __ret;
    }

  private:
    tuple<_ForwardIterator1, _ForwardIterator1, _BinaryPredicate> _M_m;
  };

  template <typename _Key, typename _Tp, typename _Hash, typename _Pred>
  struct __boyer_moore_map_base
  {
    template <typename _RAIter>
    __boyer_moore_map_base(_RAIter __pat, size_t __patlen,
                           _Hash &&__hf, _Pred &&__pred)
        : _M_bad_char{__patlen, std::move(__hf), std::move(__pred)}
    {
      if (__patlen > 0)
        for (__diff_type __i = 0; __i < __patlen - 1; ++__i)
          _M_bad_char[__pat[__i]] = __patlen - 1 - __i;
    }

    using __diff_type = _Tp;

    __diff_type
    _M_lookup(_Key __key, __diff_type __not_found) const
    {
      auto __iter = _M_bad_char.find(__key);
      if (__iter == _M_bad_char.end())
        return __not_found;
      return __iter->second;
    }

    _Pred
    _M_pred() const { return _M_bad_char.key_eq(); }

    _GLIBCXX_STD_C::unordered_map<_Key, _Tp, _Hash, _Pred> _M_bad_char;
  };

  template <typename _Tp, size_t _Len, typename _Pred>
  struct __boyer_moore_array_base
  {
    template <typename _RAIter, typename _Unused>
    __boyer_moore_array_base(_RAIter __pat, size_t __patlen,
                             _Unused &&, _Pred &&__pred)
        : _M_bad_char{_GLIBCXX_STD_C::array<_Tp, _Len>{}, std::move(__pred)}
    {
      std::get<0>(_M_bad_char).fill(__patlen);
      if (__patlen > 0)
        for (__diff_type __i = 0; __i < __patlen - 1; ++__i)
        {
          auto __ch = __pat[__i];
          using _UCh = make_unsigned_t<decltype(__ch)>;
          auto __uch = static_cast<_UCh>(__ch);
          std::get<0>(_M_bad_char)[__uch] = __patlen - 1 - __i;
        }
    }

    using __diff_type = _Tp;

    template <typename _Key>
    __diff_type
    _M_lookup(_Key __key, __diff_type __not_found) const
    {
      auto __ukey = static_cast<make_unsigned_t<_Key>>(__key);
      if (__ukey >= _Len)
        return __not_found;
      return std::get<0>(_M_bad_char)[__ukey];
    }

    const _Pred &
    _M_pred() const { return std::get<1>(_M_bad_char); }

    tuple<_GLIBCXX_STD_C::array<_Tp, _Len>, _Pred> _M_bad_char;
  };

  // Use __boyer_moore_array_base when pattern consists of narrow characters
  // (or std::byte) and uses std::equal_to as the predicate.
  template <typename _RAIter, typename _Hash, typename _Pred,
            typename _Val = typename iterator_traits<_RAIter>::value_type,
            typename _Diff = typename iterator_traits<_RAIter>::difference_type>
  using __boyer_moore_base_t = conditional_t<__is_byte_like<_Val, _Pred>::value,
                                             __boyer_moore_array_base<_Diff, 256, _Pred>,
                                             __boyer_moore_map_base<_Val, _Diff, _Hash, _Pred>>;

  template <typename _RAIter, typename _Hash = hash<typename iterator_traits<_RAIter>::value_type>,
            typename _BinaryPredicate = equal_to<>>
  class boyer_moore_searcher
      : __boyer_moore_base_t<_RAIter, _Hash, _BinaryPredicate>
  {
    using _Base = __boyer_moore_base_t<_RAIter, _Hash, _BinaryPredicate>;
    using typename _Base::__diff_type;

  public:
    boyer_moore_searcher(_RAIter __pat_first, _RAIter __pat_last,
                         _Hash __hf = _Hash(),
                         _BinaryPredicate __pred = _BinaryPredicate());

    template <typename _RandomAccessIterator2>
    pair<_RandomAccessIterator2, _RandomAccessIterator2>
    operator()(_RandomAccessIterator2 __first,
               _RandomAccessIterator2 __last) const;

  private:
    bool
    _M_is_prefix(_RAIter __word, __diff_type __len,
                 __diff_type __pos)
    {
      const auto &__pred = this->_M_pred();
      __diff_type __suffixlen = __len - __pos;
      for (__diff_type __i = 0; __i < __suffixlen; ++__i)
        if (!__pred(__word[__i], __word[__pos + __i]))
          return false;
      return true;
    }

    __diff_type
    _M_suffix_length(_RAIter __word, __diff_type __len,
                     __diff_type __pos)
    {
      const auto &__pred = this->_M_pred();
      __diff_type __i = 0;
      while (__pred(__word[__pos - __i], __word[__len - 1 - __i]) && __i < __pos)
      {
        ++__i;
      }
      return __i;
    }

    template <typename _Tp>
    __diff_type
    _M_bad_char_shift(_Tp __c) const
    {
      return this->_M_lookup(__c, _M_pat_end - _M_pat);
    }

    _RAIter _M_pat;
    _RAIter _M_pat_end;
    _GLIBCXX_STD_C::vector<__diff_type> _M_good_suffix;
  };

  template <typename _RAIter, typename _Hash = hash<typename iterator_traits<_RAIter>::value_type>,
            typename _BinaryPredicate = equal_to<>>
  class boyer_moore_horspool_searcher
      : __boyer_moore_base_t<_RAIter, _Hash, _BinaryPredicate>
  {
    using _Base = __boyer_moore_base_t<_RAIter, _Hash, _BinaryPredicate>;
    using typename _Base::__diff_type;

  public:
    boyer_moore_horspool_searcher(_RAIter __pat,
                                  _RAIter __pat_end,
                                  _Hash __hf = _Hash(),
                                  _BinaryPredicate __pred = _BinaryPredicate())
        : _Base(__pat, __pat_end - __pat, std::move(__hf), std::move(__pred)),
          _M_pat(__pat), _M_pat_end(__pat_end)
    {
    }

    template <typename _RandomAccessIterator2>
    pair<_RandomAccessIterator2, _RandomAccessIterator2>
    operator()(_RandomAccessIterator2 __first,
               _RandomAccessIterator2 __last) const
    {
      const auto &__pred = this->_M_pred();
      auto __patlen = _M_pat_end - _M_pat;
      if (__patlen == 0)
        return std::make_pair(__first, __first);
      auto __len = __last - __first;
      while (__len >= __patlen)
      {
        for (auto __scan = __patlen - 1;
             __pred(__first[__scan], _M_pat[__scan]); --__scan)
          if (__scan == 0)
            return std::make_pair(__first, __first + __patlen);
        auto __shift = _M_bad_char_shift(__first[__patlen - 1]);
        __len -= __shift;
        __first += __shift;
      }
      return std::make_pair(__last, __last);
    }

  private:
    template <typename _Tp>
    __diff_type
    _M_bad_char_shift(_Tp __c) const
    {
      return this->_M_lookup(__c, _M_pat_end - _M_pat);
    }

    _RAIter _M_pat;
    _RAIter _M_pat_end;
  };

  template <typename _RAIter, typename _Hash, typename _BinaryPredicate>
  boyer_moore_searcher<_RAIter, _Hash, _BinaryPredicate>::
      boyer_moore_searcher(_RAIter __pat, _RAIter __pat_end,
                           _Hash __hf, _BinaryPredicate __pred)
      : _Base(__pat, __pat_end - __pat, std::move(__hf), std::move(__pred)),
        _M_pat(__pat), _M_pat_end(__pat_end), _M_good_suffix(__pat_end - __pat)
  {
    auto __patlen = __pat_end - __pat;
    if (__patlen == 0)
      return;
    __diff_type __last_prefix = __patlen - 1;
    for (__diff_type __p = __patlen - 1; __p >= 0; --__p)
    {
      if (_M_is_prefix(__pat, __patlen, __p + 1))
        __last_prefix = __p + 1;
      _M_good_suffix[__p] = __last_prefix + (__patlen - 1 - __p);
    }
    for (__diff_type __p = 0; __p < __patlen - 1; ++__p)
    {
      auto __slen = _M_suffix_length(__pat, __patlen, __p);
      auto __pos = __patlen - 1 - __slen;
      if (!__pred(__pat[__p - __slen], __pat[__pos]))
        _M_good_suffix[__pos] = __patlen - 1 - __p + __slen;
    }
  }

  template <typename _RAIter, typename _Hash, typename _BinaryPredicate>
  template <typename _RandomAccessIterator2>
  pair<_RandomAccessIterator2, _RandomAccessIterator2>
  boyer_moore_searcher<_RAIter, _Hash, _BinaryPredicate>::
  operator()(_RandomAccessIterator2 __first,
             _RandomAccessIterator2 __last) const
  {
    auto __patlen = _M_pat_end - _M_pat;
    if (__patlen == 0)
      return std::make_pair(__first, __first);
    const auto &__pred = this->_M_pred();
    __diff_type __i = __patlen - 1;
    auto __stringlen = __last - __first;
    while (__i < __stringlen)
    {
      __diff_type __j = __patlen - 1;
      while (__j >= 0 && __pred(__first[__i], _M_pat[__j]))
      {
        --__i;
        --__j;
      }
      if (__j < 0)
      {
        const auto __match = __first + __i + 1;
        return std::make_pair(__match, __match + __patlen);
      }
      __i += std::max(_M_bad_char_shift(__first[__i]),
                      _M_good_suffix[__j]);
    }
    return std::make_pair(__last, __last);
  }

#endif // C++17
#endif // C++14

  _GLIBCXX_END_NAMESPACE_VERSION
} // namespace std_GLIBCXX_VISIBILITY(default)

#endif // C++11

#endif // _GLIBCXX_FUNCTIONAL
