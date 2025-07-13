//
// Created by sayan on 7/13/25.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#pragma once
#include <functional>
#include <memory>
#include <utility>

class Callable {
private:
  struct ImplBase {
    virtual constexpr void call() = 0;
  };
  std::unique_ptr<ImplBase> impl;
  template <std::invocable F> struct ImplType : ImplBase {
    F f;
    ImplType(std::invocable auto &&f_) : f(std::forward<decltype(f_)>(f_)) {}
    constexpr void call() override { std::invoke(f); }
  };

public:
  template <std::invocable F>
  Callable(F &&f) : impl{std::make_unique<ImplType<F>>(std::forward<F>(f))} {}
  constexpr void operator()() const { std::invoke(&ImplBase::call, impl); }
  Callable() = default;
  Callable(Callable &&other) : impl(std::move(other.impl)) {}
  Callable &operator=(Callable &&other) {
    impl = std::move(other.impl);
    return *this;
  }
  Callable(const Callable &) = delete;
  Callable(Callable &) = delete;
  Callable &operator=(const Callable &) = delete;
};
#endif // MESSAGE_HPP