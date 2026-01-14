#include "stdafx.h"
#include "RunIdGenerator.h"
#include "../../libs/DNX.Utils/DateUtils.h"

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming

RunIdGenerator::RunIdGenerator()
{
    m_start_datetime = DateUtils::ToCalendarDateTime(DateUtils::GetNow());

    m_run_id = DateUtils::FormatDate(&m_start_datetime, "%Y%m%d%H%M%S");
}

tm RunIdGenerator::GetStartDateTime() const
{
    return m_start_datetime;
}

string RunIdGenerator::GetRunId() const
{
    return m_run_id;
}
