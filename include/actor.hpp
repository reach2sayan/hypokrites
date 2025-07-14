//
// Created by sayan on 7/10/25.
//

#ifndef ABSTRACT_ACTOR_HPP
#define ABSTRACT_ACTOR_HPP
#pragma once

#include "actor_base.hpp"
#include "message_handler.hpp"

// extend<ar, T>::with<ob, fo> == fo<ob<ar, T>, T>
// IMonitor<ActorBase>
class Actor : public extend<ActorBase>::with<MonitoredActor, ScheduledActor> {
private:
  const char *name;
  struct ActorStateBase {
    virtual constexpr ActorStateBase &get_state() = 0;
  };
  template <typename TState> struct ActorState;
  std::unique_ptr<ActorStateBase> state;

public:
  constexpr ActorStateBase &get_state() { return state->get_state(); }
  static constexpr ActorStateBase &get_state(Actor &actor) {
    return actor.get_state();
  }
  template <typename TState>
  using state_reference =
      decltype(std::declval<ActorState<TState>>().get_state());
};

template <typename TState> struct Actor::ActorState : ActorStateBase {
  TState state;

public:
  constexpr ActorState(auto &&state_)
      : state(std::forward<decltype(state_)>(state_)) {}
  virtual constexpr ActorStateBase &get_state() override final { return *this; }
};

template <CMessage... TMessages> class TypedActor : public Actor {
  Behaviours<Actor, TMessages...> behaviours;

public:
  using base = Actor;
  using behaviours_t = Behaviours<Actor, TMessages...>;
  constexpr auto become(CMessage auto &&...messages)
    requires(std::same_as<TMessages, std::remove_cv_t<decltype(messages)>> &&
             ...)
  {
    std::exchange(behaviours, Behaviours<Actor, TMessages...>(
                                  std::forward<messages>(messages)...));
  }
};

using cell = TypedActor<replies_to<int>::with<void>, replies_to<int>::with<double>>;
constexpr cell cell_t;
#endif // ABSTRACT_ACTOR_HPP
