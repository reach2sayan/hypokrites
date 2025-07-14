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

template <typename Ret, typename... Args> class Message {
  std::function<Ret(Args...)> f;
  using callable_t = std::function<Ret(Args...)>;

public:
  Message() = default;
  constexpr Message(std::invocable<Ret, Args...> auto f_) : f(std::move(f_)) {}
  constexpr auto operator()(auto &&...args) const
    requires(std::invocable<decltype(f), decltype(args)...>)
  {
    return f(std::forward<decltype(args)>(args)...);
  }
};

template <CBaseActor TActor> class DefaultMessage : public Message<Callable> {};
template <CBaseActor TActor> class DownMessage : public Message<Callable> {};
template <CBaseActor TActor> class ExitMessage : public Message<Callable> {};

template <typename TMessage>
concept CMessage =
    std::invocable<TMessage> && std::is_default_constructible_v<TMessage> &&
    std::copy_constructible<TMessage>;

static_assert(CMessage<void (*)()>);
static_assert(CMessage<Message<void (*)()>>);
static_assert(CMessage<Message<std::function<void()>>>);
static_assert(CMessage<std::function<void()>>);
static_assert(CMessage<Message<std::function<void()>>>);
static_assert(CMessage<std::function<void()>>);
static_assert(CMessage<Message<std::function<void(int)>>>);
static_assert(CMessage<Message<std::function<void(int)>>>);
#endif // MAILBOX_HPP
