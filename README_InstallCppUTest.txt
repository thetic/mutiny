1. Clone or unzip CppUTest into <someDirectory>, resulting in
		<someDirectory>/CppUTest/

   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT
   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT
   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT

2. Build CppUTest

 > cd <someDirectory>/CppUTest
 > cmake -B build
 > cmake --build build
 > ctest --test-dir build   # Run the CppUTest unit tests

3. Install (optional)

 > cmake --install build

