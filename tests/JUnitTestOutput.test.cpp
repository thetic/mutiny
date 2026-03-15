#include "CppMu/JUnitTestOutput.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/String.hpp"
#include "CppMu/StringCollection.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestResult.hpp"
#include "CppMu/time.hpp"

namespace {

class FileForJUnitTestOutputs
{
  cppmu::String name_;
  bool is_open_;
  cppmu::String buffer_;
  FileForJUnitTestOutputs* next_;

  cppmu::StringCollection lines_of_file_;

public:
  FileForJUnitTestOutputs(const cppmu::String& filename,
      FileForJUnitTestOutputs* next)
    : name_(filename)
    , is_open_(true)
    , next_(next)
  {
  }

  FileForJUnitTestOutputs* next_file() { return next_; }

  cppmu::String name() { return name_; }

  void write(const cppmu::String& buffer) { buffer_ += buffer; }

  void close() { is_open_ = false; }

  const char* line(size_t line_number)
  {
    lines_of_file_.split_string(buffer_, '\n');
    return lines_of_file_[line_number - 1].c_str();
  }

  const char* line_from_the_back(size_t line_number_from_the_back)
  {
    return line(amount_of_lines() - (line_number_from_the_back - 1));
  }

  size_t amount_of_lines()
  {
    lines_of_file_.split_string(buffer_, '\n');
    return lines_of_file_.size();
  }

  cppmu::String content() { return buffer_; }
};

class FileSystemForJUnitTestOutputTests
{
  FileForJUnitTestOutputs* first_file_;

public:
  FileSystemForJUnitTestOutputTests()
    : first_file_(nullptr)
  {
  }
  ~FileSystemForJUnitTestOutputTests() { clear(); }

  void clear(void)
  {
    while (first_file_) {
      FileForJUnitTestOutputs* file_to_be_deleted = first_file_;
      first_file_ = first_file_->next_file();
      delete file_to_be_deleted;
    }
  }

  FileForJUnitTestOutputs* open_file(const cppmu::String& filename)
  {
    first_file_ = new FileForJUnitTestOutputs(filename, first_file_);
    return first_file_;
  }

  int amount_of_files()
  {
    int total_amount_of_files = 0;
    for (FileForJUnitTestOutputs* current = first_file_; current != nullptr;
        current = current->next_file())
      total_amount_of_files++;
    return total_amount_of_files;
  }

  bool file_exists(const char* filename)
  {
    FileForJUnitTestOutputs* searched_file = file(filename);
    return (searched_file != nullptr);
  }

  FileForJUnitTestOutputs* file(const char* filename)
  {
    for (FileForJUnitTestOutputs* current = first_file_; current != nullptr;
        current = current->next_file())
      if (current->name() == filename)
        return current;
    return nullptr;
  }
};

unsigned long millis_time = 0;
const char* the_time = "";

unsigned long
mock_get_time_in_millis()
{
  return millis_time;
}

const char*
mock_get_time_string()
{
  return the_time;
}

class JUnitTestOutputTestRunner
{
  cppmu::TestResult result_;

  const char* current_group_name_;
  cppmu::TestShell* current_test_;
  bool first_test_in_group_;
  unsigned int time_the_test_takes_;
  unsigned int number_of_checks_in_test_;
  cppmu::TestFailure* test_failure_;

public:
  explicit JUnitTestOutputTestRunner(const cppmu::TestResult& result)
    : result_(result)
    , current_group_name_(nullptr)
    , current_test_(nullptr)
    , first_test_in_group_(true)
    , time_the_test_takes_(0)
    , number_of_checks_in_test_(0)
    , test_failure_(nullptr)
  {
    millis_time = 0;
    the_time = "1978-10-03T00:00:00";

    CPPMU_PTR_SET(cppmu::get_time_in_millis, mock_get_time_in_millis);
    CPPMU_PTR_SET(cppmu::get_time_string, mock_get_time_string);
  }

