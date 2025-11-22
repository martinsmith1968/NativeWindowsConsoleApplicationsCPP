#pragma once

#include "AppDetails.h"
#include "CommandArguments.h"
#include "Commands.h"
#include "ParserConfig.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: CommandsParser
    //--------------------------------------------------------------------------
    class CommandsParser
    {
        Commands& _commands;
        const ParserConfig _parser_config;
        const AppDetails _app_details;

    public:
        explicit CommandsParser(Commands& commands, const AppDetails& app_details = AppDetails(), const ParserConfig& parser_config = ParserConfig());
        Command& Parse(int argc, char* argv[]) const;

        static list<string> RefineCommandArgumentValues(const list<string>& argument_values, const string& commandName);

        static Command& ParseCommands(Commands& commands, int argc, char* argv[], const AppDetails& app_details = AppDetails(), const ParserConfig& parser_config = ParserConfig());
    };
}
