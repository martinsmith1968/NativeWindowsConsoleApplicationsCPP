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
        Arguments* _arguments;
        string _name;
        string _description;

    public:
        Command();
        virtual ~Command() = default;

        [[nodiscard]] bool IsEmpty() const;

        [[nodiscard]] Arguments& GetArguments() const;
        [[nodiscard]] string GetName() const;
        [[nodiscard]] string GetDescription() const;

        Command(
            Arguments* arguments,
            const string& name,
            const string& description
        );

        static EmptyCommand _empty_command;
        static EmptyCommand& Empty() { return _empty_command; }

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
