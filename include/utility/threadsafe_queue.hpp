#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>

template <typename T> class Queue {
  struct Node {
    std::shared_ptr<T> data;
    std::unique_ptr<Node> next;
  };

  std::mutex head_mutex_;
  std::unique_ptr<Node> head_;

  std::mutex tail_mutex_;
  Node *tail_;

  std::condition_variable data_cond;
  uint32_t max_size_;
  std::atomic<uint32_t> size_ = 0;

  constexpr Node *get_tail() {
    std::lock_guard tail_lock{tail_mutex_};
    return tail_;
  }

  constexpr std::unique_ptr<Node> pop_head() {
    auto old_head = std::move(head_);
    head_ = std::move(old_head->next);
    return old_head;
  }

  constexpr std::unique_ptr<Node> try_pop_head() {
    std::lock_guard head_lock(head_mutex_);
    if (head_.get() == get_tail()) {
      return {};
    }
    size_.fetch_sub(1);
    return pop_head();
  }

  constexpr std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock head_lock(head_mutex_);
    data_cond.wait(head_lock, [&] { return head_.get() != get_tail(); });
    return std::move(head_lock);
  }

  constexpr std::unique_ptr<Node> wait_pop_head() {
    std::unique_lock head_lock(wait_for_data());
    return pop_head();
  }

public:
  explicit constexpr Queue(uint32_t max_size = static_cast<uint32_t>(-1))
      : head_{std::make_unique<Node>()}, tail_{head_.get()},
        max_size_{max_size} {}

  Queue(const Queue &other) = delete;
  Queue &operator=(const Queue &other) = delete;

  constexpr std::shared_ptr<T> try_pop() {
    std::unique_ptr<Node> old_head = try_pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
  }

  constexpr bool try_pop(T &value) {
    std::unique_ptr<Node> old_head = try_pop_head();
    return old_head ? value = std::move(*old_head->data), void(), true : false;
  }

  constexpr std::shared_ptr<T> wait_and_pop() {
    const std::unique_ptr<Node> old_head = wait_pop_head();
    return old_head->data;
  }
  constexpr void wait_and_pop(T &value) { value = *wait_and_pop(); }
  constexpr void push(T new_value);
  constexpr bool empty() {
    std::lock_guard lk{head_mutex_};
    return head_.get() == get_tail();
  }
  [[nodiscard]] constexpr auto get_size() const {
    return size_.load(std::memory_order_relaxed);
  }
};

template <typename T> constexpr void Queue<T>::push(T new_value) {
  if (size_.load(std::memory_order_relaxed) == max_size_) {
    throw std::length_error("queue size exceeded limit!");
  }
  auto new_data = std::make_shared<T>(std::move(new_value));
  auto p = std::make_unique<Node>();
  Node *const new_tail = p.get();
  std::lock_guard tail_lock(tail_mutex_);
  tail_->data = std::move(new_data);
  tail_->next = std::move(p);
  tail_ = new_tail;
  size_.fetch_add(1);
}