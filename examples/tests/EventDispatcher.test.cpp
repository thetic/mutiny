#include "EventDispatcher.h"

#include "CppUTest/Extensions/MockSupport.hpp"

#include "CppUTest/TestHarness.hpp"

using cpputest::extensions::mock;

class ObserverMock : public EventObserver
{
public:
  virtual void notify(const Event& event, int timeOutInSeconds) override
  {
    mock()
        .actualCall("notify")
        .onObject(this)
        .withParameterOfType("Event", "event", static_cast<const void*>(&event))
        .withParameter("timeOutInSeconds", timeOutInSeconds);
  }
  virtual void notifyRegistration(EventObserver* newObserver) override
  {
    mock()
        .actualCall("notifyRegistration")
        .onObject(this)
        .withParameter("newObserver", newObserver);
  }
};

class EventComparator : public cpputest::extensions::MockNamedValueComparator
{
public:
  virtual bool isEqual(const void* object1, const void* object2) override
  {
    return static_cast<const Event*>(object1)->type ==
           static_cast<const Event*>(object2)->type;
  }
  virtual cpputest::String valueToString(const void* object) override
  {
    return cpputest::StringFrom(static_cast<const Event*>(object)->type);
  }
};

TEST_GROUP(EventDispatcher)
{
  Event event;
  EventDispatcher* dispatcher;
  ObserverMock observer;
  ObserverMock observer2;
  EventComparator eventComparator;

  void setup() override
  {
    dispatcher = new EventDispatcher;
    mock().installComparator("Event", eventComparator);
  }
  void teardown() override
  {
    delete dispatcher;
    mock().removeAllComparatorsAndCopiers();
  }
};

TEST(EventDispatcher, EventWithoutRegistrationsResultsIntoNoCalls)
{
  dispatcher->dispatchEvent(event, 10);
}

TEST(EventDispatcher, EventWithRegistrationForEventResultsIntoCallback)
{
  mock()
      .expectOneCall("notify")
      .onObject(&observer)
      .withParameterOfType("Event", "event", &event)
      .withParameter("timeOutInSeconds", 10);
  event.type = IMPORTANT_EVENT;

  dispatcher->registerObserver(IMPORTANT_EVENT, &observer);
  dispatcher->dispatchEvent(event, 10);
}

TEST(EventDispatcher, DifferentEventWithRegistrationDoesNotResultIntoCallback)
{
  event.type = LESS_IMPORTANT_EVENT;
  dispatcher->registerObserver(IMPORTANT_EVENT, &observer);
  dispatcher->dispatchEvent(event, 10);
}

TEST(EventDispatcher,
    RegisterTwoObserversResultIntoTwoCallsAndARegistrationNotification)
{
  mock()
      .expectOneCall("notify")
      .onObject(&observer)
      .withParameterOfType("Event", "event", &event)
      .withParameter("timeOutInSeconds", 10);
  mock()
      .expectOneCall("notify")
      .onObject(&observer2)
      .withParameterOfType("Event", "event", &event)
      .withParameter("timeOutInSeconds", 10);
  mock()
      .expectOneCall("notifyRegistration")
      .onObject(&observer)
      .withParameter("newObserver", &observer2);

  event.type = IMPORTANT_EVENT;
  dispatcher->registerObserver(IMPORTANT_EVENT, &observer);
  dispatcher->registerObserver(IMPORTANT_EVENT, &observer2);
  dispatcher->dispatchEvent(event, 10);
}
