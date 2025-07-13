//
// Created by sayan on 7/10/25.
//

#ifndef META_HPP
#define META_HPP
#pragma once

template <class D, class B, template <class, class> class... Ms>
struct extend_helper;

template <class D, class B> struct extend_helper<D, B> {
  using type = B;
};

template <class D, class B, template <class, class> class M,
          template <class, class> class... Ms>
struct extend_helper<D, B, M, Ms...> : extend_helper<D, M<B, D>, Ms...> {};

// extend<ar, T>::with<ob, fo> == fo<ob<ar, T>, T>
template <class Base, class Derived = Base> struct extend {
  template <template <class, class> class... Mixins>
  using with = typename extend_helper<Derived, Base, Mixins...>::type;
};

template <typename ReturnType, typename... Args> struct make_message_handler {
  using ret_t = ReturnType;
  using args_t = std::tuple<Args...>;
  using behaviour_t = std::function<ret_t(Args...)>;
  using message_t = MessageHandler<behaviour_t>;
};

template <typename ReturnType, typename... Args>
using make_message_handler_t =
    typename make_message_handler<ReturnType, Args...>::message_t;

template <typename... Args> struct replies_t {
  using type = std::tuple<Args...>;
  template <typename ReturnType>
  using with = make_message_handler_t<ReturnType, Args...>;
};

#endif // META_HPP
