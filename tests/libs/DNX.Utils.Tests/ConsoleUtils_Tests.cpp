#include "pch.h"
#include "../../../Common/GoogleTest_Utils.h"
#include "../../../libs/DNX.Utils/ConsoleUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP ConsoleUtils

TEST(TEST_GROUP, ShowBlankLines_outputs_the_correct_number_of_lines)
{
    TEST_STDOUT(
        ConsoleUtils::ShowBlankLines(3)
    )
}
