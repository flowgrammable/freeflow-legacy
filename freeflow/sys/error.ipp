// Copyright (c) 2013-2014 Flowgrammable, LLC.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an "AS IS"
// BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing
// permissions and limitations under the License.

namespace freeflow {

// -------------------------------------------------------------------------- //
// Error

/// Construct an error value that signifies success.
inline 
Error::Error() 
  : code_(), data_() { }

/// Construt an error value with the given code. Additiional data
/// may also be associated with the error.
inline
Error::Error(Code c, Data d) 
  : code_(c), data_(d) { }

/// Allows contextual conversion to bool, returning true if and only
/// if there is no error (i.e., the underlying code has value 0).
inline
Error::operator bool() const { return !code_; }

/// Returns the error category.
inline const Error_category&
Error::category() const { return code_.category(); }

/// Returns the error code.
inline Error::Code 
Error::code() const { return code_; }

/// Returns associated error data.
inline Error::Data
Error::data() const { return data_; }

inline std::string
Error::message() const { return code_.message(); }

/// Returns true when two errors have the same error code.
inline bool 
operator==(Error a, Error b) { return a.code() == b.code(); }

inline bool 
operator!=(Error a, Error b) { return not(a == b); }


// -------------------------------------------------------------------------- //
// Error constructors

/// Returns an error condition based on a predicate.
///
/// If the condition b is true, the resulting error condition will evaluate
/// to SUCCESS. Otherwise, the condition will evaluate to the code c.
inline Error 
ok(bool b, Error err) { return b ? Error() : err; }

/// Returns an error code that encapsulates the current system error.
inline Error
system_error() { return system_error(errno); }

/// Returns an error value corresponding to the given system error.
inline Error
system_error(int n) { return make_error_code(static_cast<std::errc>(n)); }

// -------------------------------------------------------------------------- //
// Expected value

namespace impl {
template<typename T>
  inline
  Expected_value<T>::Expected_value(const T& x)
    : value(x) { }

template<typename T>
  inline 
  Expected_value<T>::Expected_value(T&& x)
    : value(std::move(x)) { }

template<typename T>
  inline
  Expected_value<T>::Expected_value(Error e)
    : error(e) { }
}

template<typename T>
  inline
  Expected<T>::Expected(const Expected& x)
    : which_(x.which_)
  { construct(x); }

template<typename T>
  inline Expected<T>&
  Expected<T>::operator=(const Expected& x) {
    destroy();
    which_ = x.which_;
    init(x);
    return *this;
  }

template<typename T>
  inline
  Expected<T>::Expected(Expected&& x)
    : which_(x.which_)
  { construct(std::move(x)); }

template<typename T>
  inline Expected<T>&
  Expected<T>::operator=(Expected&& x) {
    destroy();
    which_ = x.which_;
    init(std::move(x));
    return *this;
  }

template<typename T>
  inline
  Expected<T>::Expected(const T& x)
    : which_(SUCCESS), data_(x) { }

template<typename T>
  inline
  Expected<T>::Expected(T&& x)
    : which_(SUCCESS), data_(std::move(x)) { }

template<typename T>
  inline
  Expected<T>::Expected(Error e)
    : which_(FAILURE), data_(e) { }

/// Allows contextual conversion to bool. Returns true when the expected
/// value is not an error.
template<typename T>
  inline
  Expected<T>::operator bool() const { return which_; }

/// Get the expected value. Behavior is undefined if this object indicates
/// an error.
template<typename T>
  inline const T&
  Expected<T>::get() const {
    assert(which_);
    return data_.value;
  }

/// Move the expected value from this object. Behavior is undefined if
/// this object indicates an error.
template<typename T>
  inline T&&
  Expected<T>::take() {
    assert(which_);
    return std::move(data_.value);
  }

template<typename T>
  inline Error
  Expected<T>::error() const {
    assert(!which_);
    return data_.error;
  }

template<typename T>
  inline void
  Expected<T>::construct(const Expected& x) {
    if (which_)
      new (&data_.value) T(x.data_.value);
    else
      new (&data_.error) Error(x.data_.error);
  }

template<typename T>
  inline void
  Expected<T>::construct(Expected&& x) {
    if (which_)
      new (&data_.value) T(std::move(x.data_.value));
    else
      new (&data_.error) Error(std::move(x.data_.error));
  }

template<typename T>
  inline void
  Expected<T>::destroy() {
    if (which_)
      data_.value.~T();
    else
      data_.value.~Error();
  }


// -------------------------------------------------------------------------- //
// Trap

/// Initialzie the trap with an error. This allows implicit conversions
/// to trap objects.
inline
Trap::Trap(Error e)
  : err_(e) { }

/// Allows contextual conversion to bool, returning true if and only if
/// the underlying error does not indicate success.
inline 
Trap::operator bool() const { return (bool)err_.code(); }

inline
Trap::operator Error() const { return err_; }

/// Returns the underlying error.
inline Error
Trap::error() const { return err_; }

/// Returns the error code.
inline Error::Code 
Trap::code() const { return err_.code(); }

/// Returns the error data.
inline Error::Data
Trap::data() const { return err_.data(); }


} // namespace freeflow
