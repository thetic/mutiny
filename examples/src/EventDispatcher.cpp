#include "EventDispatcher.hpp"

EventDispatcher::EventDispatcher() {}

void
EventDispatcher::registerObserver(EventType type, EventObserver* observer)
{
  for (std::list<std::pair<EventType, EventObserver*>>::iterator i =
           observerList_.begin();
      i != observerList_.end();
      i++)
    i->second->notifyRegistration(observer);

  observerList_.push_back(std::make_pair(type, observer));
}

void
EventDispatcher::dispatchEvent(const Event& event, int timeoutSeconds)
{
  for (std::list<std::pair<EventType, EventObserver*>>::iterator i =
           observerList_.begin();
      i != observerList_.end();
      i++) {
    if (i->first == event.type)
      i->second->notify(event, timeoutSeconds);
  }
}
