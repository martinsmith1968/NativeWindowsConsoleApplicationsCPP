#include "stdafx.h"
#include "NamedTimer.h"
#include <chrono>
#include <ctime>
#include <sstream>

#include "../DNX.App/ValueConverter.h"
#include "../DNX.Utils/ListUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace SimpleTimer;
using namespace DNX::App;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming

NamedTimer::NamedTimer(const string& name)
{
    m_Name    = name;
    m_Start   = time(nullptr);
    m_Elapsed = 0;
}
string NamedTimer::GetName() const
{
    return m_Name;
}
time_t NamedTimer::GetStart() const
{
    return m_Start;
}
long NamedTimer::GetElapsed() const
{
    return m_Elapsed;
}
bool NamedTimer::IsEmpty() const
{
    return m_Name.empty();
}

NamedTimer NamedTimer::Create(const string& definition)
{
    NamedTimer timer("");
    timer.FromDefinition(definition);
    return timer;
}

string NamedTimer::ToDefinition() const
{
    stringstream ss;

    ss << m_Name
        << "|" << to_string(m_Start)
        << "|" << to_string(m_Elapsed)
        ;

    return ss.str();
}

void NamedTimer::FromDefinition(const string& definition)
{
    auto parts = StringUtils::SplitText(definition, '|');

    m_Name  = ListUtils::GetAt(parts, 0);
    m_Start = static_cast<time_t>(stol(ListUtils::GetAt(parts, 1)));
    m_Start = ValueConverter::ToLong(ListUtils::GetAt(parts, 2));
}
