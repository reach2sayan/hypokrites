//
// Created by sayan on 7/10/25.
//

#ifndef ABSTRACT_ACTOR_HPP
#define ABSTRACT_ACTOR_HPP
#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <list>
#include <memory>

#include "monitors.hpp"
#include "utility/meta.hpp"
#include "message_handler.hpp"

using actor_address_t = boost::uuids::uuid;
class ActorSystem;

class ActorBase {
private:
  const actor_address_t addr;
  ActorSystem &sys;

public:
  ActorBase(ActorSystem &sys_)
      : addr(boost::uuids::random_generator()()), sys(sys_) {}
  virtual actor_address_t address() const final { return addr; }
  virtual ActorSystem & system() final { return sys; }
  virtual ~ActorBase() = default;
};

template <typename TActor, typename>
class MonitoredActor : public IMonitor<TActor> {
  std::list<TActor *> monitors;

public:
  virtual void add_monitor(TActor *observer) override {
    monitors.push_back(std::move(observer));
  }
  virtual void remove_monitor(TActor *observer) override {
    monitors.remove(observer);
  }
  virtual void notify() override {
    // TODO To be implemented
  }
};

template<typename TActor, typename>
class ScheduledActor {
  BaseMessageHandler<TActor> base_handler;
  using BaseMessageHandler<TActor>::exit_handler_t;
  using BaseMessageHandler<TActor>::down_handler_t;
  using BaseMessageHandler<TActor>::default_handler_t;
public:
  constexpr void quit();
  constexpr void set_exit_handler(exit_handler_t handler) {
    base_handler.set_exit_handler(std::move(handler));
  }
  constexpr void set_default_handler(default_handler_t handler) {
    base_handler.set_default_handler(std::move(handler));
  }
  constexpr void set_down_handler(default_handler_t handler) {
    base_handler.set_down_handler(std::move(handler));
  }
};

// extend<ar, T>::with<ob, fo> == fo<ob<ar, T>, T>
// IMonitor<ActorBase>
class Actor : public extend<ActorBase>::with<MonitoredActor, ScheduledActor> {
  const char *name;
};

#endif // ABSTRACT_ACTOR_HPP
