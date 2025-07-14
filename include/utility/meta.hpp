//
// Created by sayan on 7/10/25.
//

#ifndef META_HPP
#define META_HPP
#pragma once

template <typename...> struct TD;

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
#endif // META_HPP
