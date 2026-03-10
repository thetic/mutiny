#ifndef PLATFORMSPECIFICFUNCTIONS_H_
#define PLATFORMSPECIFICFUNCTIONS_H_

#include "CppUTest/TestOutput.hpp"

namespace cpputest {
TestOutput::WorkingEnvironment
PlatformSpecificGetWorkingEnvironment();

class TestPlugin;
}
/* Platform specific interface we use in order to minimize dependencies with
 * LibC. This enables porting to different embedded platforms.
 *
 */

#include "CppUTest/PlatformSpecificFunctions.h"

#endif
