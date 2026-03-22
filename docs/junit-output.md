# JUnit XML Output

CppMu can emit JUnit-compatible XML files via `JUnitOutputPlugin`. CI systems such as Jenkins, GitHub Actions, and GitLab CI can parse this format to display test results in their dashboards.

## Activating JUnit Output

### Via command-line flag

Pass `-pjunit` when running your test executable:

```bash
./my_tests -pjunit
```

### Via plugin in `main()`

Install the plugin before calling `run_all_tests`:

```cpp
#include "CppMu/CommandLineTestRunner.hpp"
#include "CppMu/JUnitOutputPlugin.hpp"
#include "CppMu/TestRegistry.hpp"

int main(int argc, char** argv)
{
    cppmu::JUnitOutputPlugin junit;
    cppmu::TestRegistry::get_current_registry()->install_plugin(&junit);
    return cppmu::CommandLineTestRunner::run_all_tests(argc, argv);
}
```

With the plugin installed, `-pjunit` on the command line enables the XML output. Without the flag, no XML is written and the console output is unchanged.

## Output File Location

JUnit XML is written to the current working directory. One file is created per test group, named `cpputest_<GroupName>.xml`.

For example, a group named `Widget` produces `cpputest_Widget.xml`.

## XML Format

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<testsuite errors="0" failures="1" hostname="localhost" name="Widget" tests="3" time="0.001">
  <testcase classname="Widget" name="ReturnsInitialValue" time="0.000" />
  <testcase classname="Widget" name="CanBeSet" time="0.000">
    <failure message="widget.test.cpp:25: expected &lt;42&gt; but was &lt;0&gt;" type="AssertionError" />
  </testcase>
</testsuite>
```

`TEST_PROPERTY` annotations appear as `<property>` elements inside `<testcase>`.

## CI Integration

### GitHub Actions

```yaml
- name: Run tests
  run: ./build/my_tests -pjunit

- name: Publish test results
  uses: EnricoMi/publish-unit-test-result-action@v2
  with:
    files: cpputest_*.xml
```

### GitLab CI

```yaml
test:
  script:
    - ./build/my_tests -pjunit
  artifacts:
    reports:
      junit: cpputest_*.xml
```

### Jenkins

Configure the "Publish JUnit test result report" post-build step with the pattern `cpputest_*.xml`.
