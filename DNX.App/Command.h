#pragma once
#include "stdafx.h"
#include "Arguments.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions

using namespace std;
using namespace DNX::App;

namespace DNX::App
{
    class EmptyCommand;

    //--------------------------------------------------------------------------
    // Class: Command
    //--------------------------------------------------------------------------
    class Command
    {
        Arguments* m_arguments;
        string m_name;
        string m_description;
        int m_sequence;

    public:
        Command();
        virtual ~Command() = default;

        static bool CompareBySequence(const Command& first, const Command& second);

        [[nodiscard]] bool IsEmpty() const;

        [[nodiscard]] Arguments& GetArguments() const;
        [[nodiscard]] string GetName() const;
        [[nodiscard]] string GetDescription() const;
        [[nodiscard]] int GetSequence() const;

        Command(
            Arguments* arguments,
            const string& name,
            const string& description,
            int sequence = 0
        );

        static EmptyCommand _empty_command;
        static EmptyCommand& Empty();

        virtual void Execute();
    };

    class EmptyCommand final : public Command
    {
    public:
        EmptyCommand() = default;

        void Execute() override
        { }
    };
}
