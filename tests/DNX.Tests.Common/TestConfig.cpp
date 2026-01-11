#include "stdafx.h"
#include "TestConfig.h"
#include "../DNX.Utils/PathUtils.h"

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming

TestConfig::TestConfig(testing::UnitTest* unit_test_instance, const string& executable_name, const string& slug_id)
{
    m_unit_test_instance = unit_test_instance;
    m_executable_name    = executable_name;
    m_slug_id            = slug_id;
}

string TestConfig::GetExpectedOutputFileName() const
{
    return PathUtils::Combine("ExpectedOutput", string(m_unit_test_instance->current_test_info()->name()) + ".txt");
}
