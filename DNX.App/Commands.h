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

        CommandArguments& GetArguments();

        void AddCommand(const Command& command);

        list<Command> GetCommands() const;
        Command& GetCommandByName(const string& commandName);

        list<string> GetErrors() const;
        bool IsValid() const;
    };
}
