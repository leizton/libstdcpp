// <future> -*- C++ -*-

// Copyright (C) 2009-2018 Free Software Foundation, Inc.
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

/** @file include/future
 *  This is a Standard C++ Library header.
 */

#ifndef _GLIBCXX_FUTURE
#define _GLIBCXX_FUTURE 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#include <mutex>
#include <thread>
#include <condition_variable>
#include <system_error>
#include <atomic>
#include <bits/atomic_futex.h>
#include <bits/functexcept.h>
#include <bits/invoke.h>
#include <bits/unique_ptr.h>
#include <bits/shared_ptr.h>
#include <bits/std_function.h>
#include <bits/uses_allocator.h>
#include <bits/allocated_ptr.h>
#include <ext/aligned_buffer.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @defgroup futures Futures
   * @ingroup concurrency
   *
   * Classes for futures support.
   * @{
   */

  /// Error code for futures
  enum class future_errc
  {
    future_already_retrieved = 1,
    promise_already_satisfied,
    no_state,
    broken_promise
  };

  /// Specialization.
  template<>
    struct is_error_code_enum<future_errc> : public true_type { };

  /// Points to a statically-allocated object derived from error_category.
  const error_category&
  future_category() noexcept;

  /// Overload for make_error_code.
  inline error_code
  make_error_code(future_errc __errc) noexcept
  { return error_code(static_cast<int>(__errc), future_category()); }

  /// Overload for make_error_condition.
  inline error_condition
  make_error_condition(future_errc __errc) noexcept
  { return error_condition(static_cast<int>(__errc), future_category()); }

  /**
   *  @brief Exception type thrown by futures.
   *  @ingroup exceptions
   */
  class future_error : public logic_error
  {
  public:
    explicit
    future_error(future_errc __errc)
    : future_error(std::make_error_code(__errc))
    { }

    virtual ~future_error() noexcept;

    virtual const char*
    what() const noexcept;

    const error_code&
    code() const noexcept { return _M_code; }

  private:
    explicit
    future_error(error_code __ec)
    : logic_error("std::future_error: " + __ec.message()), _M_code(__ec)
    { }

    friend void __throw_future_error(int);

    error_code                     _M_code;
  };

  // Forward declarations.
  template<typename _Res>
    class future;

  template<typename _Res>
    class shared_future;

  template<typename _Signature>
    class packaged_task;

  template<typename _Res>
    class promise;

  /// Launch code for futures
  enum class launch
  {
    async = 1,
    deferred = 2
  };

  constexpr launch operator&(launch __x, launch __y)
  {
    return static_cast<launch>(
   static_cast<int>(__x) & static_cast<int>(__y));
  }

  constexpr launch operator|(launch __x, launch __y)
  {
    return static_cast<launch>(
   static_cast<int>(__x) | static_cast<int>(__y));
  }

  constexpr launch operator^(launch __x, launch __y)
  {
    return static_cast<launch>(
   static_cast<int>(__x) ^ static_cast<int>(__y));
  }

  constexpr launch operator~(launch __x)
  { return static_cast<launch>(~static_cast<int>(__x)); }

  inline launch& operator&=(launch& __x, launch __y)
  { return __x = __x & __y; }

  inline launch& operator|=(launch& __x, launch __y)
  { return __x = __x | __y; }

  inline launch& operator^=(launch& __x, launch __y)
  { return __x = __x ^ __y; }

  /// Status code for futures
  enum class future_status
  {
    ready,
    timeout,
    deferred
  };

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2021. Further incorrect usages of result_of
  template<typename _Fn, typename... _Args>
    using __async_result_of = typename result_of<
      typename decay<_Fn>::type(typename decay<_Args>::type...)>::type;

  template<typename _Fn, typename... _Args>
    future<__async_result_of<_Fn, _Args...>>
    async(launch __policy, _Fn&& __fn, _Args&&... __args);

  template<typename _Fn, typename... _Args>
    future<__async_result_of<_Fn, _Args...>>
    async(_Fn&& __fn, _Args&&... __args);

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)

  /// Base class and enclosing scope.
  struct __future_base
  {
    /// Base class for results.
    struct _Result_base
    {
      exception_ptr                _M_error;

      _Result_base(const _Result_base&) = delete;
      _Result_base& operator=(const _Result_base&) = delete;

      // _M_destroy() allows derived classes to control deallocation
      virtual void _M_destroy() = 0;

      struct _Deleter
      {
   void operator()(_Result_base* __fr) const { __fr->_M_destroy(); }
      };

    protected:
      _Result_base();
      virtual ~_Result_base();
    };

    /// A unique_ptr for result objects.
    template<typename _Res>
      using _Ptr = unique_ptr<_Res, _Result_base::_Deleter>;

    /// A result object that has storage for an object of type _Res.
    template<typename _Res>
      struct _Result : _Result_base
      {
      private:
   __gnu_cxx::__aligned_buffer<_Res>       _M_storage;
   bool                                    _M_initialized;

      public:
   typedef _Res result_type;

   _Result() noexcept : _M_initialized() { }

   ~_Result()
   {
     if (_M_initialized)
       _M_value().~_Res();
   }

   // Return lvalue, future will add const or rvalue-reference
   _Res&
   _M_value() noexcept { return *_M_storage._M_ptr(); }

   void
   _M_set(const _Res& __res)
   {
     ::new (_M_storage._M_addr()) _Res(__res);
     _M_initialized = true;
   }

   void
   _M_set(_Res&& __res)
   {
     ::new (_M_storage._M_addr()) _Res(std::move(__res));
     _M_initialized = true;
   }

      private:
   void _M_destroy() { delete this; }
    };

    /// A result object that uses an allocator.
    template<typename _Res, typename _Alloc>
      struct _Result_alloc final : _Result<_Res>, _Alloc
      {
   using __allocator_type = __alloc_rebind<_Alloc, _Result_alloc>;

        explicit
   _Result_alloc(const _Alloc& __a) : _Result<_Res>(), _Alloc(__a)
   { }

      private:
   void _M_destroy()
   {
     __allocator_type __a(*this);
     __allocated_ptr<__allocator_type> __guard_ptr{ __a, this };
     this->~_Result_alloc();
   }
      };

    // Create a result object that uses an allocator.
    template<typename _Res, typename _Allocator>
      static _Ptr<_Result_alloc<_Res, _Allocator>>
      _S_allocate_result(const _Allocator& __a)
      {
   using __result_type = _Result_alloc<_Res, _Allocator>;
   typename __result_type::__allocator_type __a2(__a);
   auto __guard = std::__allocate_guarded(__a2);
   __result_type* __p = ::new((void*)__guard.get()) __result_type{__a};
   __guard = nullptr;
   return _Ptr<__result_type>(__p);
      }

    // Keep it simple for std::allocator.
    template<typename _Res, typename _Tp>
      static _Ptr<_Result<_Res>>
      _S_allocate_result(const std::allocator<_Tp>& __a)
      {
   return _Ptr<_Result<_Res>>(new _Result<_Res>);
      }

    // Base class for various types of shared state created by an
    // asynchronous provider (such as a std::promise) and shared with one
    // or more associated futures.
    class _State_baseV2
    {
      typedef _Ptr<_Result_base> _Ptr_type;

      enum _Status : unsigned {
   __not_ready,
   __ready
      };

      _Ptr_type                    _M_result;
      __atomic_futex_unsigned<>    _M_status;
      atomic_flag          _M_retrieved = ATOMIC_FLAG_INIT;
      once_flag                    _M_once;

    public:
      _State_baseV2() noexcept : _M_result(), _M_status(_Status::__not_ready)
   { }
      _State_baseV2(const _State_baseV2&) = delete;
      _State_baseV2& operator=(const _State_baseV2&) = delete;
      virtual ~_State_baseV2() = default;

      _Result_base&
      wait()
      {
   // Run any deferred function or join any asynchronous thread:
   _M_complete_async();
   // Acquire MO makes sure this synchronizes with the thread that made
   // the future ready.
   _M_status._M_load_when_equal(_Status::__ready, memory_order_acquire);
   return *_M_result;
      }

      template<typename _Rep, typename _Period>
        future_status
        wait_for(const chrono::duration<_Rep, _Period>& __rel)
        {
     // First, check if the future has been made ready.  Use acquire MO
     // to synchronize with the thread that made it ready.
     if (_M_status._M_load(memory_order_acquire) == _Status::__ready)
       return future_status::ready;
     if (_M_is_deferred_future())
       return future_status::deferred;
     if (_M_status._M_load_when_equal_for(_Status::__ready,
         memory_order_acquire, __rel))
       {
         // _GLIBCXX_RESOLVE_LIB_DEFECTS
         // 2100.  timed waiting functions must also join
         // This call is a no-op by default except on an async future,
         // in which case the async thread is joined.  It's also not a
         // no-op for a deferred future, but such a future will never
         // reach this point because it returns future_status::deferred
         // instead of waiting for the future to become ready (see
         // above).  Async futures synchronize in this call, so we need
         // no further synchronization here.
         _M_complete_async();

         return future_status::ready;
       }
     return future_status::timeout;
   }

      template<typename _Clock, typename _Duration>
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs)
        {
     // First, check if the future has been made ready.  Use acquire MO
     // to synchronize with the thread that made it ready.
     if (_M_status._M_load(memory_order_acquire) == _Status::__ready)
       return future_status::ready;
     if (_M_is_deferred_future())
       return future_status::deferred;
     if (_M_status._M_load_when_equal_until(_Status::__ready,
         memory_order_acquire, __abs))
       {
         // _GLIBCXX_RESOLVE_LIB_DEFECTS
         // 2100.  timed waiting functions must also join
         // See wait_for(...) above.
         _M_complete_async();

         return future_status::ready;
       }
     return future_status::timeout;
   }

      // Provide a result to the shared state and make it ready.
      // Calls at most once: _M_result = __res();
      void
      _M_set_result(function<_Ptr_type()> __res, bool __ignore_failure = false)
      {
   bool __did_set = false;
        // all calls to this function are serialized,
        // side-effects of invoking __res only happen once
   call_once(_M_once, &_State_baseV2::_M_do_set, this,
             std::__addressof(__res), std::__addressof(__did_set));
   if (__did_set)
     // Use release MO to synchronize with observers of the ready state.
     _M_status._M_store_notify_all(_Status::__ready,
                                   memory_order_release);
   else if (!__ignore_failure)
          __throw_future_error(int(future_errc::promise_already_satisfied));
      }

      // Provide a result to the shared state but delay making it ready
      // until the calling thread exits.
      // Calls at most once: _M_result = __res();
      void
      _M_set_delayed_result(function<_Ptr_type()> __res,
                       weak_ptr<_State_baseV2> __self)
      {
   bool __did_set = false;
   unique_ptr<_Make_ready> __mr{new _Make_ready};
        // all calls to this function are serialized,
        // side-effects of invoking __res only happen once
   call_once(_M_once, &_State_baseV2::_M_do_set, this,
             std::__addressof(__res), std::__addressof(__did_set));
   if (!__did_set)
          __throw_future_error(int(future_errc::promise_already_satisfied));
   __mr->_M_shared_state = std::move(__self);
   __mr->_M_set();
   __mr.release();
      }

      // Abandon this shared state.
      void
      _M_break_promise(_Ptr_type __res)
      {
   if (static_cast<bool>(__res))
     {
       __res->_M_error =
         make_exception_ptr(future_error(future_errc::broken_promise));
       // This function is only called when the last asynchronous result
       // provider is abandoning this shared state, so noone can be
       // trying to make the shared state ready at the same time, and
       // we can access _M_result directly instead of through call_once.
       _M_result.swap(__res);
       // Use release MO to synchronize with observers of the ready state.
       _M_status._M_store_notify_all(_Status::__ready,
                                     memory_order_release);
     }
      }

      // Called when this object is first passed to a future.
      void
      _M_set_retrieved_flag()
      {
   if (_M_retrieved.test_and_set())
     __throw_future_error(int(future_errc::future_already_retrieved));
      }

      template<typename _Res, typename _Arg>
        struct _Setter;

      // set lvalues
      template<typename _Res, typename _Arg>
        struct _Setter<_Res, _Arg&>
        {
          // check this is only used by promise<R>::set_value(const R&)
          // or promise<R&>::set_value(R&)
          static_assert(is_same<_Res, _Arg&>::value  // promise<R&>
              || is_same<const _Res, _Arg>::value,   // promise<R>
              "Invalid specialisation");

     // Used by std::promise to copy construct the result.
          typename promise<_Res>::_Ptr_type operator()() const
          {
            _M_promise->_M_storage->_M_set(*_M_arg);
            return std::move(_M_promise->_M_storage);
          }
          promise<_Res>*    _M_promise;
          _Arg*             _M_arg;
        };

      // set rvalues
      template<typename _Res>
        struct _Setter<_Res, _Res&&>
        {
     // Used by std::promise to move construct the result.
          typename promise<_Res>::_Ptr_type operator()() const
          {
            _M_promise->_M_storage->_M_set(std::move(*_M_arg));
            return std::move(_M_promise->_M_storage);
          }
          promise<_Res>*    _M_promise;
          _Res*             _M_arg;
        };

      // set void
      template<typename _Res>
   struct _Setter<_Res, void>
   {
     static_assert(is_void<_Res>::value, "Only used for promise<void>");

     typename promise<_Res>::_Ptr_type operator()() const
     { return std::move(_M_promise->_M_storage); }

     promise<_Res>*    _M_promise;
   };

      struct __exception_ptr_tag { };

      // set exceptions
      template<typename _Res>
        struct _Setter<_Res, __exception_ptr_tag>
        {
     // Used by std::promise to store an exception as the result.
          typename promise<_Res>::_Ptr_type operator()() const
          {
            _M_promise->_M_storage->_M_error = *_M_ex;
            return std::move(_M_promise->_M_storage);
          }

          promise<_Res>*   _M_promise;
          exception_ptr*    _M_ex;
        };

      template<typename _Res, typename _Arg>
        static _Setter<_Res, _Arg&&>
        __setter(promise<_Res>* __prom, _Arg&& __arg)
        {
     _S_check(__prom->_M_future);
          return _Setter<_Res, _Arg&&>{ __prom, std::__addressof(__arg) };
        }

      template<typename _Res>
        static _Setter<_Res, __exception_ptr_tag>
        __setter(exception_ptr& __ex, promise<_Res>* __prom)
        {
     _S_check(__prom->_M_future);
          return _Setter<_Res, __exception_ptr_tag>{ __prom, &__ex };
        }

      template<typename _Res>
   static _Setter<_Res, void>
   __setter(promise<_Res>* __prom)
   {
     _S_check(__prom->_M_future);
     return _Setter<_Res, void>{ __prom };
   }

      template<typename _Tp>
        static void
        _S_check(const shared_ptr<_Tp>& __p)
        {
          if (!static_cast<bool>(__p))
            __throw_future_error((int)future_errc::no_state);
        }

    private:
      // The function invoked with std::call_once(_M_once, ...).
      void
      _M_do_set(function<_Ptr_type()>* __f, bool* __did_set)
      {
        _Ptr_type __res = (*__f)();
        // Notify the caller that we did try to set; if we do not throw an
        // exception, the caller will be aware that it did set (e.g., see
        // _M_set_result).
   *__did_set = true;
        _M_result.swap(__res); // nothrow
      }

      // Wait for completion of async function.
      virtual void _M_complete_async() { }

      // Return true if state corresponds to a deferred function.
      virtual bool _M_is_deferred_future() const { return false; }

      struct _Make_ready final : __at_thread_exit_elt
      {
   weak_ptr<_State_baseV2> _M_shared_state;
   static void _S_run(void*);
   void _M_set();
      };
    };