  JUnitTestOutputTestRunner& start()
  {
    result_.tests_started();
    return *this;
  }

  JUnitTestOutputTestRunner& end()
  {
    end_of_previous_test_group();
    delete current_test_;
    result_.tests_ended();
    return *this;
  }

  JUnitTestOutputTestRunner& end_group_and_clear_test()
  {
    end_of_previous_test_group();
    delete current_test_;
    current_test_ = nullptr;
    return *this;
  }

  void end_of_previous_test_group()
  {
    run_previous_test();
    if (current_test_) {
      result_.current_group_ended(current_test_);
      first_test_in_group_ = true;
    }

    current_group_name_ = nullptr;
  }

  JUnitTestOutputTestRunner& with_group(const char* group_name)
  {
    run_previous_test();
    end_of_previous_test_group();

    current_group_name_ = group_name;
    return *this;
  }

  JUnitTestOutputTestRunner& with_test(const char* test_name)
  {
    run_previous_test();
    delete current_test_;

    current_test_ =
        new cppmu::TestShell(current_group_name_, test_name, "file", 1);
    return *this;
  }

  JUnitTestOutputTestRunner& with_ignored_test(const char* test_name)
  {
    run_previous_test();
    delete current_test_;

    current_test_ =
        new cppmu::IgnoredTestShell(current_group_name_, test_name, "file", 1);
    return *this;
  }

  JUnitTestOutputTestRunner& in_file(const char* file_name)
  {
    if (current_test_) {
      current_test_->set_file_name(file_name);
    }
    return *this;
  }

  JUnitTestOutputTestRunner& on_line(size_t line_number)
  {
    if (current_test_) {
      current_test_->set_line_number(line_number);
    }
    return *this;
  }

  void run_previous_test()
  {
    if (current_test_ == nullptr)
      return;

    if (first_test_in_group_) {
      result_.current_group_started(current_test_);
      first_test_in_group_ = false;
    }
    result_.current_test_started(current_test_);

    millis_time += time_the_test_takes_;
    for (unsigned int i = 0; i < number_of_checks_in_test_; i++) {
      result_.count_check();
    }
    number_of_checks_in_test_ = 0;

    if (test_failure_) {
      result_.add_failure(*test_failure_);
      delete test_failure_;
      test_failure_ = nullptr;
    }

    result_.current_test_ended(current_test_);
  }

  JUnitTestOutputTestRunner& that_has_checks(unsigned int num_of_checks)
  {
    number_of_checks_in_test_ = num_of_checks;
    return *this;
  }

  JUnitTestOutputTestRunner& that_takes(unsigned int time_elapsed)
  {
    time_the_test_takes_ = time_elapsed;
    return *this;
  }

  JUnitTestOutputTestRunner& seconds() { return *this; }

  JUnitTestOutputTestRunner& that_fails(const char* message,
      const char* file,
      size_t line)
  {
    test_failure_ = new cppmu::TestFailure(current_test_, file, line, message);
    return *this;
  }

  JUnitTestOutputTestRunner& at_time(const char* new_time)
  {
    the_time = new_time;
    return *this;
  }

  JUnitTestOutputTestRunner& that_prints(const char* output)
  {
    run_previous_test();
    result_.print(output);
    return *this;
  }
};

FileSystemForJUnitTestOutputTests file_system;
FileForJUnitTestOutputs* current_file = nullptr;

cppmu::File
mock_f_open(const char* filename, const char*)
{
  current_file = file_system.open_file(filename);
  return current_file;
}

void (*original_f_puts)(const char* str, cppmu::File file);
void
mock_f_puts(const char* str, cppmu::File file)
{
  if (file == current_file) {
    static_cast<FileForJUnitTestOutputs*>(file)->write(str);
  } else {
    original_f_puts(str, file);
  }
}

void
mock_f_close(cppmu::File file)
{
  current_file = nullptr;
  static_cast<FileForJUnitTestOutputs*>(file)->close();
}

} // namespace

