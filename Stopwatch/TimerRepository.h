#pragma once

#include "Timer.h"
#include <map>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace Stopwatch
{
    class TimerRepository
    {
        string m_fileName;

    public:
        TimerRepository();
        explicit TimerRepository(const string& filename);

        static string GetDefaultFileName();
        static string GetDefaultRepositoryFileName();

        [[nodiscard]] map<string, Timer> ReadAll() const;
        void SaveAll(const map<string, Timer>& timers);

        //void Save(const Timer& timer);
    };
}
