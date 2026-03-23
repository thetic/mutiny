Getting Started with mutiny
===========================

Building mutiny
---------------

mutiny uses CMake. Clone the repository and configure:

.. code-block:: bash

   cmake -B build
   cmake --build build
   cmake --install build  # optional system install

Adding mutiny to Your Project
-----------------------------

The recommended approach is CMake ``FetchContent``:

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.11)
   project(my_tests)

   include(FetchContent)
   FetchContent_Declare(
       mutiny
       GIT_REPOSITORY https://github.com/thetic/mutiny.git
       GIT_TAG        main
   )
   FetchContent_MakeAvailable(mutiny)

   add_executable(my_tests main.cpp widget.test.cpp)
   target_link_libraries(my_tests PRIVATE mutiny::mutiny)

   include(CTest)
   include(mutiny)
   mutiny_discover_tests(my_tests)

If mutiny is already installed system-wide, use
``find_package(mutiny REQUIRED)`` instead of ``FetchContent``.

Headers
~~~~~~~

All public headers live under ``include/mutiny/``. The main headers you'll use:

.. list-table::
   :header-rows: 1

   * - Header
     - Purpose
   * - ``mutiny/test.hpp``
     - Test definition macros (``TEST_GROUP``, ``TEST``, etc.)
   * - ``mutiny/test/Shell.hpp``
     - Assertion macros (``CHECK``, ``CHECK_EQUAL``, etc.)
   * - ``mutiny/mock.hpp``
     - Mock framework (``mock()``, :cpp:class:`Support <mu::tiny::mock::Support>`)
   * - ``mutiny/test.h``
     - C interface (include in ``.test.c`` files)
   * - ``mutiny/test/CommandLineRunner.hpp``
     - ``main()`` runner
   * - ``mutiny/test/Ordered.hpp``
     - ``TEST_ORDERED`` macro

Writing ``main()``
------------------

Every test executable needs a ``main()``. The simplest form uses
:cpp:class:`CommandLineRunner <mu::tiny::test::CommandLineRunner>`:

.. code-block:: cpp

   #include "mutiny/test/CommandLineRunner.hpp"

   int main(int argc, char** argv)
   {
       return mu::tiny::test::CommandLineRunner::run_all_tests(argc, argv);
   }

To add plugins (e.g. JUnit output, SetPointer,
:cpp:class:`SupportPlugin <mu::tiny::mock::SupportPlugin>`), install them via
:cpp:class:`Registry <mu::tiny::test::Registry>`:

.. code-block:: cpp

   #include "mutiny/test/CommandLineRunner.hpp"
   #include "mutiny/test/JUnitOutputPlugin.hpp"
   #include "mutiny/mock/SupportPlugin.hpp"
   #include "mutiny/test/Registry.hpp"

   int main(int argc, char** argv)
   {
       mu::tiny::test::JUnitOutputPlugin junit;
       mu::tiny::mock::SupportPlugin mock_plugin;
       auto* reg = mu::tiny::test::Registry::get_current_registry();
       reg->install_plugin(&junit);
       reg->install_plugin(&mock_plugin);
       return mu::tiny::test::CommandLineRunner::run_all_tests(argc, argv);
   }

Your First Test
---------------

.. literalinclude:: ../examples/tests/CheatSheet.test.cpp
   :language: cpp

- ``TEST_GROUP`` declares a group; the struct implicitly inherits from
  :cpp:class:`mu::tiny::test::Test`. ``setup()`` runs before each test body;
  ``teardown()`` runs after.
- ``TEST(group, name)`` defines a single test. The body is the
  ``{ ... }`` block that follows.
- A failing assertion immediately exits the test body (via ``longjmp``
  or exception).

Running Tests
-------------

Run the binary directly:

.. code-block:: bash

   ./build/GNU/tests/my_tests                  # run all
   ./build/GNU/tests/my_tests -v               # verbose: print each test name
   ./build/GNU/tests/my_tests -g CheatSheet    # only group "CheatSheet"
   ./build/GNU/tests/my_tests -n TestName      # only tests whose name contains this

Via CTest (after ``mutiny_discover_tests`` in CMakeLists):

.. code-block:: bash

   ctest --preset GNU
   ctest --preset GNU -R CheatSheet

See :doc:`command-line-reference` for all flags.

Examples
--------

.. list-table::
   :header-rows: 1

   * - File
     - Demonstrates
   * - `CheatSheet.test.cpp <https://github.com/thetic/mutiny/tree/main/examples/tests/CheatSheet.test.cpp>`__
     - Minimal ``TEST_GROUP`` + ``TEST`` with ``setup()``, ``MUTINY_PTR_SET``,
       and a handful of assertion macros
   * - `CircularBuffer.test.cpp <https://github.com/thetic/mutiny/tree/main/examples/tests/CircularBuffer.test.cpp>`__
     - Full group with setup/teardown, helper methods, and multiple assertion styles
