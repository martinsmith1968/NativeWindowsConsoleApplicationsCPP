#include "stdafx.h"
#include "TimerRepository.h"
#include "AppInfo.h"
#include "Timer.h"
#include "../../libs/DNX.Utils/DirectoryUtils.h"
#include "../../libs/DNX.Utils/FileUtils.h"
#include "../../libs/DNX.Utils/MapUtils.h"
#include "../../libs/DNX.Utils/PathUtils.h"
#include "../../libs/DNX.Utils/StringUtils.h"

using namespace std;
using namespace Stopwatch;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppMemberFunctionMayBeConst
// ReSharper disable CppTooWideScopeInitStatement

TimerRepository::TimerRepository()
{
    m_fileName = GetDefaultFileName();
}

TimerRepository::TimerRepository(const string& filename)
{
    m_fileName = filename;
}

string TimerRepository::GetDefaultFileName()
{
    return string(VER_PRODUCTNAME_STR)
        .append(".data");
}

string TimerRepository::GetDefaultRepositoryFileName()
{
    string file_path = PathUtils::Combine(DirectoryUtils::GetUserDataDirectory(), COMPANY_DATA_FOLDER_NAME);
    file_path = PathUtils::Combine(file_path, VER_PRODUCTNAME_STR);
    file_path = PathUtils::Combine(file_path, GetDefaultFileName());
    return file_path;
}

string TimerRepository::GetFileName() const
{
    return m_fileName;
}

map<string, Timer> TimerRepository::ReadAll() const
{
    map<string, Timer> items;

    const auto lines = FileUtils::ReadAllLines(m_fileName);
    for (auto& line : lines)
    {
        Timer timer = Timer::Create(line);

        items[timer.GetName()] = timer;
    }

    return items;
}

void TimerRepository::SaveAll(const map<string, Timer>& timers)
{
    list<string> lines;

    for (const auto& [key, value] : timers)
    {
        auto line = value.ToDefinition();
        lines.emplace_back(line);
    }

    const auto file_path = StringUtils::EnsureEndsWith(PathUtils::GetDriveAndPath(m_fileName), PathUtils::PATH_SEPARATOR);
    if (!file_path.empty())
        DirectoryUtils::Create(file_path);

    FileUtils::WriteAllLines(m_fileName, lines);
}

bool TimerRepository::Exists(const string& stopwatch_name) const
{
    const auto timers = ReadAll();

    return MapUtils::Exists(timers, stopwatch_name);
}

Timer TimerRepository::GetByName(const string& stopwatch_name) const
{
    auto timers = ReadAll();

    if (!MapUtils::Exists(timers, stopwatch_name))
        return Timer::Empty();

    auto& timer = timers.at(stopwatch_name);

    return timer;
}

void TimerRepository::Delete(const Timer& timer)
{
    auto items = ReadAll();
    if (!MapUtils::Exists(items, timer.GetName()))
    {
        throw exception("Timer not found");
    }

    items.erase(timer.GetName());
    SaveAll(items);
}

void TimerRepository::Add(Timer& timer)
{
    auto items = ReadAll();
    if (MapUtils::Exists(items, timer.GetName()))
    {
        throw exception("Timer already exists");
    }

    items.emplace(timer.GetName(), timer);
    SaveAll(items);
}

void TimerRepository::Update(Timer& timer)
{
    auto items = ReadAll();
    if (!MapUtils::Exists(items, timer.GetName()))
    {
        throw exception("Timer not found");
    }
    items.erase(timer.GetName());
    items.emplace(timer.GetName(), timer);
    SaveAll(items);
}
