Command-Line Reference
======================

All flags are passed directly to the test executable:

.. code-block:: bash

   ./my_tests [flags...]

Run ``./my_tests -h`` to print the built-in help screen (also lists any
plugin-provided flags).

Query Flags (do not run tests)
-------------------------------

.. list-table::
   :header-rows: 1

   * - Flag
     - Description
   * - ``-h``
     - Print help screen and exit
   * - ``-lg``
     - Print all group names, space-separated
   * - ``-ln``
     - Print all test names as ``group.name``, space-separated
   * - ``-ll``
     - Print test locations as ``group.name.file.line``
   * - ``-llo``
     - Print ordered test locations as ``group.name.file.line``

Output Flags
------------

.. list-table::
   :header-rows: 1

   * - Flag
     - Description
   * - ``-c``
     - Colorize output: green for pass, red for fail
   * - ``-v``
     - Verbose: print each test name as it runs
   * - ``-vv``
     - Very verbose: print internal framework state

Plugin Flags
------------

Plugin flags begin with ``-p``. The plugin must be installed in
``main()`` to parse them.

.. list-table::
   :header-rows: 1

   * - Flag
     - Plugin
     - Description
   * - ``-pjunit``
     - ``JUnitOutputPlugin``
     - Write JUnit XML output files. See :doc:`junit-output`

Filter Flags
------------

Multiple filter flags can be combined; all must match for a test to run.

Include filters (substring match by default)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1

   * - Flag
     - Matches
   * - ``-g <group>``
     - Tests whose group name *contains* ``<group>``
   * - ``-n <name>``
     - Tests whose test name *contains* ``<name>``
   * - ``-t <group>.<name>``
     - Tests whose group contains ``<group>`` **and** name contains ``<name>``

Include filters (exact match)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1

   * - Flag
     - Matches
   * - ``-sg <group>``
     - Tests whose group *exactly equals* ``<group>``
   * - ``-sn <name>``
     - Tests whose test name *exactly equals* ``<name>``
   * - ``-st <group>.<name>``
     - Both group and name exactly equal

Exclude filters (substring match)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1

   * - Flag
     - Excludes
   * - ``-xg <group>``
     - Tests whose group contains ``<group>``
   * - ``-xn <name>``
     - Tests whose name contains ``<name>``
   * - ``-xt <group>.<name>``
     - Tests whose group and name both contain the respective substrings

Exclude filters (exact match)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1

   * - Flag
     - Excludes
   * - ``-xsg <group>``
     - Tests whose group exactly equals ``<group>``
   * - ``-xsn <name>``
     - Tests whose name exactly equals ``<name>``
   * - ``-xst <group>.<name>``
     - Both group and name exactly equal

Copy-paste filter
~~~~~~~~~~~~~~~~~

Verbose output (``-v``) prints lines like ``TEST(Widget, CanBeSet)``.
You can paste this directly on the command line to run only that test:

.. code-block:: bash

   ./my_tests "TEST(Widget, CanBeSet)"
   ./my_tests "IGNORE_TEST(Widget, Skipped)"

This performs an exact match on both group and name.

Execution Flags
---------------

.. list-table::
   :header-rows: 1

   * - Flag
     - Description
   * - ``-b``
     - Run tests in reverse registration order
   * - ``-s [seed]``
     - Shuffle test execution order randomly. Optional ``seed`` (integer > 0)
       makes the order reproducible; if omitted, seed is time-based and
       printed to console
   * - ``-r[#]``
     - Repeat the full test suite ``#`` times. If ``#`` is omitted, repeats
       twice. Useful for detecting order-dependent failures
   * - ``-ri``
     - Run ignored tests (:c:macro:`IGNORE_TEST`) as if they were normal tests
   * - ``-f``
     - Crash on first failure instead of continuing (useful with a debugger)
   * - ``-e``
     - Do not rethrow unexpected exceptions as failures (catch and continue)
