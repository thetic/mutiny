#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <list>
#include <utility>

enum EventType
{
  important_event,
  less_important_event
};

class Event
{
public:
  EventType type;
};

class EventObserver
{
public:
  virtual void notify(const Event& event, int time_out_in_seconds) = 0;
  virtual void notify_registration(EventObserver* new_observer) = 0;
  virtual ~EventObserver() {}
};

class EventDispatcher
{
  std::list<std::pair<EventType, EventObserver*>> observer_list_;

public:
  EventDispatcher();

  void register_observer(EventType type, EventObserver* observer);
  void dispatch_event(const Event& event, int timeout_seconds);
};

#endif
