//
// Created by sayan on 7/13/25.
//

#ifndef ACTOR_SYSTEM_HPP
#define ACTOR_SYSTEM_HPP

#include "actor.hpp"
class ActorSystem {
  std::vector<std::unique_ptr<Actor>> actors;
};

using cell = TypedActor<replies_to<int>::with<void>, replies_to<int>::with<double>>;
ActorSystem sys;
constexpr cell cell_t{sys};
#endif //ACTOR_SYSTEM_HPP