#ifdef _GLIBCXX_ASYNC_ABI_COMPAT
    class _State_base;
    class _Async_state_common;
#else
    using _State_base = _State_baseV2;
    class _Async_state_commonV2;
#endif

    template<typename _BoundFn,
        typename _Res = decltype(std::declval<_BoundFn&>()())>
      class _Deferred_state;

    template<typename _BoundFn,
        typename _Res = decltype(std::declval<_BoundFn&>()())>
      class _Async_state_impl;

    template<typename _Signature>
      class _Task_state_base;

    template<typename _Fn, typename _Alloc, typename _Signature>
      class _Task_state;

    template<typename _BoundFn>
      static std::shared_ptr<_State_base>
      _S_make_deferred_state(_BoundFn&& __fn);

    template<typename _BoundFn>
      static std::shared_ptr<_State_base>
      _S_make_async_state(_BoundFn&& __fn);

    template<typename _Res_ptr, typename _Fn,
        typename _Res = typename _Res_ptr::element_type::result_type>
      struct _Task_setter;

    template<typename _Res_ptr, typename _BoundFn>
      static _Task_setter<_Res_ptr, _BoundFn>
      _S_task_setter(_Res_ptr& __ptr, _BoundFn& __call)
      {
   return { std::__addressof(__ptr), std::__addressof(__call) };
      }
  };

  /// Partial specialization for reference types.
  template<typename _Res>
    struct __future_base::_Result<_Res&> : __future_base::_Result_base
    {
      typedef _Res& result_type;

      _Result() noexcept : _M_value_ptr() { }

      void
      _M_set(_Res& __res) noexcept
      { _M_value_ptr = std::addressof(__res); }

      _Res& _M_get() noexcept { return *_M_value_ptr; }

    private:
      _Res*                        _M_value_ptr;

      void _M_destroy() { delete this; }
    };

  /// Explicit specialization for void.
  template<>
    struct __future_base::_Result<void> : __future_base::_Result_base
    {
      typedef void result_type;

    private:
      void _M_destroy() { delete this; }
    };

