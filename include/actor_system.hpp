//
// Created by sayan on 7/13/25.
//

#ifndef ACTOR_SYSTEM_HPP
#define ACTOR_SYSTEM_HPP

#include "actor.hpp"
#include "scheduler.hpp"

class ActorSystem {
private:
  Scheduler scheduler;
public:
  ActorSystem() : scheduler{std::make_unique<ThreadPool<>>()} {}
  template <typename TActor> constexpr void subscribe(TActor &actor) {
    scheduler.subscribe(actor);
  }
  template <typename ActorType> constexpr void unsubscribe(ActorType &actor) {
    scheduler.unsubscribe(actor);
  }
};
#endif //ACTOR_SYSTEM_HPP
