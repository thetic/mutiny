Ordered Tests
=============

Normally mutiny runs tests in an unspecified order (registration order,
possibly shuffled with ``-s``). ``OrderedTest`` lets you assign a
numeric level to selected tests so they run in level order, independent
of the registration order of all other tests.

Include ``"mutiny/test/Ordered.hpp"``.

``TEST_ORDERED(group, name, level)``
-------------------------------------

Declares a test that runs at the given level. Lower levels run first.
Tests at the same level run in registration order relative to each
other.

.. code-block:: cpp

   #include "mutiny/test.hpp"
   #include "mutiny/test/Ordered.hpp"

   TEST_GROUP(Database) {};

   TEST_ORDERED(Database, CreateSchema,  10) { db_create_schema(); }
   TEST_ORDERED(Database, SeedData,      20) { db_seed(); }
   TEST_ORDERED(Database, RunQueries,    30) { run_queries(); }
   TEST_ORDERED(Database, CleanUp,       40) { db_drop_schema(); }

Ordered tests run as a block, separated from regular ``TEST`` tests. The
block of ordered tests runs after all unordered tests by default.

C Wrapper
---------

``TEST_ORDERED_C_WRAPPER(group, name, level)`` wires a C test function
into an ordered test. Use it in the ``.test.cpp`` companion file
alongside ``TEST_C_WRAPPER``:

.. code-block:: cpp

   TEST_ORDERED_C_WRAPPER(Database, CreateSchema, 10)
   TEST_ORDERED_C_WRAPPER(Database, SeedData,     20)

Listing Ordered Tests
---------------------

.. code-block:: bash

   ./my_tests -llo   # print ordered test locations: group.name.file.line

When to Use Ordered Tests
--------------------------

Ordered tests are appropriate when:

- You are integration-testing a stateful system (e.g. a database, a
  file system) where setup steps must precede query steps.
- The test itself mutates persistent state that subsequent tests depend
  on.

Avoid them when:

- Tests can be made independent through ``setup()``/``teardown()`` —
  independent tests are more robust and can be run in isolation.
- You want the ``-s`` (shuffle) flag to help detect ordering
  dependencies — ordered tests are excluded from the shuffle.

Examples
--------

.. list-table::
   :header-rows: 1

   * - File
     - Demonstrates
   * - `OrderedTest.test.cpp <https://github.com/thetic/mutiny/tree/main/examples/tests/OrderedTest.test.cpp>`__
     - Four-phase lifecycle (init → events → verify → shutdown) with two
       tests sharing level 20, plus a regular ``TEST`` that runs before
       the ordered block
