#include "EventDispatcher.hpp"

void EventDispatcher::register_observer(EventType type, EventObserver* observer)
{
  for (auto i = observer_list_.begin(); i != observer_list_.end(); i++) {
    i->second->notify_registration(observer);
  }

  observer_list_.push_back(std::make_pair(type, observer));
}

void EventDispatcher::dispatch_event(const Event& event, int timeout_seconds)
{
  for (auto i = observer_list_.begin(); i != observer_list_.end(); i++) {
    if (i->first == event.type) {
      i->second->notify(event, timeout_seconds);
    }
  }
}
