#include "EventDispatcher.hpp"

#include "mu/tiny/mock/NamedValueComparator.hpp"

#include "mu/tiny/mock.hpp"
#include "mu/tiny/test.hpp"

using mu::tiny::mock::mock;

class ObserverMock : public EventObserver
{
public:
  void notify(const Event& event, int time_out_in_seconds) override
  {
    mock()
        .actual_call("notify")
        .on_object(this)
        .with_parameter_of_type(
            "Event", "event", static_cast<const void*>(&event)
        )
        .with_parameter("timeOutInSeconds", time_out_in_seconds);
  }
  void notify_registration(EventObserver* new_observer) override
  {
    mock()
        .actual_call("notifyRegistration")
        .on_object(this)
        .with_parameter("newObserver", new_observer);
  }
};

class EventComparator : public mu::tiny::mock::NamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    return static_cast<const Event*>(object1)->type ==
           static_cast<const Event*>(object2)->type;
  }
  mu::tiny::String value_to_string(const void* object) override
  {
    return mu::tiny::string_from(static_cast<const Event*>(object)->type);
  }
};

TEST_GROUP(EventDispatcher)
{
  Event event;
  EventDispatcher* dispatcher;
  ObserverMock observer;
  ObserverMock observer2;
  EventComparator event_comparator;

  void setup() override
  {
    dispatcher = new EventDispatcher;
    mock().install_comparator("Event", event_comparator);
  }
  void teardown() override
  {
    delete dispatcher;
    mock().remove_all_comparators_and_copiers();
  }
};

TEST(EventDispatcher, EventWithoutRegistrationsResultsIntoNoCalls)
{
  dispatcher->dispatch_event(event, 10);
}

TEST(EventDispatcher, EventWithRegistrationForEventResultsIntoCallback)
{
  mock()
      .expect_one_call("notify")
      .on_object(&observer)
      .with_parameter_of_type("Event", "event", &event)
      .with_parameter("timeOutInSeconds", 10);
  event.type = important_event;

  dispatcher->register_observer(important_event, &observer);
  dispatcher->dispatch_event(event, 10);
}

TEST(EventDispatcher, DifferentEventWithRegistrationDoesNotResultIntoCallback)
{
  event.type = less_important_event;
  dispatcher->register_observer(important_event, &observer);
  dispatcher->dispatch_event(event, 10);
}

TEST(EventDispatcher, RegisterTwoObserversResultIntoTwoCallsAndARegistrationNotification)
{
  mock()
      .expect_one_call("notify")
      .on_object(&observer)
      .with_parameter_of_type("Event", "event", &event)
      .with_parameter("timeOutInSeconds", 10);
  mock()
      .expect_one_call("notify")
      .on_object(&observer2)
      .with_parameter_of_type("Event", "event", &event)
      .with_parameter("timeOutInSeconds", 10);
  mock()
      .expect_one_call("notifyRegistration")
      .on_object(&observer)
      .with_parameter("newObserver", &observer2);

  event.type = important_event;
  dispatcher->register_observer(important_event, &observer);
  dispatcher->register_observer(important_event, &observer2);
  dispatcher->dispatch_event(event, 10);
}
