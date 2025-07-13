//
// Created by sayan on 7/10/25.
//

#ifndef MAILBOX_HPP
#define MAILBOX_HPP
#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <functional>
#include <stack>

#include "utility/callable.hpp"

using mail_id_t = boost::uuids::uuid;
class Actor;

class Mail {
  Actor *actor;
  mail_id_t id;
  std::stack<std::function<void()>> stages;
  std::tuple<std::string, double> message;
};

class Message {
  Callable f;
public:
  template<typename... TArgs>
  constexpr auto operator()(TArgs&&... args) const {
    return f(std::forward<TArgs>(args)...);
  }
};

template <typename TActor> class DefaultMessage : public Message {};

template <typename TActor> class DownMessage : public Message {};

template <typename TActor> class ExitMessage : public Message {};

template <typename TMessage>
concept CMessage =
    std::invocable<TMessage> && std::is_default_constructible_v<TMessage> &&
    std::copy_constructible<TMessage> && std::move_constructible<TMessage>;

#endif // MAILBOX_HPP
