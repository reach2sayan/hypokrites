//
// Created by sayan on 7/10/25.
//

#ifndef ABSTRACT_ACTOR_HPP
#define ABSTRACT_ACTOR_HPP
#pragma once
#include "actor_base.hpp"

// extend<ar, T>::with<ob, fo> == fo<ob<ar, T>, T>
// IMonitor<ActorBase>
class Actor : public extend<ActorBase>::with<MonitoredActor, ScheduledActor> {
  const char *name;
};

template <CMessage... TMessages>
constexpr auto become(TMessages &&...messages) {
  return Behaviours<Actor, TMessages...>(std::forward<TMessages>(messages));
}

template <CMessage... TMessages> class TypedActor : public Actor {
  Behaviours<Actor, TMessages...> behaviours;

public:
};

#endif // ABSTRACT_ACTOR_HPP
