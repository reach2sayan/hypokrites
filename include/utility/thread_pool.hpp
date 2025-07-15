#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "callable.hpp"
#include "utility/threadsafe_queue.hpp"
#include <future>
#include <iostream>
#include <ranges>
#include <thread>
#include <type_traits>
#include <vector>

template <typename Iterable = std::vector<std::jthread>> class ThreadPool {
private:
  std::atomic<bool> done = false;
  Queue<Callable> work_queue;
  Iterable threads;

public:
  ThreadPool();
  constexpr ~ThreadPool() { done = true; }
  constexpr void submit(Callable c) { work_queue.push(std::move(c)); }

private:
  void worker_thread();
};

template <typename Iterable> ThreadPool<Iterable>::ThreadPool() {
  unsigned const thread_count = std::jthread::hardware_concurrency();
  threads.reserve(thread_count);
  try {
    for (auto i = 0; i < thread_count; ++i) {
      threads.emplace_back(std::jthread(&ThreadPool::worker_thread, this));
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

template <typename Iterable> void ThreadPool<Iterable>::worker_thread() {
  while (!done) {
    Callable task;
    if (work_queue.try_pop(task)) {
      task();
    } else {
      std::this_thread::yield();
    }
  }
}

#endif // THREAD_POOL_H_