Test Definition Macros
======================

Include ``"mutiny/test.hpp"`` for all macros on this page.

Test Group and Lifecycle
------------------------

``TEST_GROUP(group)``
~~~~~~~~~~~~~~~~~~~~~

:c:macro:`TEST_GROUP` declares a test group. Expands to a struct definition; add
:cpp:func:`setup() <mu::tiny::test::Test::setup>` and
:cpp:func:`teardown() <mu::tiny::test::Test::teardown>` inside it.

.. code-block:: cpp

   TEST_GROUP(MyGroup)
   {
       MyObject* obj;

       void setup() override    { obj = new MyObject; }
       void teardown() override { delete obj; }
   };

All tests in the group share a fresh instance of the struct (constructed
before :cpp:func:`setup() <mu::tiny::test::Test::setup>`, destroyed after
:cpp:func:`teardown() <mu::tiny::test::Test::teardown>`).

``TEST(group, name)``
~~~~~~~~~~~~~~~~~~~~~

:c:macro:`TEST` defines a test belonging to ``group``. The ``{ ... }`` block that
follows is the test body.

.. code-block:: cpp

   TEST(MyGroup, DoesTheThing)
   {
       CHECK(obj->do_thing());
   }

Skipping and Expected Failures
-------------------------------

``IGNORE_TEST(group, name)``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:c:macro:`IGNORE_TEST` marks a test as ignored. It is registered but skipped during
normal runs. Use :option:`-ri` to run ignored tests anyway.

.. code-block:: cpp

   IGNORE_TEST(MyGroup, NotImplementedYet)
   {
       // will not run unless -ri is passed
       CHECK(false);
   }

Ignored tests appear in the summary count as "ignored".

``EXPECT_FAIL_TEST(group, name)``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:c:macro:`EXPECT_FAIL_TEST` declares a test that is expected to fail. The test runner
treats a failure as a pass and a pass as a failure. Useful for
documenting known bugs that cannot yet be fixed.

.. code-block:: cpp

   EXPECT_FAIL_TEST(MyGroup, KnownBug)
   {
       CHECK_EQUAL(expected, buggy_function()); // currently produces wrong result
   }

Metadata
--------

``TEST_PROPERTY(name, value)``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:c:macro:`TEST_PROPERTY` attaches a key/value metadata pair to the currently running
test. Emitted in :doc:`JUnit XML output <junit-output>` as ``<property>`` elements. Useful for
CI tagging (e.g. requirement IDs).

.. code-block:: cpp

   TEST(MyGroup, Requirement42)
   {
       TEST_PROPERTY("requirement", "REQ-42");
       CHECK(meets_requirement_42());
   }

C Wrapper Macros
----------------

These bridge C test files into the C++ test runner. See
:doc:`testing-in-c` for full details.

.. list-table::
   :header-rows: 1

   * - Macro
     - Purpose
   * - :c:macro:`TEST_GROUP_C_WRAPPER(group) <TEST_GROUP_C_WRAPPER>`
     - Declares a C++ :c:macro:`TEST_GROUP` that calls C setup/teardown
   * - :c:macro:`TEST_GROUP_C_SETUP_WRAPPER(group) <TEST_GROUP_C_SETUP_WRAPPER>`
     - Wires C setup into the C++ group's :cpp:func:`setup() <mu::tiny::test::Test::setup>`
   * - :c:macro:`TEST_GROUP_C_TEARDOWN_WRAPPER(group) <TEST_GROUP_C_TEARDOWN_WRAPPER>`
     - Wires C teardown into the C++ group's :cpp:func:`teardown() <mu::tiny::test::Test::teardown>`
   * - :c:macro:`TEST_C_WRAPPER(group, name) <TEST_C_WRAPPER>`
     - Wires a C test function into a C++ :c:macro:`TEST`
   * - :c:macro:`IGNORE_TEST_C_WRAPPER(group, name) <IGNORE_TEST_C_WRAPPER>`
     - Same as above but ignored
   * - :c:macro:`EXPECT_FAIL_TEST_C_WRAPPER(group, name) <EXPECT_FAIL_TEST_C_WRAPPER>`
     - Same as above but expected to fail

Examples
--------

.. list-table::
   :header-rows: 1

   * - File
     - Demonstrates
   * - :source:`CheatSheet.test.cpp <examples/tests/CheatSheet.test.cpp>`
     - :c:macro:`TEST_GROUP`, :c:macro:`TEST`, :cpp:func:`setup() <mu::tiny::test::Test::setup>`, :cpp:func:`teardown() <mu::tiny::test::Test::teardown>`, and :c:macro:`MUTINY_PTR_SET`
   * - :source:`hello.test.c <examples/tests/hello.test.c>` +
       :source:`hello.test.cpp <examples/tests/hello.test.cpp>`
     - C-wrapper two-file pattern with :c:macro:`TEST_GROUP_SETUP`, :c:macro:`TEST`, and :c:macro:`TEST_C_WRAPPER`
