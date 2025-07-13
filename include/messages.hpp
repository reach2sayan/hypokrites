//
// Created by sayan on 7/10/25.
//

#ifndef MAILBOX_HPP
#define MAILBOX_HPP
#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <stack>

#include "actor_base.hpp"
#include "utility/callable.hpp"

using mail_id_t = boost::uuids::uuid;

class MailBox {
  Actor *actor;
  mail_id_t id;
  std::stack<std::function<void()>> stages;
  std::tuple<std::string, double> message;
};

template<typename TCallable>
class Message {
  TCallable f;
public:
  template <typename... TArgs>
    requires(std::invocable<TCallable, TArgs...>)
  constexpr auto operator()(TArgs &&...args) const {
    return f(std::forward<TArgs>(args)...);
  }
};

template <CActor TActor> class DefaultMessage : public Message<Callable> {};
template <CActor TActor> class DownMessage : public Message<Callable> {};
template <CActor TActor> class ExitMessage : public Message<Callable> {};

template <typename TMessage>
concept CMessage =
    std::invocable<TMessage> && std::is_default_constructible_v<TMessage> &&
    std::copy_constructible<TMessage> && std::move_constructible<TMessage>;

static_assert(CMessage<Message<void(*)()>>);
static_assert(CMessage<Message<std::function<void()>>>);
static_assert(CMessage<Message<std::function<void()>>>);
#endif // MAILBOX_HPP