TEST_GROUP(JUnitTestOutput)
{
  cppmu::JUnitTestOutput* junit_output;
  cppmu::TestResult* result;
  JUnitTestOutputTestRunner* test_case_runner;
  FileForJUnitTestOutputs* output_file;

  void setup() override
  {
    CPPMU_PTR_SET(cppmu::f_open, mock_f_open);
    original_f_puts = cppmu::f_puts;
    CPPMU_PTR_SET(cppmu::f_puts, mock_f_puts);
    CPPMU_PTR_SET(cppmu::f_close, mock_f_close);
    junit_output = new cppmu::JUnitTestOutput();
    result = new cppmu::TestResult(*junit_output);
    test_case_runner = new JUnitTestOutputTestRunner(*result);
  }

  void teardown() override
  {
    delete test_case_runner;
    delete result;
    delete junit_output;
    file_system.clear();
  }
};

TEST(JUnitTestOutput, withOneTestGroupAndOneTestOnlyWriteToOneFile)
{
  test_case_runner->start().with_group("groupname").with_test("testname").end();

  LONGS_EQUAL(1, file_system.amount_of_files());
  CHECK(file_system.file_exists("cppmu_groupname.xml"));
}

TEST(JUnitTestOutput,
    withReservedCharactersInPackageOrTestGroupUsesUnderscoresForFileName)
{
  junit_output->set_package_name("p/a\\c?k%a*g:e|n\"a<m>e.");
  test_case_runner->start()
      .with_group("g/r\\o?u%p*n:a|m\"e<h>ere")
      .with_test("testname")
      .end();

  CHECK(file_system.file_exists(
      "cppmu_p_a_c_k_a_g_e_n_a_m_e._g_r_o_u_p_n_a_m_e_h_ere.xml"));
}

TEST(JUnitTestOutput, withOneTestGroupAndOneTestOutputsValidXMLFiles)
{
  test_case_runner->start().with_group("groupname").with_test("testname").end();

  output_file = file_system.file("cppmu_groupname.xml");
  STRCMP_EQUAL(
      "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n", output_file->line(1));
}

TEST(JUnitTestOutput,
    withOneTestGroupAndOneTestOutputsTestSuiteStartAndEndBlocks)
{
  test_case_runner->start().with_group("groupname").with_test("testname").end();

  output_file = file_system.file("cppmu_groupname.xml");
  STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" "
               "name=\"groupname\" tests=\"1\" time=\"0.000\" "
               "timestamp=\"1978-10-03T00:00:00\">\n",
      output_file->line(2));
  STRCMP_EQUAL("</testsuite>\n", output_file->line_from_the_back(1));
}

TEST(JUnitTestOutput,
    withOneTestGroupAndOneTestFileShouldContainAnEmptyPropertiesBlock)
{
  test_case_runner->start().with_group("groupname").with_test("testname").end();

  output_file = file_system.file("cppmu_groupname.xml");
  STRCMP_EQUAL("<properties>\n", output_file->line(3));
  STRCMP_EQUAL("</properties>\n", output_file->line(4));
}

TEST(JUnitTestOutput,
    withOneTestGroupAndOneTestFileShouldContainAnEmptyStdoutBlock)
{
  test_case_runner->start().with_group("groupname").with_test("testname").end();

  output_file = file_system.file("cppmu_groupname.xml");
  STRCMP_EQUAL(
      "<system-out></system-out>\n", output_file->line_from_the_back(3));
}

TEST(JUnitTestOutput,
    withOneTestGroupAndOneTestFileShouldContainAnEmptyStderrBlock)
{
  test_case_runner->start().with_group("groupname").with_test("testname").end();

  output_file = file_system.file("cppmu_groupname.xml");
  STRCMP_EQUAL(
      "<system-err></system-err>\n", output_file->line_from_the_back(2));
}

