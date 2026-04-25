Command-Line Reference
======================

All flags are passed directly to the test executable:

.. code-block:: bash

   ./my_tests [flags...]

Run ``./my_tests -h`` to print the built-in help screen (also lists any
plugin-provided flags).

Query Flags (do not run tests)
-------------------------------

.. option:: -h, --help

   Print help screen and exit.

.. option:: --list-groups

   Print all group names, space-separated.

.. option:: --list-tests

   Print all test names as ``group.name``, space-separated.

.. option:: --list-locations

   Print test locations as ``group.name.file.line``.

.. option:: --list-ordered-locations

   Print ordered test locations as ``group.name.file.line``.

.. option:: --list-group-locations

   Print group source locations as ``group.file.line``.  Each entry points to
   the :c:macro:`TEST_GROUP` declaration rather than an individual test.

Output Flags
------------

.. option:: -c, --color

   Colorize output: green for pass, red for fail.

.. option:: -v, --verbose, --verbose=1

   Verbose: print each test name as it runs.

.. option:: -vv, --verbose=2

   Very verbose: print internal framework state.

Plugin Flags
------------

Plugin flags begin with ``-p``. The plugin must be installed in
``main()`` to parse them.

.. option:: -p<plugin>

   Invoke the named plugin flag.  Each plugin registered in ``main()``
   receives every ``-p``-prefixed argument and may act on flags it
   recognises.

.. option:: -pjunit

   Write JUnit XML output files (requires :cpp:class:`JUnitOutputPlugin <mu::tiny::test::JUnitOutputPlugin>`).
   See :doc:`junit-output`.

.. option:: -ptap

   Write TAP version 13 output to stdout (requires :cpp:class:`TapOutputPlugin <mu::tiny::test::TapOutputPlugin>`).
   See :doc:`tap-output`.

Filter Flags
------------

Multiple filter flags can be combined; all must match for a test to run.

Include filters (substring match by default)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. option:: -g <group>, --group <group>

   Run tests whose group name *contains* ``<group>``.  Also accepts
   ``--group=<group>``.

.. option:: -n <name>, --name <name>

   Run tests whose test name *contains* ``<name>``.  Also accepts
   ``--name=<name>``.

.. option:: -t <group>.<name>, --test <group>.<name>

   Run tests whose group contains ``<group>`` **and** name contains ``<name>``.
   Also accepts ``--test=<group>.<name>``.

Include filters (exact match)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. option:: --exact-group <group>

   Run tests whose group *exactly equals* ``<group>``.  Also accepts
   ``--exact-group=<group>``.

.. option:: --exact-name <name>

   Run tests whose test name *exactly equals* ``<name>``.  Also accepts
   ``--exact-name=<name>``.

.. option:: --exact-test <group>.<name>

   Run tests where both group and name exactly equal the given values.  Also
   accepts ``--exact-test=<group>.<name>``.

Exclude filters (substring match)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. option:: --exclude-group <group>

   Exclude tests whose group contains ``<group>``.  Also accepts
   ``--exclude-group=<group>``.

.. option:: --exclude-name <name>

   Exclude tests whose name contains ``<name>``.  Also accepts
   ``--exclude-name=<name>``.

.. option:: --exclude-test <group>.<name>

   Exclude tests whose group and name both contain the respective substrings.
   Also accepts ``--exclude-test=<group>.<name>``.

Exclude filters (exact match)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. option:: --exclude-exact-group <group>

   Exclude tests whose group exactly equals ``<group>``.  Also accepts
   ``--exclude-exact-group=<group>``.

.. option:: --exclude-exact-name <name>

   Exclude tests whose name exactly equals ``<name>``.  Also accepts
   ``--exclude-exact-name=<name>``.

.. option:: --exclude-exact-test <group>.<name>

   Exclude tests where both group and name exactly equal the given values.
   Also accepts ``--exclude-exact-test=<group>.<name>``.

Copy-paste filter
~~~~~~~~~~~~~~~~~

Verbose output (:option:`-v`) prints lines like ``TEST(Widget, CanBeSet)``.
You can paste this directly on the command line to run only that test:

.. code-block:: bash

   ./my_tests "TEST(Widget, CanBeSet)"
   ./my_tests "SKIPPED_TEST(Widget, Skipped)"

This performs an exact match on both group and name.

Execution Flags
---------------

.. option:: -b, --reverse

   Run tests in reverse registration order.

.. option:: -s [seed], --shuffle [seed]

   Shuffle test execution order randomly. Optional ``seed`` (integer > 0)
   makes the order reproducible; if omitted, seed is time-based and
   printed to console.  Also accepts ``--shuffle=<seed>``.

.. option:: -r[#], --repeat [#]

   Repeat the full test suite ``#`` times. If ``#`` is omitted, repeats
   twice. Useful for detecting order-dependent failures.  Also accepts
   ``--repeat=<#>``.

.. option:: --run-skipped

   Run skipped tests (:c:macro:`SKIPPED_TEST`) as if they were normal tests.

.. option:: -f, --crash-on-fail

   Crash on first failure instead of continuing (useful with a debugger).

.. option:: -e, --no-rethrow

   Do not rethrow unexpected exceptions as failures (catch and continue).
