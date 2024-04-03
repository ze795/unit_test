/**
 *  Copyright (C) 2023 - Innovusion Inc.
 *
 *  All Rights Reserved.
 *
 *  $Id$
 */
#ifndef STUB_STUB_MOCK_H
#define STUB_STUB_MOCK_H

#include <gmock/gmock.h>
#include "stub.h"

class StubMock : public Stub {
  /**
   * @brief default implementation
  */
 private:
  StubMock() = default;
  ~StubMock() = default;

 public:
  StubMock(const StubMock &) = delete;
  StubMock operator=(const StubMock &) = delete;
  StubMock(StubMock &&) = delete;
  StubMock operator=(StubMock &&) = delete;

  static StubMock &get_instance() {
    static StubMock stub_mock;
    return stub_mock;
  }

  /**
   * @brief static stub function implementation
  */
 private:
  template <int N, typename R, typename... ARGS>
  class FnStatic {
   public:
    static testing::Action<R(ARGS...)> action;
  };

  template <int N, typename R, typename... ARGS>
  static R call_fn(ARGS... args) {
    return FnStatic<N, R, ARGS...>::action.Perform({std::forward<ARGS>(args)...});
  }

  /**
   * @brief set different stub function implementation
  */
 public:
  template <int N = 0, typename F, typename R, typename... ARGS>
  static void set_fn(R (*fn)(ARGS...), F fn_stub) {
    FnStatic<N, R, ARGS...>::action = fn_stub;
    get_instance().set(fn, &StubMock::call_fn<N, R, ARGS...>);
  }

#define GEN_SET_FN(SPEC)                                                     \
  template <int N = 0, typename F, typename R, typename C, typename... ARGS> \
  static void set_fn(R (C::*fn)(ARGS...) SPEC, F fn_stub) {                  \
    FnStatic<N, R, C *, ARGS...>::action = fn_stub;                          \
    get_instance().set(fn, &StubMock::call_fn<N, R, C *, ARGS...>);          \
  }

  GEN_SET_FN()
  GEN_SET_FN(const)
  GEN_SET_FN(volatile)
  GEN_SET_FN(const volatile)
  GEN_SET_FN(&)
  GEN_SET_FN(const &)
  GEN_SET_FN(volatile &)
  GEN_SET_FN(const volatile &)
  GEN_SET_FN(&&)
  GEN_SET_FN(const &&)
  GEN_SET_FN(volatile &&)
  GEN_SET_FN(const volatile &&)

#undef GEN_SET_FN

  /**
   * @brief auxiliary derivation function, implementation is empty
  */
public:
#define GEN_VFN_ADDR(SPEC)                                             \
 template <typename R, typename C, typename... ARGS>                   \
 static auto vfn_addr(R (C::*)(ARGS...) SPEC) -> R (*)(C *, ARGS...) { \
   return nullptr;                                                     \
 }

 GEN_VFN_ADDR()
 GEN_VFN_ADDR(const)
 GEN_VFN_ADDR(volatile)
 GEN_VFN_ADDR(const volatile)
 GEN_VFN_ADDR(&)
 GEN_VFN_ADDR(const &)
 GEN_VFN_ADDR(volatile &)
 GEN_VFN_ADDR(const volatile &)
 GEN_VFN_ADDR(&&)
 GEN_VFN_ADDR(const &&)
 GEN_VFN_ADDR(volatile &&)
 GEN_VFN_ADDR(const volatile &&)

#undef GEN_VFN_ADDR
};

template <int N, typename R, typename... ARGS>
testing::Action<R(ARGS...)> StubMock::FnStatic<N, R, ARGS...>::action;


/**
 * @brief extension gtest macro, check volatile spec
*/
#define GMOCK_INTERNAL_DETECT_VOLATILE_I_volatile ,
#define GMOCK_INTERNAL_DETECT_VOLATILE(_i, _, _elem) GMOCK_PP_CAT(GMOCK_INTERNAL_DETECT_VOLATILE_I_, _elem)
#define GMOCK_INTERNAL_HAS_VOLATILE(_Tuple) \
  GMOCK_PP_HAS_COMMA(GMOCK_PP_FOR_EACH(GMOCK_INTERNAL_DETECT_VOLATILE, ~, _Tuple))

/**
 * @brief help macro, extraction spec field (const, volatile, &, &&)
 */
#define CLASS_SPEC(SPEC)                                     \
  GMOCK_PP_IF(GMOCK_INTERNAL_HAS_CONST(SPEC), const, )       \
  GMOCK_PP_IF(GMOCK_INTERNAL_HAS_VOLATILE(SPEC), volatile, ) \
  GMOCK_INTERNAL_GET_REF_SPEC(SPEC)                          \
  GMOCK_INTERNAL_GET_NOEXCEPT_SPEC(SPEC)                     \
  GMOCK_PP_IF(GMOCK_INTERNAL_HAS_OVERRIDE(SPEC), override, ) \
  GMOCK_PP_IF(GMOCK_INTERNAL_HAS_FINAL(SPEC), final, )

/**
 * @brief helper macro, get function address
*/
#define V_ADDR(CLASS_NAME, MEMBER_NAME) decltype(StubMock::vfn_addr(&CLASS_NAME::MEMBER_NAME))(&CLASS_NAME::MEMBER_NAME)
#define O_ADDR(CLASS_NAME, MEMBER_NAME, RETURN, ARGS, SPEC) \
  ((RETURN(CLASS_NAME::*) ARGS CLASS_SPEC(SPEC))ADDR(CLASS_NAME, MEMBER_NAME))
#define OV_ADDR(CLASS_NAME, MEMBER_NAME, RETURN, ARGS, SPEC)                                                        \
  decltype(StubMock::vfn_addr(O_ADDR(CLASS_NAME, MEMBER_NAME, RETURN, ARGS, SPEC))) O_ADDR(CLASS_NAME, MEMBER_NAME, \
                                                                                           RETURN, ARGS, SPEC)

/**
 * @brief global variable, StubMock instance
*/
#define SMOCK (StubMock::get_instance())

/**
 * @brief clear all stub function
*/
#define SMOCK_CLEAR SMOCK.clear()

/**
 * @brief set stub function
 * 
 * @warning n: stub function number, same type stub function stub function number must be different
*/
#define NF_SMOCK(n, fn, fn_stub) SMOCK.set_fn<n>((fn), (fn_stub))

/**
 * @brief set stub function, stub function number auto increment
*/
#define F_SMOCK(fn, fn_stub) NF_SMOCK(__COUNTER__, fn, fn_stub)

#endif  // STUB_STUB_MOCK_H
