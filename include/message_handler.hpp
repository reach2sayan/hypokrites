//
// Created by sayan on 7/10/25.
//

#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP
#pragma once
#include <chrono>
#include <tuple>
#include <utility>

template <typename... TCallables> class MessageHandler {
  std::tuple<TCallables...> handlers;
  MessageHandler or_else(MessageHandler other);

public:
  MessageHandler operator|(MessageHandler other) {
    return or_else(std::move(other));
  }
};

template <typename FinalCallable, typename... TCallables>
class Behaviours : public MessageHandler<TCallables...> {
  FinalCallable final_handler;

public:
  Behaviours(TCallables... handlers, FinalCallable final_handler)
      : MessageHandler<TCallables...>(std::move(handlers)...),
        final_handler(std::move(final_handler)) {}
};

#endif // MESSAGE_HANDLER_HPP
