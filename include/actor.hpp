//
// Created by sayan on 7/10/25.
//

#ifndef ABSTRACT_ACTOR_HPP
#define ABSTRACT_ACTOR_HPP
#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <memory>
#include <list>

#include "utility/meta.hpp"
#include "monitors.hpp"

using actor_address_t = boost::uuids::uuid;
class ActorSystem;

class ActorBase {
private:
  const actor_address_t addr;
  ActorSystem &sys;

public:
  ActorBase(ActorSystem &sys_)
      : addr(boost::uuids::random_generator()()), sys(sys_) {}
  virtual auto address() -> actor_address_t const final { return addr; }
  virtual auto system() -> ActorSystem & final { return sys; }
  ~ActorBase() = default;
};

template<typename TActor, typename>
class MonitoredActor: public IMonitor<TActor> {
  std::list<TActor*> monitors;
public:
  virtual void add_monitor(TActor* observer) override {
    monitors.push_back(std::move(observer));
  }
  virtual void remove_monitor(TActor* observer) override {
    monitors.remove_if([observer](auto&& ptr) { return ptr.get() == observer;});
  }
  virtual void notify() override {
//TODO To be implemented
  }
};

// extend<ar, T>::with<ob, fo> == fo<ob<ar, T>, T>
// IMonitor<ActorBase>
class Actor : public extend<ActorBase>::with<MonitoredActor> {
  const char* name;
};

#endif // ABSTRACT_ACTOR_HPP
