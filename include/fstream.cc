// File based streams -*- C++ -*-

// Copyright (C) 1997-2018 Free Software Foundation, Inc.
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

/** @file include/fstream
 *  This is a Standard C++ Library header.
 */

//
// ISO C++ 14882: 27.8  File-based streams
//

#ifndef _GLIBCXX_FSTREAM
#define _GLIBCXX_FSTREAM 1

#pragma GCC system_header

#include <istream>
#include <ostream>
#include <bits/codecvt.h>
#include <cstdio>             // For BUFSIZ
#include <bits/basic_file.h>  // For __basic_file, __c_lock
#if __cplusplus >= 201103L
#include <string>             // For std::string overloads.
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#if __cplusplus >= 201703L
  // Enable if _Path is a filesystem::path or experimental::filesystem::path
  template<typename _Path, typename _Result = _Path, typename _Path2
      = decltype(std::declval<_Path&>().make_preferred().filename())>
    using _If_fs_path = enable_if_t<is_same_v<_Path, _Path2>, _Result>;
#endif // C++17


  // [27.8.1.1] template class basic_filebuf
  /**
   *  @brief  The actual work of input and output (for files).
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *
   *  This class associates both its input and output sequence with an
   *  external disk file, and maintains a joint file position for both
   *  sequences.  Many of its semantics are described in terms of similar
   *  behavior in the Standard C Library's @c FILE streams.
   *
   *  Requirements on traits_type, specific to this class:
   *  - traits_type::pos_type must be fpos<traits_type::state_type>
   *  - traits_type::off_type must be streamoff
   *  - traits_type::state_type must be Assignable and DefaultConstructible,
   *  - traits_type::state_type() must be the initial state for codecvt.
   */
  template<typename _CharT, typename _Traits>
    class basic_filebuf : public basic_streambuf<_CharT, _Traits>
    {
#if __cplusplus >= 201103L
      template<typename _Tp>
   using __chk_state = __and_<is_copy_assignable<_Tp>,
                              is_copy_constructible<_Tp>,
                              is_default_constructible<_Tp>>;

      static_assert(__chk_state<typename _Traits::state_type>::value,
               "state_type must be CopyAssignable, CopyConstructible"
               " and DefaultConstructible");

      static_assert(is_same<typename _Traits::pos_type,
                       fpos<typename _Traits::state_type>>::value,
               "pos_type must be fpos<state_type>");
#endif
    public:
      // Types:
      typedef _CharT                               char_type;
      typedef _Traits                              traits_type;
      typedef typename traits_type::int_type               int_type;
      typedef typename traits_type::pos_type               pos_type;
      typedef typename traits_type::off_type               off_type;

      typedef basic_streambuf<char_type, traits_type>      __streambuf_type;
      typedef basic_filebuf<char_type, traits_type>     __filebuf_type;
      typedef __basic_file<char>                   __file_type;
      typedef typename traits_type::state_type          __state_type;
      typedef codecvt<char_type, char, __state_type>    __codecvt_type;

      friend class ios_base; // For sync_with_stdio.

    protected:
      // Data Members:
      // MT lock inherited from libio or other low-level io library.
      __c_lock             _M_lock;

      // External buffer.
      __file_type          _M_file;

      /// Place to stash in || out || in | out settings for current filebuf.
      ios_base::openmode   _M_mode;

      // Beginning state type for codecvt.
      __state_type                 _M_state_beg;

      // During output, the state that corresponds to pptr(),
      // during input, the state that corresponds to egptr() and
      // _M_ext_next.
      __state_type         _M_state_cur;

      // Not used for output. During input, the state that corresponds
      // to eback() and _M_ext_buf.
      __state_type         _M_state_last;

      /// Pointer to the beginning of internal buffer.
      char_type*           _M_buf;

      /**
       *  Actual size of internal buffer. This number is equal to the size
       *  of the put area + 1 position, reserved for the overflow char of
       *  a full area.
       */
      size_t                       _M_buf_size;

      // Set iff _M_buf is allocated memory from _M_allocate_internal_buffer.
      bool                 _M_buf_allocated;

      /**
       *  _M_reading == false && _M_writing == false for @b uncommitted mode;
       *  _M_reading == true for @b read mode;
       *  _M_writing == true for @b write mode;
       *
       *  NB: _M_reading == true && _M_writing == true is unused.
       */
      bool                      _M_reading;
      bool                      _M_writing;

      //@{
      /**
       *  Necessary bits for putback buffer management.
       *
       *  @note pbacks of over one character are not currently supported.
       */
      char_type                    _M_pback;
      char_type*           _M_pback_cur_save;
      char_type*           _M_pback_end_save;
      bool                 _M_pback_init;
      //@}

      // Cached codecvt facet.
      const __codecvt_type*        _M_codecvt;

      /**
       *  Buffer for external characters. Used for input when
       *  codecvt::always_noconv() == false. When valid, this corresponds
       *  to eback().
       */
      char*                        _M_ext_buf;

      /**
       *  Size of buffer held by _M_ext_buf.
       */
      streamsize           _M_ext_buf_size;

      /**
       *  Pointers into the buffer held by _M_ext_buf that delimit a
       *  subsequence of bytes that have been read but not yet converted.
       *  When valid, _M_ext_next corresponds to egptr().
       */
      const char*          _M_ext_next;
      char*                        _M_ext_end;

      /**
       *  Initializes pback buffers, and moves normal buffers to safety.
       *  Assumptions:
       *  _M_in_cur has already been moved back
       */
      void
      _M_create_pback()
      {
   if (!_M_pback_init)
     {
       _M_pback_cur_save = this->gptr();
       _M_pback_end_save = this->egptr();
       this->setg(&_M_pback, &_M_pback, &_M_pback + 1);
       _M_pback_init = true;
     }
      }

      /**
       *  Deactivates pback buffer contents, and restores normal buffer.
       *  Assumptions:
       *  The pback buffer has only moved forward.
       */
      void
      _M_destroy_pback() throw()
      {
   if (_M_pback_init)
     {
       // Length _M_in_cur moved in the pback buffer.
       _M_pback_cur_save += this->gptr() != this->eback();
       this->setg(_M_buf, _M_pback_cur_save, _M_pback_end_save);
       _M_pback_init = false;
     }
      }

    public:
      // Constructors/destructor:
      /**
       *  @brief  Does not open any files.
       *
       *  The default constructor initializes the parent class using its
       *  own default ctor.
       */
      basic_filebuf();

#if __cplusplus >= 201103L
      basic_filebuf(const basic_filebuf&) = delete;
      basic_filebuf(basic_filebuf&&);
#endif

      /**
       *  @brief  The destructor closes the file first.
       */
      virtual
      ~basic_filebuf()
      { this->close(); }

#if __cplusplus >= 201103L
      basic_filebuf& operator=(const basic_filebuf&) = delete;
      basic_filebuf& operator=(basic_filebuf&&);
      void swap(basic_filebuf&);
#endif

      // Members:
      /**
       *  @brief  Returns true if the external file is open.
       */
      bool
      is_open() const throw()
      { return _M_file.is_open(); }

      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file.
       *  @param  __mode  The open mode flags.
       *  @return  @c this on success, NULL on failure
       *
       *  If a file is already open, this function immediately fails.
       *  Otherwise it tries to open the file named @a __s using the flags
       *  given in @a __mode.
       *
       *  Table 92, adapted here, gives the relation between openmode
       *  combinations and the equivalent @c fopen() flags.
       *  (NB: lines app, in|out|app, in|app, binary|app, binary|in|out|app,
       *  and binary|in|app per DR 596)
       *  <pre>
       *  +---------------------------------------------------------+
       *  | ios_base Flag combination            stdio equivalent   |
       *  |binary  in  out  trunc  app                              |
       *  +---------------------------------------------------------+
       *  |             +                        w                  |
       *  |             +           +            a                  |
       *  |                         +            a                  |
       *  |             +     +                  w                  |
       *  |         +                            r                  |
       *  |         +   +                        r+                 |
       *  |         +   +     +                  w+                 |
       *  |         +   +           +            a+                 |
       *  |         +               +            a+                 |
       *  +---------------------------------------------------------+
       *  |   +         +                        wb                 |
       *  |   +         +           +            ab                 |
       *  |   +                     +            ab                 |
       *  |   +         +     +                  wb                 |
       *  |   +     +                            rb                 |
       *  |   +     +   +                        r+b                |
       *  |   +     +   +     +                  w+b                |
       *  |   +     +   +           +            a+b                |
       *  |   +     +               +            a+b                |
       *  +---------------------------------------------------------+
       *  </pre>
       */
      __filebuf_type*
      open(const char* __s, ios_base::openmode __mode);

#if __cplusplus >= 201103L
      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file.
       *  @param  __mode  The open mode flags.
       *  @return  @c this on success, NULL on failure
       */
      __filebuf_type*
      open(const std::string& __s, ios_base::openmode __mode)
      { return open(__s.c_str(), __mode); }

#if __cplusplus >= 201703L
      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file, as a filesystem::path.
       *  @param  __mode  The open mode flags.
       *  @return  @c this on success, NULL on failure
       */
      template<typename _Path>
   _If_fs_path<_Path, __filebuf_type*>
   open(const _Path& __s, ios_base::openmode __mode)
   { return open(__s.c_str(), __mode); }
#endif // C++17
#endif // C++11

      /**
       *  @brief  Closes the currently associated file.
       *  @return  @c this on success, NULL on failure
       *
       *  If no file is currently open, this function immediately fails.
       *
       *  If a <em>put buffer area</em> exists, @c overflow(eof) is
       *  called to flush all the characters.  The file is then
       *  closed.
       *
       *  If any operations fail, this function also fails.
       */
      __filebuf_type*
      close();

    protected:
      void
      _M_allocate_internal_buffer();

      void
      _M_destroy_internal_buffer() throw();

      // [27.8.1.4] overridden virtual functions
      virtual streamsize
      showmanyc();

      // Stroustrup, 1998, p. 628
      // underflow() and uflow() functions are called to get the next
      // character from the real input source when the buffer is empty.
      // Buffered input uses underflow()

      virtual int_type
      underflow();

      virtual int_type
      pbackfail(int_type __c = _Traits::eof());

      // Stroustrup, 1998, p 648
      // The overflow() function is called to transfer characters to the
      // real output destination when the buffer is full. A call to
      // overflow(c) outputs the contents of the buffer plus the
      // character c.
      // 27.5.2.4.5
      // Consume some sequence of the characters in the pending sequence.
      virtual int_type
      overflow(int_type __c = _Traits::eof());

      // Convert internal byte sequence to external, char-based
      // sequence via codecvt.
      bool
      _M_convert_to_external(char_type*, streamsize);

      /**
       *  @brief  Manipulates the buffer.
       *  @param  __s  Pointer to a buffer area.
       *  @param  __n  Size of @a __s.
       *  @return  @c this
       *
       *  If no file has been opened, and both @a __s and @a __n are zero, then
       *  the stream becomes unbuffered.  Otherwise, @c __s is used as a
       *  buffer; see
       *  https://gcc.gnu.org/onlinedocs/libstdc++/manual/streambufs.html#io.streambuf.buffering
       *  for more.
       */
      virtual __streambuf_type*
      setbuf(char_type* __s, streamsize __n);

      virtual pos_type
      seekoff(off_type __off, ios_base::seekdir __way,
         ios_base::openmode __mode = ios_base::in | ios_base::out);

      virtual pos_type
      seekpos(pos_type __pos,
         ios_base::openmode __mode = ios_base::in | ios_base::out);

      // Common code for seekoff, seekpos, and overflow
      pos_type
      _M_seek(off_type __off, ios_base::seekdir __way, __state_type __state);

      int
      _M_get_ext_pos(__state_type &__state);

      virtual int
      sync();

      virtual void
      imbue(const locale& __loc);

      virtual streamsize
      xsgetn(char_type* __s, streamsize __n);

      virtual streamsize
      xsputn(const char_type* __s, streamsize __n);

      // Flushes output buffer, then writes unshift sequence.
      bool
      _M_terminate_output();

      /**
       *  This function sets the pointers of the internal buffer, both get
       *  and put areas. Typically:
       *
       *   __off == egptr() - eback() upon underflow/uflow (@b read mode);
       *   __off == 0 upon overflow (@b write mode);
       *   __off == -1 upon open, setbuf, seekoff/pos (@b uncommitted mode).
       *
       *  NB: epptr() - pbase() == _M_buf_size - 1, since _M_buf_size
       *  reflects the actual allocated memory and the last cell is reserved
       *  for the overflow char of a full put area.
       */
      void
      _M_set_buffer(streamsize __off)
      {
   const bool __testin = _M_mode & ios_base::in;
   const bool __testout = (_M_mode & ios_base::out
                           || _M_mode & ios_base::app);

   if (__testin && __off > 0)
     this->setg(_M_buf, _M_buf, _M_buf + __off);
   else
     this->setg(_M_buf, _M_buf, _M_buf);

   if (__testout && __off == 0 && _M_buf_size > 1 )
     this->setp(_M_buf, _M_buf + _M_buf_size - 1);
   else
     this->setp(0, 0);
      }
    };

  // [27.8.1.5] Template class basic_ifstream
  /**
   *  @brief  Controlling input for files.
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *
   *  This class supports reading from named files, using the inherited
   *  functions from std::basic_istream.  To control the associated
   *  sequence, an instance of std::basic_filebuf is used, which this page
   *  refers to as @c sb.
   */
  template<typename _CharT, typename _Traits>
    class basic_ifstream : public basic_istream<_CharT, _Traits>
    {
    public:
      // Types:
      typedef _CharT                                       char_type;
      typedef _Traits                                      traits_type;
      typedef typename traits_type::int_type               int_type;
      typedef typename traits_type::pos_type               pos_type;
      typedef typename traits_type::off_type               off_type;

      // Non-standard types:
      typedef basic_filebuf<char_type, traits_type>        __filebuf_type;
      typedef basic_istream<char_type, traits_type>        __istream_type;

    private:
      __filebuf_type       _M_filebuf;

    public:
      // Constructors/Destructors:
      /**
       *  @brief  Default constructor.
       *
       *  Initializes @c sb using its default constructor, and passes
       *  @c &sb to the base class initializer.  Does not open any files
       *  (you haven't given it a filename to open).
       */
      basic_ifstream() : __istream_type(), _M_filebuf()
      { this->init(&_M_filebuf); }

      /**
       *  @brief  Create an input file stream.
       *  @param  __s  Null terminated string specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       *
       *  @c ios_base::in is automatically included in @a __mode.
       */
      explicit
      basic_ifstream(const char* __s, ios_base::openmode __mode = ios_base::in)
      : __istream_type(), _M_filebuf()
      {
   this->init(&_M_filebuf);
   this->open(__s, __mode);
      }

#if __cplusplus >= 201103L
      /**
       *  @brief  Create an input file stream.
       *  @param  __s  std::string specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       *
       *  @c ios_base::in is automatically included in @a __mode.
       */
      explicit
      basic_ifstream(const std::string& __s,
                ios_base::openmode __mode = ios_base::in)
      : __istream_type(), _M_filebuf()
      {
   this->init(&_M_filebuf);
   this->open(__s, __mode);
      }

#if __cplusplus >= 201703L
      /**
       *  @param  Create an input file stream.
       *  @param  __s  filesystem::path specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       *
       *  @c ios_base::in is automatically included in @a __mode.
       */
      template<typename _Path, typename _Require = _If_fs_path<_Path>>
   basic_ifstream(const _Path& __s,
                  ios_base::openmode __mode = ios_base::in)
   : basic_ifstream(__s.c_str(), __mode)
   { }
#endif // C++17

      basic_ifstream(const basic_ifstream&) = delete;

      basic_ifstream(basic_ifstream&& __rhs)
      : __istream_type(std::move(__rhs)),
      _M_filebuf(std::move(__rhs._M_filebuf))
      { __istream_type::set_rdbuf(&_M_filebuf); }
#endif // C++11

      /**
       *  @brief  The destructor does nothing.
       *
       *  The file is closed by the filebuf object, not the formatting
       *  stream.
       */
      ~basic_ifstream()
      { }

#if __cplusplus >= 201103L
      // 27.8.3.2 Assign and swap:

      basic_ifstream&
      operator=(const basic_ifstream&) = delete;

      basic_ifstream&
      operator=(basic_ifstream&& __rhs)
      {
   __istream_type::operator=(std::move(__rhs));
   _M_filebuf = std::move(__rhs._M_filebuf);
   return *this;
      }

      void
      swap(basic_ifstream& __rhs)
      {
   __istream_type::swap(__rhs);
   _M_filebuf.swap(__rhs._M_filebuf);
      }
#endif

      // Members:
      /**
       *  @brief  Accessing the underlying buffer.
       *  @return  The current basic_filebuf buffer.
       *
       *  This hides both signatures of std::basic_ios::rdbuf().
       */
      __filebuf_type*
      rdbuf() const
      { return const_cast<__filebuf_type*>(&_M_filebuf); }

      /**
       *  @brief  Wrapper to test for an open file.
       *  @return  @c rdbuf()->is_open()
       */
      bool
      is_open()
      { return _M_filebuf.is_open(); }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 365. Lack of const-qualification in clause 27
      bool
      is_open() const
      { return _M_filebuf.is_open(); }

      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(s,__mode|in).  If that function
       *  fails, @c failbit is set in the stream's error state.
       */
      void
      open(const char* __s, ios_base::openmode __mode = ios_base::in)
      {
   if (!_M_filebuf.open(__s, __mode | ios_base::in))
     this->setstate(ios_base::failbit);
   else
     // _GLIBCXX_RESOLVE_LIB_DEFECTS
     // 409. Closing an fstream should clear error state
     this->clear();
      }

#if __cplusplus >= 201103L
      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(__s,__mode|in).  If that function
       *  fails, @c failbit is set in the stream's error state.
       */
      void
      open(const std::string& __s, ios_base::openmode __mode = ios_base::in)
      {
   if (!_M_filebuf.open(__s, __mode | ios_base::in))
     this->setstate(ios_base::failbit);
   else
     // _GLIBCXX_RESOLVE_LIB_DEFECTS
     // 409. Closing an fstream should clear error state
     this->clear();
      }

#if __cplusplus >= 201703L
      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file, as a filesystem::path.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(__s,__mode|in).  If that function
       *  fails, @c failbit is set in the stream's error state.
       */
      template<typename _Path>
   _If_fs_path<_Path, void>
   open(const _Path& __s, ios_base::openmode __mode = ios_base::in)
   { open(__s.c_str(), __mode); }
#endif // C++17
#endif // C++11

      /**
       *  @brief  Close the file.
       *
       *  Calls @c std::basic_filebuf::close().  If that function
       *  fails, @c failbit is set in the stream's error state.
       */
      void
      close()
      {
   if (!_M_filebuf.close())
     this->setstate(ios_base::failbit);
      }
    };


  // [27.8.1.8] Template class basic_ofstream
  /**
   *  @brief  Controlling output for files.
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *
   *  This class supports reading from named files, using the inherited
   *  functions from std::basic_ostream.  To control the associated
   *  sequence, an instance of std::basic_filebuf is used, which this page
   *  refers to as @c sb.
   */
  template<typename _CharT, typename _Traits>
    class basic_ofstream : public basic_ostream<_CharT,_Traits>
    {
    public:
      // Types:
      typedef _CharT                                       char_type;
      typedef _Traits                                      traits_type;
      typedef typename traits_type::int_type               int_type;
      typedef typename traits_type::pos_type               pos_type;
      typedef typename traits_type::off_type               off_type;

      // Non-standard types:
      typedef basic_filebuf<char_type, traits_type>        __filebuf_type;
      typedef basic_ostream<char_type, traits_type>        __ostream_type;

    private:
      __filebuf_type       _M_filebuf;

    public:
      // Constructors:
      /**
       *  @brief  Default constructor.
       *
       *  Initializes @c sb using its default constructor, and passes
       *  @c &sb to the base class initializer.  Does not open any files
       *  (you haven't given it a filename to open).
       */
      basic_ofstream(): __ostream_type(), _M_filebuf()
      { this->init(&_M_filebuf); }

      /**
       *  @brief  Create an output file stream.
       *  @param  __s  Null terminated string specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       *
       *  @c ios_base::out is automatically included in @a __mode.
       */
      explicit
      basic_ofstream(const char* __s,
                ios_base::openmode __mode = ios_base::out)
      : __ostream_type(), _M_filebuf()
      {
   this->init(&_M_filebuf);
   this->open(__s, __mode);
      }

#if __cplusplus >= 201103L
      /**
       *  @brief  Create an output file stream.
       *  @param  __s  std::string specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       *
       *  @c ios_base::out is automatically included in @a __mode.
       */
      explicit
      basic_ofstream(const std::string& __s,
                ios_base::openmode __mode = ios_base::out)
      : __ostream_type(), _M_filebuf()
      {
   this->init(&_M_filebuf);
   this->open(__s, __mode);
      }

#if __cplusplus >= 201703L
      /**
       *  @param  Create an output file stream.
       *  @param  __s  filesystem::path specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       *
       *  @c ios_base::out is automatically included in @a __mode.
       */
      template<typename _Path, typename _Require = _If_fs_path<_Path>>
   basic_ofstream(const _Path& __s,
                  ios_base::openmode __mode = ios_base::out)
   : basic_ofstream(__s.c_str(), __mode)
   { }
#endif // C++17

      basic_ofstream(const basic_ofstream&) = delete;

      basic_ofstream(basic_ofstream&& __rhs)
      : __ostream_type(std::move(__rhs)),
      _M_filebuf(std::move(__rhs._M_filebuf))
      { __ostream_type::set_rdbuf(&_M_filebuf); }
#endif

      /**
       *  @brief  The destructor does nothing.
       *
       *  The file is closed by the filebuf object, not the formatting
       *  stream.
       */
      ~basic_ofstream()
      { }

#if __cplusplus >= 201103L
      // 27.8.3.2 Assign and swap:

      basic_ofstream&
      operator=(const basic_ofstream&) = delete;

      basic_ofstream&
      operator=(basic_ofstream&& __rhs)
      {
   __ostream_type::operator=(std::move(__rhs));
   _M_filebuf = std::move(__rhs._M_filebuf);
   return *this;
      }

      void
      swap(basic_ofstream& __rhs)
      {
   __ostream_type::swap(__rhs);
   _M_filebuf.swap(__rhs._M_filebuf);
      }
#endif

      // Members:
      /**
       *  @brief  Accessing the underlying buffer.
       *  @return  The current basic_filebuf buffer.
       *
       *  This hides both signatures of std::basic_ios::rdbuf().
       */
      __filebuf_type*
      rdbuf() const
      { return const_cast<__filebuf_type*>(&_M_filebuf); }

      /**
       *  @brief  Wrapper to test for an open file.
       *  @return  @c rdbuf()->is_open()
       */
      bool
      is_open()
      { return _M_filebuf.is_open(); }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 365. Lack of const-qualification in clause 27
      bool
      is_open() const
      { return _M_filebuf.is_open(); }

      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(__s,__mode|out).  If that
       *  function fails, @c failbit is set in the stream's error state.
       */
      void
      open(const char* __s, ios_base::openmode __mode = ios_base::out)
      {
   if (!_M_filebuf.open(__s, __mode | ios_base::out))
     this->setstate(ios_base::failbit);
   else
     // _GLIBCXX_RESOLVE_LIB_DEFECTS
     // 409. Closing an fstream should clear error state
     this->clear();
      }

#if __cplusplus >= 201103L
      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(s,mode|out).  If that
       *  function fails, @c failbit is set in the stream's error state.
       */
      void
      open(const std::string& __s, ios_base::openmode __mode = ios_base::out)
      {
   if (!_M_filebuf.open(__s, __mode | ios_base::out))
     this->setstate(ios_base::failbit);
   else
     // _GLIBCXX_RESOLVE_LIB_DEFECTS
     // 409. Closing an fstream should clear error state
     this->clear();
      }

#if __cplusplus >= 201703L
      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file, as a filesystem::path.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(__s,__mode|out).  If that
       *  function fails, @c failbit is set in the stream's error state.
       */
      template<typename _Path>
   _If_fs_path<_Path, void>
   open(const _Path& __s, ios_base::openmode __mode = ios_base::out)
   { open(__s.c_str(), __mode); }
#endif // C++17
#endif // C++11

      /**
       *  @brief  Close the file.
       *
       *  Calls @c std::basic_filebuf::close().  If that function
       *  fails, @c failbit is set in the stream's error state.
       */
      void
      close()
      {
   if (!_M_filebuf.close())
     this->setstate(ios_base::failbit);
      }
    };


  // [27.8.1.11] Template class basic_fstream
  /**
   *  @brief  Controlling input and output for files.
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *
   *  This class supports reading from and writing to named files, using
   *  the inherited functions from std::basic_iostream.  To control the
   *  associated sequence, an instance of std::basic_filebuf is used, which
   *  this page refers to as @c sb.
   */
  template<typename _CharT, typename _Traits>
    class basic_fstream : public basic_iostream<_CharT, _Traits>
    {
    public:
      // Types:
      typedef _CharT                                       char_type;
      typedef _Traits                                      traits_type;
      typedef typename traits_type::int_type               int_type;
      typedef typename traits_type::pos_type               pos_type;
      typedef typename traits_type::off_type               off_type;

      // Non-standard types:
      typedef basic_filebuf<char_type, traits_type>        __filebuf_type;
      typedef basic_ios<char_type, traits_type>            __ios_type;
      typedef basic_iostream<char_type, traits_type>       __iostream_type;

    private:
      __filebuf_type       _M_filebuf;

    public:
      // Constructors/destructor:
      /**
       *  @brief  Default constructor.
       *
       *  Initializes @c sb using its default constructor, and passes
       *  @c &sb to the base class initializer.  Does not open any files
       *  (you haven't given it a filename to open).
       */
      basic_fstream()
      : __iostream_type(), _M_filebuf()
      { this->init(&_M_filebuf); }

      /**
       *  @brief  Create an input/output file stream.
       *  @param  __s  Null terminated string specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       */
      explicit
      basic_fstream(const char* __s,
               ios_base::openmode __mode = ios_base::in | ios_base::out)
      : __iostream_type(0), _M_filebuf()
      {
   this->init(&_M_filebuf);
   this->open(__s, __mode);
      }

#if __cplusplus >= 201103L
      /**
       *  @brief  Create an input/output file stream.
       *  @param  __s  Null terminated string specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       */
      explicit
      basic_fstream(const std::string& __s,
               ios_base::openmode __mode = ios_base::in | ios_base::out)
      : __iostream_type(0), _M_filebuf()
      {
   this->init(&_M_filebuf);
   this->open(__s, __mode);
      }

#if __cplusplus >= 201703L
      /**
       *  @param  Create an input/output file stream.
       *  @param  __s  filesystem::path specifying the filename.
       *  @param  __mode  Open file in specified mode (see std::ios_base).
       */
      template<typename _Path, typename _Require = _If_fs_path<_Path>>
   basic_fstream(const _Path& __s,
                 ios_base::openmode __mode = ios_base::in | ios_base::out)
   : basic_fstream(__s.c_str(), __mode)
   { }
#endif // C++17

      basic_fstream(const basic_fstream&) = delete;

      basic_fstream(basic_fstream&& __rhs)
      : __iostream_type(std::move(__rhs)),
      _M_filebuf(std::move(__rhs._M_filebuf))
      { __iostream_type::set_rdbuf(&_M_filebuf); }
#endif

      /**
       *  @brief  The destructor does nothing.
       *
       *  The file is closed by the filebuf object, not the formatting
       *  stream.
       */
      ~basic_fstream()
      { }

#if __cplusplus >= 201103L
      // 27.8.3.2 Assign and swap:

      basic_fstream&
      operator=(const basic_fstream&) = delete;

      basic_fstream&
      operator=(basic_fstream&& __rhs)
      {
   __iostream_type::operator=(std::move(__rhs));
   _M_filebuf = std::move(__rhs._M_filebuf);
   return *this;
      }

      void
      swap(basic_fstream& __rhs)
      {
   __iostream_type::swap(__rhs);
   _M_filebuf.swap(__rhs._M_filebuf);
      }
#endif

      // Members:
      /**
       *  @brief  Accessing the underlying buffer.
       *  @return  The current basic_filebuf buffer.
       *
       *  This hides both signatures of std::basic_ios::rdbuf().
       */
      __filebuf_type*
      rdbuf() const
      { return const_cast<__filebuf_type*>(&_M_filebuf); }

      /**
       *  @brief  Wrapper to test for an open file.
       *  @return  @c rdbuf()->is_open()
       */
      bool
      is_open()
      { return _M_filebuf.is_open(); }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 365. Lack of const-qualification in clause 27
      bool
      is_open() const
      { return _M_filebuf.is_open(); }

      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(__s,__mode).  If that
       *  function fails, @c failbit is set in the stream's error state.
       */
      void
      open(const char* __s,
      ios_base::openmode __mode = ios_base::in | ios_base::out)
      {
   if (!_M_filebuf.open(__s, __mode))
     this->setstate(ios_base::failbit);
   else
     // _GLIBCXX_RESOLVE_LIB_DEFECTS
     // 409. Closing an fstream should clear error state
     this->clear();
      }

#if __cplusplus >= 201103L
      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(__s,__mode).  If that
       *  function fails, @c failbit is set in the stream's error state.
       */
      void
      open(const std::string& __s,
      ios_base::openmode __mode = ios_base::in | ios_base::out)
      {
   if (!_M_filebuf.open(__s, __mode))
     this->setstate(ios_base::failbit);
   else
     // _GLIBCXX_RESOLVE_LIB_DEFECTS
     // 409. Closing an fstream should clear error state
     this->clear();
      }

#if __cplusplus >= 201703L
      /**
       *  @brief  Opens an external file.
       *  @param  __s  The name of the file, as a filesystem::path.
       *  @param  __mode  The open mode flags.
       *
       *  Calls @c std::basic_filebuf::open(__s,__mode).  If that
       *  function fails, @c failbit is set in the stream's error state.
       */
      template<typename _Path>
   _If_fs_path<_Path, void>
   open(const _Path& __s,
        ios_base::openmode __mode = ios_base::in | ios_base::out)
   { open(__s.c_str(), __mode); }
#endif // C++17
#endif // C++11

      /**
       *  @brief  Close the file.
       *
       *  Calls @c std::basic_filebuf::close().  If that function
       *  fails, @c failbit is set in the stream's error state.
       */
      void
      close()
      {
   if (!_M_filebuf.close())
     this->setstate(ios_base::failbit);
      }
    };

#if __cplusplus >= 201103L
  /// Swap specialization for filebufs.
  template <class _CharT, class _Traits>
    inline void
    swap(basic_filebuf<_CharT, _Traits>& __x,
    basic_filebuf<_CharT, _Traits>& __y)
    { __x.swap(__y); }

  /// Swap specialization for ifstreams.
  template <class _CharT, class _Traits>
    inline void
    swap(basic_ifstream<_CharT, _Traits>& __x,
    basic_ifstream<_CharT, _Traits>& __y)
    { __x.swap(__y); }

  /// Swap specialization for ofstreams.
  template <class _CharT, class _Traits>
    inline void
    swap(basic_ofstream<_CharT, _Traits>& __x,
    basic_ofstream<_CharT, _Traits>& __y)
    { __x.swap(__y); }

  /// Swap specialization for fstreams.
  template <class _CharT, class _Traits>
    inline void
    swap(basic_fstream<_CharT, _Traits>& __x,
    basic_fstream<_CharT, _Traits>& __y)
    { __x.swap(__y); }
#endif

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#include <bits/fstream.tcc>

#endif /* _GLIBCXX_FSTREAM */
