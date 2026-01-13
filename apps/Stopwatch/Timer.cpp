#include "stdafx.h"
#include "Timer.h"
#include "../../libs/DNX.App/ValueConverter.h"
#include "../../libs/DNX.Utils/DateUtils.h"
#include "../../libs/DNX.Utils/ListUtils.h"
#include "../../libs/DNX.Utils/StringUtils.h"
#include "Commands/ListCommand.h"
#include <chrono>
#include <ctime>
#include <sstream>

using namespace std;
using namespace Stopwatch;
using namespace DNX::App;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyConcurrencyMtUnsafe

Timer Timer::m_empty_timer = Timer();

Timer::Timer() :
    m_Start(DateUtils::GetNow()),
    m_State(TimerStateType::INACTIVE),
    m_TotalElapsed(0)
{
}

Timer::Timer(const string& name)
{
    m_Name = name;
    m_Start = DateUtils::GetNow();
    m_State = TimerStateType::INACTIVE;
    m_TotalElapsed = 0;
}

bool Timer::CompareByName(const Timer& first, const Timer& second)
{
    return first.GetName() < second.GetName();
}

bool Timer::CompareByStartTime(const Timer& first, const Timer& second)
{
    return first.GetStart() < second.GetStart();
}

bool Timer::CompareByState(const Timer& first, const Timer& second)
{
    const auto text_resolver = TimerStateTypeTextResolver();

    return text_resolver.GetText(first.GetState()) < text_resolver.GetText(second.GetState());
}
bool Timer::CompareByElapsed(const Timer& first, const Timer& second)
{
    return first.GetTotalElapsed() < second.GetTotalElapsed();
}

string Timer::GetName() const
{
    return m_Name;
}
time_t Timer::GetStart() const
{
    return m_Start;
}
TimerStateType Timer::GetState() const
{
    return m_State;
}
double Timer::GetTotalElapsed() const
{
    return m_TotalElapsed;
}

double Timer::GetCurrentElapsed() const
{
    const auto elapsed = difftime(DateUtils::GetNow(), m_Start);

    return elapsed;
}

double Timer::GetAccumulatedElapsed() const
{
    double elapsed_time = GetTotalElapsed();

    if (m_State == TimerStateType::ACTIVE)
        elapsed_time += GetCurrentElapsed();

    return elapsed_time;
}

tm Timer::GetStartDateTime() const
{
    const auto start_time = GetStart();
    const auto start_time_parts = DateUtils::ToCalendarDateTime(start_time);
    return start_time_parts;
}

bool Timer::CanStart() const
{
    return (m_State == TimerStateType::INACTIVE);
}
bool Timer::CanStop() const
{
    return (m_State == TimerStateType::ACTIVE)
        || (m_State == TimerStateType::INACTIVE);
}

void Timer::Start()
{
    if (!CanStart())
        return;

    m_Start = DateUtils::GetNow();
    m_State = TimerStateType::ACTIVE;
}

void Timer::Pause()
{
    Stop();
    m_State = TimerStateType::INACTIVE;
}

void Timer::Stop()
{
    if (!CanStop())
        return;

    if (m_State == TimerStateType::ACTIVE)
        m_TotalElapsed += GetCurrentElapsed();
    m_State = TimerStateType::INACTIVE;
}

bool Timer::IsEmpty() const
{
    return m_Name.empty();
}

Timer Timer::Create(const string& definition)
{
    Timer timer("");
    timer.FromDefinition(definition);
    return timer;
}

string Timer::ToDefinition() const
{
    stringstream ss;

    ss << m_Name
        << "|" << to_string(m_Start)
        << "|" << to_string(static_cast<uint8_t>(m_State))
        << "|" << to_string(m_TotalElapsed)
        ;

    return ss.str();
}

void Timer::FromDefinition(const string& definition)
{
    auto parts = StringUtils::SplitText(definition, '|');

    m_Name = ListUtils::GetAt(parts, 0);
    m_Start = static_cast<time_t>(ValueConverter::ToLong(ListUtils::GetAt(parts, 1)));
    m_State = static_cast<TimerStateType>(ValueConverter::ToInt(ListUtils::GetAt(parts, 2)));
    m_TotalElapsed = ValueConverter::ToLong(ListUtils::GetAt(parts, 3));
}