TEST(JUnitTestOutput,
    withOneTestGroupAndOneTestFileShouldContainsATestCaseBlock)
{
  test_case_runner->start().with_group("groupname").with_test("testname").end();

  output_file = file_system.file("cppmu_groupname.xml");

  STRCMP_EQUAL("<testcase classname=\"groupname\" name=\"testname\" "
               "assertions=\"0\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
  STRCMP_EQUAL("</testcase>\n", output_file->line(6));
}

TEST(JUnitTestOutput,
    withOneTestGroupAndTwoTestCasesCreateCorrectTestgroupBlockAndCorrectTestCaseBlock)
{
  test_case_runner->start()
      .with_group("twoTestsGroup")
      .with_test("firstTestName")
      .with_test("secondTestName")
      .end();

  output_file = file_system.file("cppmu_twoTestsGroup.xml");

  STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" "
               "name=\"twoTestsGroup\" tests=\"2\" time=\"0.000\" "
               "timestamp=\"1978-10-03T00:00:00\">\n",
      output_file->line(2));
  STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" "
               "assertions=\"0\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
  STRCMP_EQUAL("</testcase>\n", output_file->line(6));
  STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" "
               "assertions=\"0\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(7));
  STRCMP_EQUAL("</testcase>\n", output_file->line(8));
}

TEST(JUnitTestOutput, withOneTestGroupAndTimeHasElapsedAndTimestampChanged)
{
  test_case_runner->start()
      .at_time("2013-07-04T22:28:00")
      .with_group("timeGroup")
      .with_test("Dummy")
      .that_takes(10)
      .seconds()
      .end();

  output_file = file_system.file("cppmu_timeGroup.xml");

  STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" "
               "name=\"timeGroup\" tests=\"1\" time=\"0.010\" "
               "timestamp=\"2013-07-04T22:28:00\">\n",
      output_file->line(2));
}

TEST(JUnitTestOutput, withOneTestGroupAndMultipleTestCasesWithElapsedTime)
{
  test_case_runner->start()
      .with_group("twoTestsGroup")
      .with_test("firstTestName")
      .that_takes(10)
      .seconds()
      .with_test("secondTestName")
      .that_takes(50)
      .seconds()
      .end();

  output_file = file_system.file("cppmu_twoTestsGroup.xml");
  STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" "
               "name=\"twoTestsGroup\" tests=\"2\" time=\"0.060\" "
               "timestamp=\"1978-10-03T00:00:00\">\n",
      output_file->line(2));
  STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" "
               "assertions=\"0\" time=\"0.010\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
  STRCMP_EQUAL("</testcase>\n", output_file->line(6));
  STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" "
               "assertions=\"0\" time=\"0.050\" file=\"file\" line=\"1\">\n",
      output_file->line(7));
  STRCMP_EQUAL("</testcase>\n", output_file->line(8));
}

TEST(JUnitTestOutput, withOneTestGroupAndOneFailingTest)
{
  test_case_runner->start()
      .with_group("testGroupWithFailingTest")
      .with_test("FailingTestName")
      .that_fails("Test failed", "thisfile", 10)
      .end();

  output_file = file_system.file("cppmu_testGroupWithFailingTest.xml");
  STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"1\" hostname=\"localhost\" "
               "name=\"testGroupWithFailingTest\" tests=\"1\" time=\"0.000\" "
               "timestamp=\"1978-10-03T00:00:00\">\n",
      output_file->line(2));
  STRCMP_EQUAL("<testcase classname=\"testGroupWithFailingTest\" "
               "name=\"FailingTestName\" "
               "assertions=\"0\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
  STRCMP_EQUAL("<failure message=\"thisfile:10: Test failed\" "
               "type=\"AssertionFailedError\">\n",
      output_file->line(6));
  STRCMP_EQUAL("</failure>\n", output_file->line(7));
  STRCMP_EQUAL("</testcase>\n", output_file->line(8));
}

