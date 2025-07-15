#pragma once
#include "utility/thread_pool.hpp"
#include "utility/threadsafe_list.hpp"
#include <threadsafe_queue.hpp>

class Scheduler {
public:
  constexpr Scheduler(std::unique_ptr<ThreadPool<>> pool_)
      : pool{std::move(pool_)}, work_queues{}, done{false} {
    auto func = [this]() { return schedule(); };
    pool->submit(Callable(std::move(func)));
  }

  template <typename ActorType> constexpr void subscribe(ActorType &actor) {
    if (!done) {
      work_queues.push_front(actor.GetQueueRef());
    }
  }

  template <typename ActorType> constexpr void unsubscribe(ActorType &actor) {
    work_queues.remove_if([&actor](Queue<std::unique_ptr<Callable>> *qPtr) {
      return qPtr == actor.GetQueueRef();
    });
  }

  constexpr void quit() { done = true; }

private:
  constexpr void schedule() {
    while (!done) {
      work_queues.for_each([this](Queue<std::unique_ptr<Callable>> *qPtr) {
        while (!qPtr->empty()) {
          auto workPtr = qPtr->wait_and_pop();
          pool->submit(std::move(*(*workPtr)));
          workPtr = nullptr;
        }
      });
    }
  }
  List<Queue<std::unique_ptr<Callable>> *> work_queues;
  std::unique_ptr<ThreadPool<>> pool;
  std::atomic<bool> done;
};