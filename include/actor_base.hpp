//
// Created by sayan on 7/13/25.
//

#ifndef ACTOR_BASE_HPP
#define ACTOR_BASE_HPP

#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <list>
#include <memory>

#include "monitors.hpp"
#include "utility/meta.hpp"

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
  virtual ActorSystem &system() final { return sys; }
  virtual ~ActorBase() = default;
};

template<typename TActor>
concept CBaseActor = std::derived_from<TActor,ActorBase>;

template <CBaseActor TActor, typename>
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

template <CBaseActor TActor> class BaseMessageHandler;
template <CBaseActor TActor, typename> class ScheduledActor {
private:
  BaseMessageHandler<TActor> base_handler;
  using exit_handler_t = BaseMessageHandler<TActor>::exit_handler_t;
  using down_handler_t = BaseMessageHandler<TActor>::down_handler_t;
  using default_handler_t = BaseMessageHandler<TActor>::default_handler_t;

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

template<typename T>
concept CActor = requires(T&& t) {
  { t.address() } -> std::same_as<actor_address_t>;
  { t.system() } -> std::same_as<ActorSystem &>;
  { t.quit() } -> std::same_as<void>;
  { t.set_exit_handler(std::declval<typename T::exit_handler_t>()) } -> std::same_as<void>;
  { t.set_default_handler(std::declval<typename T::default_handler_t>()) } -> std::same_as<void>;
  { t.set_down_handler(std::declval<typename T::down_handler_t>()) } -> std::same_as<void>;
} && SupportsMonitor<T>;

class Actor;

template<typename TState>
class ActorState;

#endif // ACTOR_BASE_HPP