TEST(JUnitTestOutput, withTwoTestGroupAndOneFailingTest)
{
  test_case_runner->start()
      .with_group("testGroupWithFailingTest")
      .with_test("FirstTest")
      .with_test("FailingTestName")
      .that_fails("Test failed", "thisfile", 10)
      .end();

  output_file = file_system.file("cppmu_testGroupWithFailingTest.xml");

  STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"1\" hostname=\"localhost\" "
               "name=\"testGroupWithFailingTest\" tests=\"2\" time=\"0.000\" "
               "timestamp=\"1978-10-03T00:00:00\">\n",
      output_file->line(2));
  STRCMP_EQUAL("<testcase classname=\"testGroupWithFailingTest\" "
               "name=\"FailingTestName\" "
               "assertions=\"0\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(7));
  STRCMP_EQUAL("<failure message=\"thisfile:10: Test failed\" "
               "type=\"AssertionFailedError\">\n",
      output_file->line(8));
}

TEST(JUnitTestOutput, testFailureWithLessThanAndGreaterThanInsideIt)
{
  test_case_runner->start()
      .with_group("testGroupWithFailingTest")
      .with_test("FailingTestName")
      .that_fails("Test <failed>", "thisfile", 10)
      .end();

  output_file = file_system.file("cppmu_testGroupWithFailingTest.xml");

  STRCMP_EQUAL("<failure message=\"thisfile:10: Test &lt;failed&gt;\" "
               "type=\"AssertionFailedError\">\n",
      output_file->line(6));
}

TEST(JUnitTestOutput, testFailureWithQuotesInIt)
{
  test_case_runner->start()
      .with_group("testGroupWithFailingTest")
      .with_test("FailingTestName")
      .that_fails("Test \"failed\"", "thisfile", 10)
      .end();

  output_file = file_system.file("cppmu_testGroupWithFailingTest.xml");

  STRCMP_EQUAL("<failure message=\"thisfile:10: Test &quot;failed&quot;\" "
               "type=\"AssertionFailedError\">\n",
      output_file->line(6));
}

TEST(JUnitTestOutput, testFailureWithNewlineInIt)
{
  test_case_runner->start()
      .with_group("testGroupWithFailingTest")
      .with_test("FailingTestName")
      .that_fails("Test \nfailed", "thisfile", 10)
      .end();

  output_file = file_system.file("cppmu_testGroupWithFailingTest.xml");

  STRCMP_EQUAL("<failure message=\"thisfile:10: Test &#10;failed\" "
               "type=\"AssertionFailedError\">\n",
      output_file->line(6));
}

TEST(JUnitTestOutput, testFailureWithDifferentFileAndLine)
{
  test_case_runner->start()
      .with_group("testGroupWithFailingTest")
      .with_test("FailingTestName")
      .that_fails("Test failed", "importantFile", 999)
      .end();

  output_file = file_system.file("cppmu_testGroupWithFailingTest.xml");

  STRCMP_EQUAL("<failure message=\"importantFile:999: Test failed\" "
               "type=\"AssertionFailedError\">\n",
      output_file->line(6));
}

TEST(JUnitTestOutput, testFailureWithAmpersandsAndLessThan)
{
  test_case_runner->start()
      .with_group("testGroupWithFailingTest")
      .with_test("FailingTestName")
      .that_fails("&object1 < &object2", "importantFile", 999)
      .end();

  output_file = file_system.file("cppmu_testGroupWithFailingTest.xml");

  STRCMP_EQUAL("<failure message=\"importantFile:999: &amp;object1 &lt; "
               "&amp;object2\" type=\"AssertionFailedError\">\n",
      output_file->line(6));
}

TEST(JUnitTestOutput, testFailureWithAmpersands)
{
  test_case_runner->start()
      .with_group("testGroupWithFailingTest")
      .with_test("FailingTestName")
      .that_fails("&object1 != &object2", "importantFile", 999)
      .end();

  output_file = file_system.file("cppmu_testGroupWithFailingTest.xml");

  STRCMP_EQUAL("<failure message=\"importantFile:999: &amp;object1 != "
               "&amp;object2\" type=\"AssertionFailedError\">\n",
      output_file->line(6));
}