#ifndef _GLIBCXX_ASYNC_ABI_COMPAT

  // Allow _Setter objects to be stored locally in std::function
  template<typename _Res, typename _Arg>
    struct __is_location_invariant
    <__future_base::_State_base::_Setter<_Res, _Arg>>
    : true_type { };

  // Allow _Task_setter objects to be stored locally in std::function
  template<typename _Res_ptr, typename _Fn, typename _Res>
    struct __is_location_invariant
    <__future_base::_Task_setter<_Res_ptr, _Fn, _Res>>
    : true_type { };

  /// Common implementation for future and shared_future.
  template<typename _Res>
    class __basic_future : public __future_base
    {
    protected:
      typedef shared_ptr<_State_base>              __state_type;
      typedef __future_base::_Result<_Res>&        __result_type;

    private:
      __state_type                 _M_state;

    public:
      // Disable copying.
      __basic_future(const __basic_future&) = delete;
      __basic_future& operator=(const __basic_future&) = delete;

      bool
      valid() const noexcept { return static_cast<bool>(_M_state); }

      void
      wait() const
      {
        _State_base::_S_check(_M_state);
        _M_state->wait();
      }

      template<typename _Rep, typename _Period>
        future_status
        wait_for(const chrono::duration<_Rep, _Period>& __rel) const
        {
          _State_base::_S_check(_M_state);
          return _M_state->wait_for(__rel);
        }

      template<typename _Clock, typename _Duration>
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs) const
        {
          _State_base::_S_check(_M_state);
          return _M_state->wait_until(__abs);
        }

    protected:
      /// Wait for the state to be ready and rethrow any stored exception
      __result_type
      _M_get_result() const
      {
        _State_base::_S_check(_M_state);
        _Result_base& __res = _M_state->wait();
        if (!(__res._M_error == 0))
          rethrow_exception(__res._M_error);
        return static_cast<__result_type>(__res);
      }

      void _M_swap(__basic_future& __that) noexcept
      {
        _M_state.swap(__that._M_state);
      }

      // Construction of a future by promise::get_future()
      explicit
      __basic_future(const __state_type& __state) : _M_state(__state)
      {
        _State_base::_S_check(_M_state);
        _M_state->_M_set_retrieved_flag();
      }

      // Copy construction from a shared_future
      explicit
      __basic_future(const shared_future<_Res>&) noexcept;

      // Move construction from a shared_future
      explicit
      __basic_future(shared_future<_Res>&&) noexcept;

      // Move construction from a future
      explicit
      __basic_future(future<_Res>&&) noexcept;

      constexpr __basic_future() noexcept : _M_state() { }

      struct _Reset
      {
        explicit _Reset(__basic_future& __fut) noexcept : _M_fut(__fut) { }
        ~_Reset() { _M_fut._M_state.reset(); }
        __basic_future& _M_fut;
      };
    };


  /// Primary template for future.
  template<typename _Res>
    class future : public __basic_future<_Res>
    {
      friend class promise<_Res>;
      template<typename> friend class packaged_task;
      template<typename _Fn, typename... _Args>
        friend future<__async_result_of<_Fn, _Args...>>
        async(launch, _Fn&&, _Args&&...);

      typedef __basic_future<_Res> _Base_type;
      typedef typename _Base_type::__state_type __state_type;

      explicit
      future(const __state_type& __state) : _Base_type(__state) { }

    public:
      constexpr future() noexcept : _Base_type() { }

      /// Move constructor
      future(future&& __uf) noexcept : _Base_type(std::move(__uf)) { }

      // Disable copying
      future(const future&) = delete;
      future& operator=(const future&) = delete;

      future& operator=(future&& __fut) noexcept
      {
        future(std::move(__fut))._M_swap(*this);
        return *this;
      }

      /// Retrieving the value
      _Res
      get()
      {
        typename _Base_type::_Reset __reset(*this);
        return std::move(this->_M_get_result()._M_value());
      }

      shared_future<_Res> share() noexcept;
    };

  /// Partial specialization for future<R&>
  template<typename _Res>
    class future<_Res&> : public __basic_future<_Res&>
    {
      friend class promise<_Res&>;
      template<typename> friend class packaged_task;
      template<typename _Fn, typename... _Args>
        friend future<__async_result_of<_Fn, _Args...>>
        async(launch, _Fn&&, _Args&&...);

      typedef __basic_future<_Res&> _Base_type;
      typedef typename _Base_type::__state_type __state_type;

      explicit
      future(const __state_type& __state) : _Base_type(__state) { }

    public:
      constexpr future() noexcept : _Base_type() { }

      /// Move constructor
      future(future&& __uf) noexcept : _Base_type(std::move(__uf)) { }

      // Disable copying
      future(const future&) = delete;
      future& operator=(const future&) = delete;

      future& operator=(future&& __fut) noexcept
      {
        future(std::move(__fut))._M_swap(*this);
        return *this;
      }

      /// Retrieving the value
      _Res&
      get()
      {
        typename _Base_type::_Reset __reset(*this);
        return this->_M_get_result()._M_get();
      }

      shared_future<_Res&> share() noexcept;
    };

  /// Explicit specialization for future<void>
  template<>
    class future<void> : public __basic_future<void>
    {
      friend class promise<void>;
      template<typename> friend class packaged_task;
      template<typename _Fn, typename... _Args>
        friend future<__async_result_of<_Fn, _Args...>>
        async(launch, _Fn&&, _Args&&...);

      typedef __basic_future<void> _Base_type;
      typedef typename _Base_type::__state_type __state_type;

      explicit
      future(const __state_type& __state) : _Base_type(__state) { }

    public:
      constexpr future() noexcept : _Base_type() { }

      /// Move constructor
      future(future&& __uf) noexcept : _Base_type(std::move(__uf)) { }

      // Disable copying
      future(const future&) = delete;
      future& operator=(const future&) = delete;

      future& operator=(future&& __fut) noexcept
      {
        future(std::move(__fut))._M_swap(*this);
        return *this;
      }

      /// Retrieving the value
      void
      get()
      {
        typename _Base_type::_Reset __reset(*this);
        this->_M_get_result();
      }

      shared_future<void> share() noexcept;
    };


  /// Primary template for shared_future.
  template<typename _Res>
    class shared_future : public __basic_future<_Res>
    {
      typedef __basic_future<_Res> _Base_type;

    public:
      constexpr shared_future() noexcept : _Base_type() { }

      /// Copy constructor
      shared_future(const shared_future& __sf) noexcept : _Base_type(__sf) { }

      /// Construct from a future rvalue
      shared_future(future<_Res>&& __uf) noexcept
      : _Base_type(std::move(__uf))
      { }

      /// Construct from a shared_future rvalue
      shared_future(shared_future&& __sf) noexcept
      : _Base_type(std::move(__sf))
      { }

      shared_future& operator=(const shared_future& __sf) noexcept
      {
        shared_future(__sf)._M_swap(*this);
        return *this;
      }

      shared_future& operator=(shared_future&& __sf) noexcept
      {
        shared_future(std::move(__sf))._M_swap(*this);
        return *this;
      }

      /// Retrieving the value
      const _Res&
      get() const { return this->_M_get_result()._M_value(); }
    };

  /// Partial specialization for shared_future<R&>
  template<typename _Res>
    class shared_future<_Res&> : public __basic_future<_Res&>
    {
      typedef __basic_future<_Res&>           _Base_type;

    public:
      constexpr shared_future() noexcept : _Base_type() { }

      /// Copy constructor
      shared_future(const shared_future& __sf) : _Base_type(__sf) { }

      /// Construct from a future rvalue
      shared_future(future<_Res&>&& __uf) noexcept
      : _Base_type(std::move(__uf))
      { }

      /// Construct from a shared_future rvalue
      shared_future(shared_future&& __sf) noexcept
      : _Base_type(std::move(__sf))
      { }

      shared_future& operator=(const shared_future& __sf)
      {
        shared_future(__sf)._M_swap(*this);
        return *this;
      }

      shared_future& operator=(shared_future&& __sf) noexcept
      {
        shared_future(std::move(__sf))._M_swap(*this);
        return *this;
      }

      /// Retrieving the value
      _Res&
      get() const { return this->_M_get_result()._M_get(); }
    };

  /// Explicit specialization for shared_future<void>
  template<>
    class shared_future<void> : public __basic_future<void>
    {
      typedef __basic_future<void> _Base_type;

    public:
      constexpr shared_future() noexcept : _Base_type() { }

      /// Copy constructor
      shared_future(const shared_future& __sf) : _Base_type(__sf) { }

      /// Construct from a future rvalue
      shared_future(future<void>&& __uf) noexcept
      : _Base_type(std::move(__uf))
      { }

      /// Construct from a shared_future rvalue
      shared_future(shared_future&& __sf) noexcept
      : _Base_type(std::move(__sf))
      { }

      shared_future& operator=(const shared_future& __sf)
      {
        shared_future(__sf)._M_swap(*this);
        return *this;
      }

      shared_future& operator=(shared_future&& __sf) noexcept
      {
        shared_future(std::move(__sf))._M_swap(*this);
        return *this;
      }

      // Retrieving the value
      void
      get() const { this->_M_get_result(); }
    };

  // Now we can define the protected __basic_future constructors.
  template<typename _Res>
    inline __basic_future<_Res>::
    __basic_future(const shared_future<_Res>& __sf) noexcept
    : _M_state(__sf._M_state)
    { }

  template<typename _Res>
    inline __basic_future<_Res>::
    __basic_future(shared_future<_Res>&& __sf) noexcept
    : _M_state(std::move(__sf._M_state))
    { }

  template<typename _Res>
    inline __basic_future<_Res>::
    __basic_future(future<_Res>&& __uf) noexcept
    : _M_state(std::move(__uf._M_state))
    { }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2556. Wide contract for future::share()
  template<typename _Res>
    inline shared_future<_Res>
    future<_Res>::share() noexcept
    { return shared_future<_Res>(std::move(*this)); }

  template<typename _Res>
    inline shared_future<_Res&>
    future<_Res&>::share() noexcept
    { return shared_future<_Res&>(std::move(*this)); }

  inline shared_future<void>
  future<void>::share() noexcept
  { return shared_future<void>(std::move(*this)); }

  /// Primary template for promise
  template<typename _Res>
    class promise
    {
      typedef __future_base::_State_base   _State;
      typedef __future_base::_Result<_Res> _Res_type;
      typedef __future_base::_Ptr<_Res_type>       _Ptr_type;
      template<typename, typename> friend class _State::_Setter;
      friend _State;

      shared_ptr<_State>                        _M_future;
      _Ptr_type                                 _M_storage;

    public:
      promise()
      : _M_future(std::make_shared<_State>()),
   _M_storage(new _Res_type())
      { }

      promise(promise&& __rhs) noexcept
      : _M_future(std::move(__rhs._M_future)),
   _M_storage(std::move(__rhs._M_storage))
      { }

      template<typename _Allocator>
        promise(allocator_arg_t, const _Allocator& __a)
        : _M_future(std::allocate_shared<_State>(__a)),
     _M_storage(__future_base::_S_allocate_result<_Res>(__a))
        { }

      template<typename _Allocator>
        promise(allocator_arg_t, const _Allocator&, promise&& __rhs)
        : _M_future(std::move(__rhs._M_future)),
     _M_storage(std::move(__rhs._M_storage))
        { }

      promise(const promise&) = delete;

      ~promise()
      {
        if (static_cast<bool>(_M_future) && !_M_future.unique())
          _M_future->_M_break_promise(std::move(_M_storage));
      }

      // Assignment
      promise&
      operator=(promise&& __rhs) noexcept
      {
        promise(std::move(__rhs)).swap(*this);
        return *this;
      }

      promise& operator=(const promise&) = delete;

      void
      swap(promise& __rhs) noexcept
      {
        _M_future.swap(__rhs._M_future);
        _M_storage.swap(__rhs._M_storage);
      }

      // Retrieving the result
      future<_Res>
      get_future()
      { return future<_Res>(_M_future); }

      // Setting the result
      void
      set_value(const _Res& __r)
      { _M_future->_M_set_result(_State::__setter(this, __r)); }

      void
      set_value(_Res&& __r)
      { _M_future->_M_set_result(_State::__setter(this, std::move(__r))); }

      void
      set_exception(exception_ptr __p)
      { _M_future->_M_set_result(_State::__setter(__p, this)); }

      void
      set_value_at_thread_exit(const _Res& __r)
      {
   _M_future->_M_set_delayed_result(_State::__setter(this, __r),
                                    _M_future);
      }

      void
      set_value_at_thread_exit(_Res&& __r)
      {
   _M_future->_M_set_delayed_result(
       _State::__setter(this, std::move(__r)), _M_future);
      }

      void
      set_exception_at_thread_exit(exception_ptr __p)
      {
   _M_future->_M_set_delayed_result(_State::__setter(__p, this),
                                    _M_future);
      }
    };

  template<typename _Res>
    inline void
    swap(promise<_Res>& __x, promise<_Res>& __y) noexcept
    { __x.swap(__y); }

  template<typename _Res, typename _Alloc>
    struct uses_allocator<promise<_Res>, _Alloc>
    : public true_type { };


  /// Partial specialization for promise<R&>
  template<typename _Res>
    class promise<_Res&>
    {
      typedef __future_base::_State_base   _State;
      typedef __future_base::_Result<_Res&>        _Res_type;
      typedef __future_base::_Ptr<_Res_type>       _Ptr_type;
      template<typename, typename> friend class _State::_Setter;
      friend _State;

      shared_ptr<_State>                        _M_future;
      _Ptr_type                                 _M_storage;

    public:
      promise()
      : _M_future(std::make_shared<_State>()),
   _M_storage(new _Res_type())
      { }

      promise(promise&& __rhs) noexcept
      : _M_future(std::move(__rhs._M_future)),
   _M_storage(std::move(__rhs._M_storage))
      { }

      template<typename _Allocator>
        promise(allocator_arg_t, const _Allocator& __a)
        : _M_future(std::allocate_shared<_State>(__a)),
     _M_storage(__future_base::_S_allocate_result<_Res&>(__a))
        { }

      template<typename _Allocator>
        promise(allocator_arg_t, const _Allocator&, promise&& __rhs)
        : _M_future(std::move(__rhs._M_future)),
     _M_storage(std::move(__rhs._M_storage))
        { }

      promise(const promise&) = delete;

      ~promise()
      {
        if (static_cast<bool>(_M_future) && !_M_future.unique())
          _M_future->_M_break_promise(std::move(_M_storage));
      }

      // Assignment
      promise&
      operator=(promise&& __rhs) noexcept
      {
        promise(std::move(__rhs)).swap(*this);
        return *this;
      }

      promise& operator=(const promise&) = delete;

      void
      swap(promise& __rhs) noexcept
      {
        _M_future.swap(__rhs._M_future);
        _M_storage.swap(__rhs._M_storage);
      }

      // Retrieving the result
      future<_Res&>
      get_future()
      { return future<_Res&>(_M_future); }

      // Setting the result
      void
      set_value(_Res& __r)
      { _M_future->_M_set_result(_State::__setter(this, __r)); }

      void
      set_exception(exception_ptr __p)
      { _M_future->_M_set_result(_State::__setter(__p, this)); }

      void
      set_value_at_thread_exit(_Res& __r)
      {
   _M_future->_M_set_delayed_result(_State::__setter(this, __r),
                                    _M_future);
      }

      void
      set_exception_at_thread_exit(exception_ptr __p)
      {
   _M_future->_M_set_delayed_result(_State::__setter(__p, this),
                                    _M_future);
      }
    };

  /// Explicit specialization for promise<void>
  template<>
    class promise<void>
    {
      typedef __future_base::_State_base   _State;
      typedef __future_base::_Result<void> _Res_type;
      typedef __future_base::_Ptr<_Res_type>       _Ptr_type;
      template<typename, typename> friend class _State::_Setter;
      friend _State;

      shared_ptr<_State>                        _M_future;
      _Ptr_type                                 _M_storage;

    public:
      promise()
      : _M_future(std::make_shared<_State>()),
   _M_storage(new _Res_type())
      { }

      promise(promise&& __rhs) noexcept
      : _M_future(std::move(__rhs._M_future)),
   _M_storage(std::move(__rhs._M_storage))
      { }

      template<typename _Allocator>
        promise(allocator_arg_t, const _Allocator& __a)
        : _M_future(std::allocate_shared<_State>(__a)),
     _M_storage(__future_base::_S_allocate_result<void>(__a))
        { }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 2095.  missing constructors needed for uses-allocator construction
      template<typename _Allocator>
        promise(allocator_arg_t, const _Allocator&, promise&& __rhs)
        : _M_future(std::move(__rhs._M_future)),
     _M_storage(std::move(__rhs._M_storage))
        { }

      promise(const promise&) = delete;

      ~promise()
      {
        if (static_cast<bool>(_M_future) && !_M_future.unique())
          _M_future->_M_break_promise(std::move(_M_storage));
      }

      // Assignment
      promise&
      operator=(promise&& __rhs) noexcept
      {
        promise(std::move(__rhs)).swap(*this);
        return *this;
      }

      promise& operator=(const promise&) = delete;

      void
      swap(promise& __rhs) noexcept
      {
        _M_future.swap(__rhs._M_future);
        _M_storage.swap(__rhs._M_storage);
      }

      // Retrieving the result
      future<void>
      get_future()
      { return future<void>(_M_future); }

      // Setting the result
      void
      set_value()
      { _M_future->_M_set_result(_State::__setter(this)); }

      void
      set_exception(exception_ptr __p)
      { _M_future->_M_set_result(_State::__setter(__p, this)); }

      void
      set_value_at_thread_exit()
      { _M_future->_M_set_delayed_result(_State::__setter(this), _M_future); }

      void
      set_exception_at_thread_exit(exception_ptr __p)
      {
   _M_future->_M_set_delayed_result(_State::__setter(__p, this),
                                    _M_future);
      }
    };

  template<typename _Ptr_type, typename _Fn, typename _Res>
    struct __future_base::_Task_setter
    {
      // Invoke the function and provide the result to the caller.
      _Ptr_type operator()() const
      {
   __try
     {
       (*_M_result)->_M_set((*_M_fn)());
     }
   __catch(const __cxxabiv1::__forced_unwind&)
     {
       __throw_exception_again; // will cause broken_promise
     }
   __catch(...)
     {
       (*_M_result)->_M_error = current_exception();
     }
   return std::move(*_M_result);
      }
      _Ptr_type*   _M_result;
      _Fn*         _M_fn;
    };

  template<typename _Ptr_type, typename _Fn>
    struct __future_base::_Task_setter<_Ptr_type, _Fn, void>
    {
      _Ptr_type operator()() const
      {
   __try
     {
       (*_M_fn)();
     }
   __catch(const __cxxabiv1::__forced_unwind&)
     {
       __throw_exception_again; // will cause broken_promise
     }
   __catch(...)
     {
       (*_M_result)->_M_error = current_exception();
     }
   return std::move(*_M_result);
      }
      _Ptr_type*   _M_result;
      _Fn*         _M_fn;
    };

  // Holds storage for a packaged_task's result.
  template<typename _Res, typename... _Args>
    struct __future_base::_Task_state_base<_Res(_Args...)>
    : __future_base::_State_base
    {
      typedef _Res _Res_type;

      template<typename _Alloc>
   _Task_state_base(const _Alloc& __a)
   : _M_result(_S_allocate_result<_Res>(__a))
   { }

      // Invoke the stored task and make the state ready.
      virtual void
      _M_run(_Args&&... __args) = 0;

      // Invoke the stored task and make the state ready at thread exit.
      virtual void
      _M_run_delayed(_Args&&... __args, weak_ptr<_State_base>) = 0;

      virtual shared_ptr<_Task_state_base>
      _M_reset() = 0;

      typedef __future_base::_Ptr<_Result<_Res>> _Ptr_type;
      _Ptr_type _M_result;
    };

  // Holds a packaged_task's stored task.
  template<typename _Fn, typename _Alloc, typename _Res, typename... _Args>
    struct __future_base::_Task_state<_Fn, _Alloc, _Res(_Args...)> final
    : __future_base::_Task_state_base<_Res(_Args...)>
    {
      template<typename _Fn2>
   _Task_state(_Fn2&& __fn, const _Alloc& __a)
   : _Task_state_base<_Res(_Args...)>(__a),
     _M_impl(std::forward<_Fn2>(__fn), __a)
   { }

    private:
      virtual void
      _M_run(_Args&&... __args)
      {
   auto __boundfn = [&] () -> typename result_of<_Fn&(_Args&&...)>::type {
       return std::__invoke(_M_impl._M_fn, std::forward<_Args>(__args)...);
   };
   this->_M_set_result(_S_task_setter(this->_M_result, __boundfn));
      }

      virtual void
      _M_run_delayed(_Args&&... __args, weak_ptr<_State_base> __self)
      {
   auto __boundfn = [&] () -> typename result_of<_Fn&(_Args&&...)>::type {
       return std::__invoke(_M_impl._M_fn, std::forward<_Args>(__args)...);
   };
   this->_M_set_delayed_result(_S_task_setter(this->_M_result, __boundfn),
                               std::move(__self));
      }

      virtual shared_ptr<_Task_state_base<_Res(_Args...)>>
      _M_reset();

      struct _Impl : _Alloc
      {
   template<typename _Fn2>
     _Impl(_Fn2&& __fn, const _Alloc& __a)
     : _Alloc(__a), _M_fn(std::forward<_Fn2>(__fn)) { }
   _Fn _M_fn;
      } _M_impl;
    };

  template<typename _Signature, typename _Fn, typename _Alloc>
    static shared_ptr<__future_base::_Task_state_base<_Signature>>
    __create_task_state(_Fn&& __fn, const _Alloc& __a)
    {
      typedef typename decay<_Fn>::type _Fn2;
      typedef __future_base::_Task_state<_Fn2, _Alloc, _Signature> _State;
      return std::allocate_shared<_State>(__a, std::forward<_Fn>(__fn), __a);
    }

  template<typename _Fn, typename _Alloc, typename _Res, typename... _Args>
    shared_ptr<__future_base::_Task_state_base<_Res(_Args...)>>
    __future_base::_Task_state<_Fn, _Alloc, _Res(_Args...)>::_M_reset()
    {
      return __create_task_state<_Res(_Args...)>(std::move(_M_impl._M_fn),
                                            static_cast<_Alloc&>(_M_impl));
    }

  template<typename _Task, typename _Fn, bool
      = is_same<_Task, typename decay<_Fn>::type>::value>
    struct __constrain_pkgdtask
    { typedef void __type; };

  template<typename _Task, typename _Fn>
    struct __constrain_pkgdtask<_Task, _Fn, true>
    { };

  /// packaged_task
  template<typename _Res, typename... _ArgTypes>
    class packaged_task<_Res(_ArgTypes...)>
    {
      typedef __future_base::_Task_state_base<_Res(_ArgTypes...)> _State_type;
      shared_ptr<_State_type>                   _M_state;

    public:
      // Construction and destruction
      packaged_task() noexcept { }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 2095.  missing constructors needed for uses-allocator construction
      template<typename _Allocator>
   packaged_task(allocator_arg_t, const _Allocator& __a) noexcept
   { }

      template<typename _Fn, typename = typename
          __constrain_pkgdtask<packaged_task, _Fn>::__type>
   explicit
   packaged_task(_Fn&& __fn)
   : packaged_task(allocator_arg, std::allocator<int>(),
                   std::forward<_Fn>(__fn))
   { }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 2097.  packaged_task constructors should be constrained
      // 2407. [this constructor should not be] explicit
      template<typename _Fn, typename _Alloc, typename = typename
          __constrain_pkgdtask<packaged_task, _Fn>::__type>
   packaged_task(allocator_arg_t, const _Alloc& __a, _Fn&& __fn)
   : _M_state(__create_task_state<_Res(_ArgTypes...)>(
               std::forward<_Fn>(__fn), __a))
   { }

      ~packaged_task()
      {
        if (static_cast<bool>(_M_state) && !_M_state.unique())
     _M_state->_M_break_promise(std::move(_M_state->_M_result));
      }

      // No copy
      packaged_task(const packaged_task&) = delete;
      packaged_task& operator=(const packaged_task&) = delete;

      template<typename _Allocator>
   packaged_task(allocator_arg_t, const _Allocator&,
                 const packaged_task&) = delete;

      // Move support
      packaged_task(packaged_task&& __other) noexcept
      { this->swap(__other); }

      template<typename _Allocator>
   packaged_task(allocator_arg_t, const _Allocator&,
                 packaged_task&& __other) noexcept
   { this->swap(__other); }

      packaged_task& operator=(packaged_task&& __other) noexcept
      {
   packaged_task(std::move(__other)).swap(*this);
   return *this;
      }

      void
      swap(packaged_task& __other) noexcept
      { _M_state.swap(__other._M_state); }

      bool
      valid() const noexcept
      { return static_cast<bool>(_M_state); }

      // Result retrieval
      future<_Res>
      get_future()
      { return future<_Res>(_M_state); }

      // Execution
      void
      operator()(_ArgTypes... __args)
      {
   __future_base::_State_base::_S_check(_M_state);
   _M_state->_M_run(std::forward<_ArgTypes>(__args)...);
      }

      void
      make_ready_at_thread_exit(_ArgTypes... __args)
      {
   __future_base::_State_base::_S_check(_M_state);
   _M_state->_M_run_delayed(std::forward<_ArgTypes>(__args)..., _M_state);
      }

      void
      reset()
      {
   __future_base::_State_base::_S_check(_M_state);
   packaged_task __tmp;
   __tmp._M_state = _M_state;
   _M_state = _M_state->_M_reset();
      }
    };

  /// swap
  template<typename _Res, typename... _ArgTypes>
    inline void
    swap(packaged_task<_Res(_ArgTypes...)>& __x,
    packaged_task<_Res(_ArgTypes...)>& __y) noexcept
    { __x.swap(__y); }

  template<typename _Res, typename _Alloc>
    struct uses_allocator<packaged_task<_Res>, _Alloc>
    : public true_type { };


  // Shared state created by std::async().
  // Holds a deferred function and storage for its result.
  template<typename _BoundFn, typename _Res>
    class __future_base::_Deferred_state final
    : public __future_base::_State_base
    {
    public:
      explicit
      _Deferred_state(_BoundFn&& __fn)
      : _M_result(new _Result<_Res>()), _M_fn(std::move(__fn))
      { }

    private:
      typedef __future_base::_Ptr<_Result<_Res>> _Ptr_type;
      _Ptr_type _M_result;
      _BoundFn _M_fn;

      // Run the deferred function.
      virtual void
      _M_complete_async()
      {
   // Multiple threads can call a waiting function on the future and
   // reach this point at the same time. The call_once in _M_set_result
   // ensures only the first one run the deferred function, stores the
   // result in _M_result, swaps that with the base _M_result and makes
   // the state ready. Tell _M_set_result to ignore failure so all later
   // calls do nothing.
        _M_set_result(_S_task_setter(_M_result, _M_fn), true);
      }

      // Caller should check whether the state is ready first, because this
      // function will return true even after the deferred function has run.
      virtual bool _M_is_deferred_future() const { return true; }
    };

  // Common functionality hoisted out of the _Async_state_impl template.
  class __future_base::_Async_state_commonV2
    : public __future_base::_State_base
  {
  protected:
    ~_Async_state_commonV2() = default;

    // Make waiting functions block until the thread completes, as if joined.
    //
    // This function is used by wait() to satisfy the first requirement below
    // and by wait_for() / wait_until() to satisfy the second.
    //
    // [futures.async]:
    //
    // \xe2\x80\x94 a call to a waiting function on an asynchronous return object that
    // shares the shared state created by this async call shall block until
    // the associated thread has completed, as if joined, or else time out.
    //
    // \xe2\x80\x94 the associated thread completion synchronizes with the return from
    // the first function that successfully detects the ready status of the
    // shared state or with the return from the last function that releases
    // the shared state, whichever happens first.
    virtual void _M_complete_async() { _M_join(); }

    void _M_join() { std::call_once(_M_once, &thread::join, &_M_thread); }

    thread _M_thread;
    once_flag _M_once;
  };

  // Shared state created by std::async().
  // Starts a new thread that runs a function and makes the shared state ready.
  template<typename _BoundFn, typename _Res>
    class __future_base::_Async_state_impl final
    : public __future_base::_Async_state_commonV2
    {
    public:
      explicit
      _Async_state_impl(_BoundFn&& __fn)
      : _M_result(new _Result<_Res>()), _M_fn(std::move(__fn))
      {
   _M_thread = std::thread{ [this] {
       __try
         {
           _M_set_result(_S_task_setter(_M_result, _M_fn));
         }
       __catch (const __cxxabiv1::__forced_unwind&)
         {
           // make the shared state ready on thread cancellation
           if (static_cast<bool>(_M_result))
             this->_M_break_promise(std::move(_M_result));
           __throw_exception_again;
         }
        } };
      }

      // Must not destroy _M_result and _M_fn until the thread finishes.
      // Call join() directly rather than through _M_join() because no other
      // thread can be referring to this state if it is being destroyed.
      ~_Async_state_impl() { if (_M_thread.joinable()) _M_thread.join(); }

    private:
      typedef __future_base::_Ptr<_Result<_Res>> _Ptr_type;
      _Ptr_type _M_result;
      _BoundFn _M_fn;
    };

  template<typename _BoundFn>
    inline std::shared_ptr<__future_base::_State_base>
    __future_base::_S_make_deferred_state(_BoundFn&& __fn)
    {
      typedef typename remove_reference<_BoundFn>::type __fn_type;
      typedef _Deferred_state<__fn_type> __state_type;
      return std::make_shared<__state_type>(std::move(__fn));
    }

  template<typename _BoundFn>
    inline std::shared_ptr<__future_base::_State_base>
    __future_base::_S_make_async_state(_BoundFn&& __fn)
    {
      typedef typename remove_reference<_BoundFn>::type __fn_type;
      typedef _Async_state_impl<__fn_type> __state_type;
      return std::make_shared<__state_type>(std::move(__fn));
    }


  /// async
  template<typename _Fn, typename... _Args>
    future<__async_result_of<_Fn, _Args...>>
    async(launch __policy, _Fn&& __fn, _Args&&... __args)
    {
      std::shared_ptr<__future_base::_State_base> __state;
      if ((__policy & launch::async) == launch::async)
   {
     __try
       {
         __state = __future_base::_S_make_async_state(
             std::thread::__make_invoker(std::forward<_Fn>(__fn),
                                         std::forward<_Args>(__args)...)
             );
       }
#if __cpp_exceptions
     catch(const system_error& __e)
       {
         if (__e.code() != errc::resource_unavailable_try_again
             || (__policy & launch::deferred) != launch::deferred)
           throw;
       }
#endif
   }
      if (!__state)
   {
     __state = __future_base::_S_make_deferred_state(
         std::thread::__make_invoker(std::forward<_Fn>(__fn),
                                     std::forward<_Args>(__args)...));
   }
      return future<__async_result_of<_Fn, _Args...>>(__state);
    }

  /// async, potential overload
  template<typename _Fn, typename... _Args>
    inline future<__async_result_of<_Fn, _Args...>>
    async(_Fn&& __fn, _Args&&... __args)
    {
      return std::async(launch::async|launch::deferred,
                   std::forward<_Fn>(__fn),
                   std::forward<_Args>(__args)...);
    }

#endif // _GLIBCXX_ASYNC_ABI_COMPAT
#endif // _GLIBCXX_HAS_GTHREADS && _GLIBCXX_USE_C99_STDINT_TR1

  // @} group futures
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif // C++11

#endif // _GLIBCXX_FUTURE
