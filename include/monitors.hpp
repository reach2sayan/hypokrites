//
// Created by sayan on 7/10/25.
//

#ifndef MONITORS_HPP
#define MONITORS_HPP

template<typename TObserver>
class IMonitor : public TObserver {
public:
  virtual void add_monitor(TObserver* observer) = 0;
  virtual void remove_monitor(TObserver* observer) = 0;
  virtual void notify() = 0;
  virtual ~IMonitor() = default;
};

#endif //MONITORS_HPP
