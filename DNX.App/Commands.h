#pragma once

#include "stdafx.h"
#include "Command.h"
#include "CommandsParser.h"
#include <map>
#include <list>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyClangDiagnosticPadded

using namespace std;
using namespace DNX::Utils;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: Commands
    //--------------------------------------------------------------------------
    class Commands
    {
        map<string, Command> _commands{};
        CommandArguments _arguments;

    public:
        Commands() = default;
        virtual ~Commands() = default;

        CommandArguments& GetArguments();
        virtual void InitialiseCommandArguments();
        void AppendCommandsToCommandArguments();

        void AddCommand(const Command& command);

        [[nodiscard]] list<Command> GetCommands() const;
        Command& GetCommandByName(const string& commandName);

        [[nodiscard]] list<string> GetErrors() const;
        [[nodiscard]] bool IsValid() const;

        virtual void Execute(const Command& command) = 0;

        bool IsHelp() { return _arguments.IsHelp();  }
        bool IsVersion() { return _arguments.IsVersion(); }
    };
}
