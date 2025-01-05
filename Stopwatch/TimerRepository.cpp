#include "stdafx.h"
#include "AppInfo.h"
#include "TimerRepository.h"
#include "Timer.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace Stopwatch;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppMemberFunctionMayBeConst

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
    string file_path = PathUtils::Combine(PathUtils::GetUserDataDirectory(), COMPANY_DATA_FOLDER_NAME);
    file_path = PathUtils::Combine(file_path, VER_PRODUCTNAME_STR);
    file_path = PathUtils::Combine(file_path, GetDefaultFileName());
    return file_path;
}

map<string, Timer> TimerRepository::ReadAll() const
{
    map<string, Timer> items;

    const auto lines = FileUtils::ReadLines(m_fileName);
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

    const auto file_path = StringUtils::EnsureEndsWith(FileUtils::GetPath(m_fileName), PathUtils::PATH_SEPARATOR);
    if (!file_path.empty())
        PathUtils::CreateDirectory(file_path);

    FileUtils::WriteLines(m_fileName, lines);
}

//void TimerRepository::Save(const Timer& timer)
//{
//    const auto file_path = StringUtils::EnsureEndsWith(FileUtils::GetPath(m_fileName), PathUtils::PATH_SEPARATOR);
//    if (!file_path.empty())
//        PathUtils::CreateDirectory(file_path);
//
//    auto timers = ReadAll();
//    timers.emplace(timer.GetName(), timer);
//
//    SaveAll(timers);
//}