TEST(JUnitTestOutput, aCoupleOfTestFailures)
{
  test_case_runner->start()
      .with_group("testGroup")
      .with_test("passingOne")
      .with_test("FailingTest")
      .that_fails("Failure", "file", 99)
      .with_test("passingTwo")
      .with_test("passingThree")
      .with_test("AnotherFailingTest")
      .that_fails("otherFailure", "anotherFile", 10)
      .end();

  output_file = file_system.file("cppmu_testGroup.xml");

  STRCMP_EQUAL(
      "<failure message=\"file:99: Failure\" type=\"AssertionFailedError\">\n",
      output_file->line(8));
  STRCMP_EQUAL("<failure message=\"anotherFile:10: otherFailure\" "
               "type=\"AssertionFailedError\">\n",
      output_file->line(16));
}

TEST(JUnitTestOutput, testFailuresInSeparateGroups)
{
  test_case_runner->start()
      .with_group("testGroup")
      .with_test("passingOne")
      .with_test("FailingTest")
      .that_fails("Failure", "file", 99)
      .with_group("AnotherGroup")
      .with_test("AnotherFailingTest")
      .that_fails("otherFailure", "anotherFile", 10)
      .end();

  output_file = file_system.file("cppmu_testGroup.xml");

  STRCMP_EQUAL(
      "<failure message=\"file:99: Failure\" type=\"AssertionFailedError\">\n",
      output_file->line(8));

  output_file = file_system.file("cppmu_AnotherGroup.xml");
  STRCMP_EQUAL("<failure message=\"anotherFile:10: otherFailure\" "
               "type=\"AssertionFailedError\">\n",
      output_file->line(8));
}

TEST(JUnitTestOutput, twoTestGroupsWriteToTwoDifferentFiles)
{
  test_case_runner->start()
      .with_group("firstTestGroup")
      .with_test("testName")
      .with_group("secondTestGroup")
      .with_test("testName")
      .end();

  CHECK(file_system.file("cppmu_firstTestGroup.xml") != nullptr);
  CHECK(file_system.file("cppmu_secondTestGroup.xml") != nullptr);
}

TEST(JUnitTestOutput, testGroupWithWeirdName)
{
  STRCMP_EQUAL("cppmu_group_weird_name.xml",
      junit_output->create_file_name("group/weird/name").c_str());
}

TEST(JUnitTestOutput, TestCaseBlockWithAPackageName)
{
  junit_output->set_package_name("packagename");
  test_case_runner->start().with_group("groupname").with_test("testname").end();

  output_file = file_system.file("cppmu_packagename_groupname.xml");

  STRCMP_EQUAL(
      "<testcase classname=\"packagename.groupname\" name=\"testname\" "
      "assertions=\"0\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
  STRCMP_EQUAL("</testcase>\n", output_file->line(6));
}

TEST(JUnitTestOutput, TestCaseBlockForIgnoredTest)
{
  junit_output->set_package_name("packagename");
  test_case_runner->start()
      .with_group("groupname")
      .with_ignored_test("testname")
      .end();

  output_file = file_system.file("cppmu_packagename_groupname.xml");

  STRCMP_EQUAL(
      "<testcase classname=\"packagename.groupname\" name=\"testname\" "
      "assertions=\"0\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
  STRCMP_EQUAL("<skipped />\n", output_file->line(6));
  STRCMP_EQUAL("</testcase>\n", output_file->line(7));
}

TEST(JUnitTestOutput, TestCaseWithTestLocation)
{
  junit_output->set_package_name("packagename");
  test_case_runner->start()
      .with_group("groupname")
      .with_test("testname")
      .in_file("MySource.c")
      .on_line(159)
      .end();

  output_file = file_system.file("cppmu_packagename_groupname.xml");

  STRCMP_EQUAL(
      "<testcase classname=\"packagename.groupname\" name=\"testname\" "
      "assertions=\"0\" time=\"0.000\" file=\"MySource.c\" line=\"159\">\n",
      output_file->line(5));
}

TEST(JUnitTestOutput, MultipleTestCaseWithTestLocations)
{
  test_case_runner->start()
      .with_group("twoTestsGroup")
      .with_test("firstTestName")
      .in_file("MyFirstSource.c")
      .on_line(846)
      .with_test("secondTestName")
      .in_file("MySecondSource.c")
      .on_line(513)
      .end();

  output_file = file_system.file("cppmu_twoTestsGroup.xml");

  STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" "
               "assertions=\"0\" time=\"0.000\" file=\"MyFirstSource.c\" "
               "line=\"846\">\n",
      output_file->line(5));
  STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" "
               "assertions=\"0\" time=\"0.000\" file=\"MySecondSource.c\" "
               "line=\"513\">\n",
      output_file->line(7));
}

