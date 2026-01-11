#pragma once

#include "../stdafx.h"
#include "../../DNX.Utils/EnumUtils.h"
#include <cstdint>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Utils::EnumUtils;

namespace Stopwatch
{
    enum class CommandType : uint8_t
    {
        LIST,
        START,
        STOP,
        PAUSE,
        RESUME,
        ELAPSED,
        CANCEL,
        PURGE,
    };

    class CommandTypeTextResolver : public EnumTextResolver<CommandType>
    {
    public:
        CommandTypeTextResolver()
        {
            SetText(CommandType::LIST, "List");
            SetText(CommandType::START, "Start");
            SetText(CommandType::STOP, "Stop");
            SetText(CommandType::PAUSE, "Pause");
            SetText(CommandType::RESUME, "Resume");
            SetText(CommandType::ELAPSED, "Elapsed");
            SetText(CommandType::CANCEL, "Cancel");
            SetText(CommandType::PURGE, "Purge");
        }
    };
}
