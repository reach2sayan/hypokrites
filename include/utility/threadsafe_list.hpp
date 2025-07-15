#pragma once
#include <memory>
#include <mutex>

template <typename T> class List {
  struct Node {
    std::mutex mtx;
    std::shared_ptr<T> data;
    std::unique_ptr<Node> next;
    Node() = default;
    Node(const T &value) : data(std::make_shared<T>(value)) {}
  };
  Node head;
public:
  constexpr List() = default;
  constexpr ~List() {
    remove_if([](const Node &) { return true; }); // remove node
  }

  List(const List &other) = delete;
  List &operator=(const List &other) = delete;

  constexpr void push_front(const T &value) {
    auto new_node = std::make_unique<Node>(value);
    std::lock_guard<std::mutex> lk(head.mtx);
    new_node->next = std::move(head.next);
    head.next = std::move(new_node);
  }

  template <typename Function> constexpr void for_each(Function f) {
    Node *current = &head;
    std::unique_lock lk(head.mtx);
    while (Node *const next = current->next.get()) {
      std::unique_lock next_lk(next->mtx);
      lk.unlock();
      std::invoke(f,*next->data);
      current = next;
      lk = std::move(next_lk);
    }
  }

  template <typename Predicate> constexpr std::shared_ptr<T> find_first_if(Predicate p) {
    Node *current = &head;
    std::unique_lock lk(head.mtx);
    while (Node *const next = current->next.get()) {
      std::unique_lock next_lk(next->mtx);
      lk.unlock();
      if (std::invoke(p,*next->data)) {
        return next->data;
      }
      current = next;
      lk = std::move(next_lk);
    }
    return std::shared_ptr<T>{};
  }

  template <typename Predicate> constexpr void remove_if(Predicate p) {
    Node *current = &head;
    std::unique_lock lk(head.mtx);
    while (Node *const next = current->next.get()) {
      std::unique_lock next_lk(next->mtx);
      if (std::invoke(p,*next->data)) {
        auto _ = std::move(current->next);
        current->next = std::move(next->next);
        next_lk.unlock();
      } else {
        lk.unlock();
        current = next;
        lk = std::move(next_lk);
      }
    }
  }
};