//
// Created by sayan on 7/10/25.
//

#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP
#pragma once

#include <concepts>
#include <tuple>
#include <utility>

#include "messages.hpp"

template <CBaseActor TActor, typename DefaultRet, typename... Args>
class BaseMessageHandler {
public:
  using exit_handler_t = std::function<void(ExitMessage<TActor> &)>;
  using down_handler_t = std::function<void(DownMessage<TActor> &)>;
  using default_handler_t =
      std::function<DefaultRet(DefaultMessage<TActor, DefaultRet, Args...> &)>;

private:
  exit_handler_t exit_handler;
  down_handler_t down_handler;
  default_handler_t default_handler;

public:
  constexpr BaseMessageHandler() = default;
  constexpr void set_exit_handler(std::invocable auto handler) {
    exit_handler = std::move(handler);
  }
  constexpr void set_default_handler(std::invocable auto handler) {
    default_handler = std::move(handler);
  }
  constexpr void set_down_handler(std::invocable auto handler) {
    down_handler = std::move(handler);
  }
};

template <typename... TCallables> class MessageHandler {
private:
  std::tuple<TCallables...> handlers;
  constexpr MessageHandler or_else(MessageHandler other);

public:
  constexpr MessageHandler operator|(MessageHandler other) {
    return or_else(std::move(other));
  }
};

template <CMessage FinalCallable, CMessage... TCallables>
class Behaviours : public MessageHandler<TCallables...> {
  FinalCallable final_handler;

public:
  constexpr Behaviours() = default;
  constexpr Behaviours(TCallables... handlers, FinalCallable final_handler)
      : MessageHandler<TCallables...>(std::move(handlers)...),
        final_handler(std::move(final_handler)) {}
};

template <typename ReturnType, typename... Args> struct make_message_handler {
  using ret_t = ReturnType;
  using args_t = std::tuple<Args...>;
  using behaviour_t = Message<std::function<ret_t(Args...)>>;
  using message_t = MessageHandler<behaviour_t>;
};

template <typename... Args> struct replies_to {
  using type = std::tuple<Args...>;
  template <typename ReturnType>
  using with = make_message_handler<ReturnType, Args...>::behaviour_t;
};

#endif // MESSAGE_HANDLER_HPP
