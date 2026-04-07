#ifndef INCLUDED_MUTINY_MOCK_CHECKEDEXPECTEDCALL_HPP
#define INCLUDED_MUTINY_MOCK_CHECKEDEXPECTEDCALL_HPP

#include "mutiny/mock/ExpectedCall.hpp"
#include "mutiny/mock/NamedValue.hpp"
#include "mutiny/mock/NamedValueList.hpp"

namespace mu {
namespace tiny {
namespace mock {

class CheckedExpectedCall : public ExpectedCall
{

public:
  CheckedExpectedCall();
  CheckedExpectedCall(unsigned int num_calls);
  ~CheckedExpectedCall() override;

  ExpectedCall& with_name(const String& name) override;
  ExpectedCall& with_call_order(unsigned int call_order) override
  {
    return with_call_order(call_order, call_order);
  }
  ExpectedCall& with_call_order(
      unsigned int initial_call_order,
      unsigned int final_call_order
  ) override;
  ExpectedCall& with_parameter_of_type(
      const String& type_name,
      const String& name,
      const void* value
  ) override;
  ExpectedCall& with_output_parameter_returning(
      const String& name,
      const void* value,
      size_t size
  ) override;
  ExpectedCall& with_output_parameter_of_type_returning(
      const String& type_name,
      const String& name,
      const void* value
  ) override;
  ExpectedCall& with_unmodified_output_parameter(const String& name) override;
  ExpectedCall& ignore_other_parameters() override;

  ExpectedCall& and_return_value(bool value) override;
  ExpectedCall& and_return_value(long long value) override;
  ExpectedCall& and_return_value(unsigned long long value) override;
  ExpectedCall& and_return_value(double value) override;
  ExpectedCall& and_return_value(const char* value) override;
  ExpectedCall& and_return_value(void* value) override;
  ExpectedCall& and_return_value(const void* value) override;
  ExpectedCall& and_return_value(void (*value)()) override;

  virtual NamedValue return_value();

  ExpectedCall& on_object(void* object_ptr) override;

  virtual NamedValue get_input_parameter(const String& name);
  virtual NamedValue get_output_parameter(const String& name);
  virtual String get_input_parameter_type(const String& name);
  virtual String get_input_parameter_value_string(const String& name);

  virtual bool has_input_parameter_with_name(const String& name);
  virtual bool has_input_parameter(const NamedValue& parameter);
  virtual bool has_output_parameter_with_name(const String& name);
  virtual bool has_output_parameter(const NamedValue& parameter);
  virtual bool relates_to(const String& function_name);
  virtual bool relates_to_object(const void* object_ptr) const;

  virtual bool is_fulfilled();
  virtual bool can_match_actual_calls();
  virtual bool is_matching_actual_call_and_finalized();
  virtual bool is_matching_actual_call();
  virtual bool are_parameters_matching_actual_call();
  virtual bool is_out_of_order() const;

  virtual void call_was_made(unsigned int call_order);
  virtual void input_parameter_was_passed(const String& name);
  virtual void output_parameter_was_passed(const String& name);
  virtual void finalize_actual_call_match();
  virtual void was_passed_to_object();
  virtual void reset_actual_call_matching_state();

  virtual String call_to_string();
  virtual String missing_parameters_to_string();

  static constexpr unsigned int no_expected_call_order = 0;

  virtual unsigned int get_actual_calls_fulfilled() const;

protected:
  void set_name(const String& name);
  String get_name() const;

private:
  String function_name_;

  class MockExpectedFunctionParameter : public NamedValue
  {
  public:
    MockExpectedFunctionParameter(const String& name);
    void set_matches_actual_call(bool b);
    bool is_matching_actual_call() const;

  private:
    bool matches_actual_call_{ false };
  };

  MockExpectedFunctionParameter* item(NamedValueListNode* node);

  bool ignore_other_parameters_;
  bool is_actual_call_match_finalized_;
  unsigned int initial_expected_call_order_;
  unsigned int final_expected_call_order_;
  bool out_of_order_;
  NamedValueList* input_parameters_;
  NamedValueList* output_parameters_;
  NamedValue return_value_;
  void* object_ptr_;
  bool is_specific_object_expected_;
  bool was_passed_to_object_;
  unsigned int actual_calls_;
  unsigned int expected_calls_;

  // NVI back-end: private overrides of ExpectedCall's typed pure virtuals
  ExpectedCall& with_bool_parameter(const String& name, bool value) override;
  ExpectedCall& with_int_parameter(const String& name, int value) override;
  ExpectedCall& with_unsigned_int_parameter(
      const String& name,
      unsigned int value
  ) override;
  ExpectedCall& with_long_int_parameter(
      const String& name,
      long int value
  ) override;
  ExpectedCall& with_unsigned_long_int_parameter(
      const String& name,
      unsigned long int value
  ) override;
  ExpectedCall& with_long_long_int_parameter(
      const String& name,
      long long value
  ) override;
  ExpectedCall& with_unsigned_long_long_int_parameter(
      const String& name,
      unsigned long long value
  ) override;
  ExpectedCall& with_double_parameter(
      const String& name,
      double value
  ) override;
  ExpectedCall& with_double_parameter(
      const String& name,
      double value,
      double tolerance
  ) override;
  ExpectedCall& with_string_parameter(
      const String& name,
      const char* value
  ) override;
  ExpectedCall& with_pointer_parameter(
      const String& name,
      void* value
  ) override;
  ExpectedCall& with_const_pointer_parameter(
      const String& name,
      const void* value
  ) override;
  ExpectedCall& with_function_pointer_parameter(
      const String& name,
      void (*value)()
  ) override;
  ExpectedCall& with_memory_buffer_parameter(
      const String& name,
      const unsigned char* value,
      size_t size
  ) override;
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
