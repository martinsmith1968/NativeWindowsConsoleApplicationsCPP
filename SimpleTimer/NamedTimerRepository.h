#pragma once

#include "NamedTimer.h"
#include <map>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace SimpleTimer
{
    class NamedTimerRepository
    {
        string m_fileName;

    public:
        explicit NamedTimerRepository(const string& file_name);

        map<string, NamedTimer> ReadAll() const;
    };
}
