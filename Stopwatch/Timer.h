#pragma once

#include "../DNX.Utils/EnumUtils.h"
#include <string>
#include <ctime>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Utils::EnumUtils;

namespace Stopwatch
{
    enum class TimerStateType : uint8_t
    {
        ACTIVE,
        INACTIVE,
    };

    class TimerStateTypeTextResolver : public EnumTextResolver<TimerStateType>
    {
    public:
        TimerStateTypeTextResolver()
        {
            SetText(TimerStateType::ACTIVE, "Active");
            SetText(TimerStateType::INACTIVE, "Inactive");
        }
    };

    class Timer
    {
        string m_Name;
        time_t m_Start;
        TimerStateType m_State;
        double m_TotalElapsed;

        static Timer m_empty_timer;

    public:
        Timer();
        explicit Timer(const string& name);

        [[nodiscard]] string GetName() const;
        [[nodiscard]] time_t GetStart() const;
        [[nodiscard]] TimerStateType GetState() const;
        [[nodiscard]] double GetTotalElapsed() const;

        [[nodiscard]] double GetCurrentElapsed() const;
        [[nodiscard]] double GetAccumulatedElapsed() const;
        [[nodiscard]] tm GetStartDateTime() const;

        [[nodiscard]] bool CanStart() const;
        [[nodiscard]] bool CanStop() const;

        void Start();
        void Pause();
        void Stop();

        [[nodiscard]] bool IsEmpty() const;

        static Timer Create(const string& definition);

        [[nodiscard]] string ToDefinition() const;
        void FromDefinition(const string& definition);

        static Timer& Empty() { return m_empty_timer; }

        static bool CompareByStartTime(const Timer& first, const Timer& second);
    };
}
