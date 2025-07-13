//
// Created by sayan on 7/10/25.
//

#ifndef ABSTRACT_ACTOR_HPP
#define ABSTRACT_ACTOR_HPP
#pragma once

#include "message_handler.hpp"
#include "actor_base.hpp"

// extend<ar, T>::with<ob, fo> == fo<ob<ar, T>, T>
// IMonitor<ActorBase>
class Actor : public extend<ActorBase>::with<MonitoredActor, ScheduledActor> {
  const char *name;
};

template <CMessage... TMessages> class TypedActor : public Actor {
  Behaviours<Actor, TMessages...> behaviours;

public:
  constexpr auto become(CMessage auto &&...messages) {
    std::exchange(behaviours, Behaviours<Actor, TMessages...>(
                                  std::forward<messages>(messages)...));
  }
};

#endif // ABSTRACT_ACTOR_HPP
