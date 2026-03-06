1. Clone or unzip CppUTest into <someDirectory>, resulting in
		<someDirectory>/CppUTest/

   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT
   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT
   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT

2. Build CppUTest

 > cd <someDirectory>/CppUTest
 > cmake -B cpputest_build
 > cmake --build cpputest_build
 > ctest --test-dir cpputest_build   # Run the CppUTest unit tests

3. Install (optional)

 > cmake --install cpputest_build

4. To set up the support scripts (unix/cygwin)

 > cd <someDirectory>/CppUTest
 > sudo ./scripts/InstallScripts.sh

This command adds some symbolic links to /usr/local/bin.

CYGWIN - http://www.cygwin.com/
