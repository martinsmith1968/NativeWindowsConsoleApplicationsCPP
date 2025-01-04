#include "stdafx.h"
#include "NamedTimerRepository.h"
#include "NamedTimer.h"
#include "../DNX.Utils/FileUtils.h"

using namespace std;
using namespace SimpleTimer;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming

NamedTimerRepository::NamedTimerRepository(const string& file_name)
{
    m_fileName = file_name;
}

map<string, NamedTimer> NamedTimerRepository::ReadAll() const
{
    map<string, NamedTimer> items;

    const auto lines = FileUtils::ReadLines(m_fileName);
    for (auto& line : lines)
    {
        NamedTimer timer = NamedTimer::Create(line);

        items[timer.GetName()] = timer;
    }

    return items;
}