TEST(JUnitTestOutput, TestCaseBlockWithAssertions)
{
  junit_output->set_package_name("packagename");
  test_case_runner->start()
      .with_group("groupname")
      .with_test("testname")
      .that_has_checks(24)
      .end();

  output_file = file_system.file("cppmu_packagename_groupname.xml");

  STRCMP_EQUAL(
      "<testcase classname=\"packagename.groupname\" name=\"testname\" "
      "assertions=\"24\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
}

TEST(JUnitTestOutput, MultipleTestCaseBlocksWithAssertions)
{
  test_case_runner->start()
      .with_group("twoTestsGroup")
      .with_test("firstTestName")
      .that_has_checks(456)
      .with_test("secondTestName")
      .that_has_checks(567)
      .end();

  output_file = file_system.file("cppmu_twoTestsGroup.xml");

  STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" "
               "assertions=\"456\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
  STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" "
               "assertions=\"567\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(7));
}

TEST(JUnitTestOutput, MultipleTestCasesInDifferentGroupsWithAssertions)
{
  test_case_runner->start()
      .with_group("groupOne")
      .with_test("testA")
      .that_has_checks(456)
      .end_group_and_clear_test()
      .with_group("groupTwo")
      .with_test("testB")
      .that_has_checks(678)
      .end();

  output_file = file_system.file("cppmu_groupOne.xml");
  STRCMP_EQUAL("<testcase classname=\"groupOne\" name=\"testA\" "
               "assertions=\"456\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));

  output_file = file_system.file("cppmu_groupTwo.xml");
  STRCMP_EQUAL("<testcase classname=\"groupTwo\" name=\"testB\" "
               "assertions=\"678\" time=\"0.000\" file=\"file\" line=\"1\">\n",
      output_file->line(5));
}

TEST(JUnitTestOutput, UTPRINTOutputInJUnitOutput)
{
  test_case_runner->start()
      .with_group("groupname")
      .with_test("testname")
      .that_prints("someoutput")
      .end();

  output_file = file_system.file("cppmu_groupname.xml");
  STRCMP_EQUAL("<system-out>someoutput</system-out>\n",
      output_file->line_from_the_back(3));
}

TEST(JUnitTestOutput, UTPRINTOutputInJUnitOutputWithSpecials)
{
  test_case_runner->start()
      .with_group("groupname")
      .with_test("testname")
      .that_prints(
          "The <rain> in \"Spain\"\nGoes\r \\mainly\\ down the Dr&in\n")
      .end();

  output_file = file_system.file("cppmu_groupname.xml");
  STRCMP_EQUAL(
      "<system-out>The &lt;rain&gt; in &quot;Spain&quot;&#10;Goes&#13; "
      "\\mainly\\ down the Dr&amp;in&#10;</system-out>\n",
      output_file->line_from_the_back(3));
}
