#include "stdafx.h"
#include "Timer.h"
#include <chrono>
#include <ctime>
#include <sstream>

#include "../DNX.App/ValueConverter.h"
#include "../DNX.Utils/ListUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace Stopwatch;
using namespace DNX::App;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyConcurrencyMtUnsafe

Timer Timer::m_empty_timer = Timer();

Timer::Timer() :
    m_Start(time(nullptr)),
    m_State(TimerStateType::PAUSED),
    m_TotalElapsed(0)
{
}

Timer::Timer(const string& name)
{
    m_Name         = name;
    m_Start        = time(nullptr);
    m_State        = TimerStateType::PAUSED;
    m_TotalElapsed = 0;
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
    const auto elapsed = difftime(time(nullptr), m_Start);

    return elapsed;
}

double Timer::GetAccumulatedElapsed() const
{
    double elapsed_time = GetTotalElapsed();

    if (m_State == TimerStateType::RUNNING)
        elapsed_time += GetCurrentElapsed();

    return elapsed_time;
}

tm Timer::GetStartDateTime() const
{
    const auto start_time = GetStart();
    const auto start_time_parts = gmtime(&start_time);
    return *start_time_parts;
}

void Timer::Start()
{
    if (m_State == TimerStateType::RUNNING)
        return;

    m_Start = time(nullptr);
    m_State = TimerStateType::RUNNING;
}
void Timer::Stop()
{
    if (m_State == TimerStateType::PAUSED)
        return;

    m_TotalElapsed += GetCurrentElapsed();
    m_State         = TimerStateType::PAUSED;
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

    m_Name         = ListUtils::GetAt(parts, 0);
    m_Start        = static_cast<time_t>(ValueConverter::ToLong(ListUtils::GetAt(parts, 1)));
    m_State        = static_cast<TimerStateType>(ValueConverter::ToInt(ListUtils::GetAt(parts, 2)));
    m_TotalElapsed = ValueConverter::ToLong(ListUtils::GetAt(parts, 3));
}
