//
// Created by sayan on 7/13/25.
//

#ifndef ACTOR_BASE_HPP
#define ACTOR_BASE_HPP

#pragma once
#include "utility/meta.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <list>
#include <memory>

using actor_address_t = boost::uuids::uuid;
class ActorSystem;

class ActorBase {
private:
  const actor_address_t addr;
  ActorSystem &sys;

public:
  constexpr ActorBase(ActorSystem &sys_)
      : addr(boost::uuids::random_generator()()), sys(sys_) {}
  actor_address_t address() const { return addr; }
  ActorSystem &system() { return sys; }
  ~ActorBase() = default;
};

template <typename TActor>
concept CBaseActor = std::derived_from<TActor, ActorBase>;

template <CBaseActor TActor, typename> class MonitoredActor : public TActor {
  std::list<TActor *> monitors;

public:
  constexpr MonitoredActor(ActorSystem &sys_) : TActor{sys_} {}
  constexpr void add_monitor(TActor *observer) { monitors.push_back(observer); }
  constexpr void remove_monitor(TActor *observer) { monitors.remove(observer); }
  constexpr void link(TActor *other) {
    monitors.push_back(&other);
    other->add_monitor(this);
  }
  constexpr void unlink(TActor *other) {
    monitors.remove(&other);
    other->remove_monitor(this);
  }
  constexpr void notify() {
    // TODO To be implemented
  }
};

template <typename T>
concept SupportsMonitor = requires(T &&t) {
  { t.add_monitor(std::declval<T *>()) } -> std::same_as<void>;
  { t.remove_monitor(std::declval<T *>()) } -> std::same_as<void>;
  { t.link(std::declval<T *>()) } -> std::same_as<void>;
  { t.unlink(std::declval<T *>()) } -> std::same_as<void>;
  { t.notify() } -> std::same_as<void>;
};

template <CBaseActor TActor> class BaseMessageHandler;
template <CBaseActor TActor, typename> class ScheduledActor : public TActor {
private:
  BaseMessageHandler<TActor> base_handler;
  using exit_handler_t = BaseMessageHandler<TActor>::exit_handler_t;
  using down_handler_t = BaseMessageHandler<TActor>::down_handler_t;
  using default_handler_t = BaseMessageHandler<TActor>::default_handler_t;

public:
  ScheduledActor(ActorSystem &sys_) : TActor{sys_} {}
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

template <typename T>
concept SupportsBaseHandlers = requires(T &&t) {
  {
    t.set_exit_handler(std::declval<typename T::exit_handler_t>())
  } -> std::same_as<void>;
  {
    t.set_default_handler(std::declval<typename T::default_handler_t>())
  } -> std::same_as<void>;
  {
    t.set_down_handler(std::declval<typename T::down_handler_t>())
  } -> std::same_as<void>;
  { t.quit() } -> std::same_as<void>;
};

template <typename T>
concept CActor = requires(T &&t) {
  requires CBaseActor<T> && SupportsMonitor<T> && SupportsBaseHandlers<T>;
};
class Actor;
#endif // ACTOR_BASE_HPP
