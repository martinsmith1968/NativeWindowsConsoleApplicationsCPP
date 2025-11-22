#pragma once

#include "AppDetails.h"
#include "AppError.h"
#include "ArgumentsParser.h"
#include "ArgumentsUsageDisplay.h"
#include "ParserConfig.h"
#include "../DNX.App/CommandsParser.h"
#include "../DNX.App/CommandsUsageDisplay.h"
#include <iostream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticPadded
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyModernizePassByValue
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppClangTidyPortabilityTemplateVirtualMemberFunction

namespace DNX::App
{
    //------------------------------------------------------------------------------
    // TODO:
    // - Organise code structure
    // - Investigate Execute() method on Commands (base class ?)

    //--------------------------------------------------------------------------
    // Class: MultiCommandApp
    //--------------------------------------------------------------------------
    template <typename TCommands>
    class MultiCommandApp
    {
    protected:
        const AppDetails& _app_details;
        TCommands& _app_commands;
        const ParserConfig _parser_config;

        AppError _app_error = AppError::Empty();

        [[nodiscard]] bool HasError() const
        {
            return !_app_error.IsEmpty();
        }

        void SetError(const string& description, const int return_code = 0)
        {
            SetError(AppError(description, return_code));
        }

        void SetError(const AppError& app_error)
        {
            _app_error = app_error;
        }

    public:
        virtual ~MultiCommandApp() = default;

        MultiCommandApp(const AppDetails& app_details, TCommands& app_commands, const ParserConfig& parser_config)
            : _app_details(app_details)
            , _app_commands(app_commands)
            , _parser_config(parser_config)
        {
        }

        virtual int Run(int argc, char* argv[])
        {
            try
            {
                auto& command = CommandsParser::ParseCommands(_app_commands, argc, argv);

                if (command.IsEmpty())
                {

                    if (_app_commands.IsHelp())
                    {
                        CommandsUsageDisplay::ShowUsage(_app_commands, _parser_config, _app_details);
                        return 1;
                    }
                    if (_app_commands.IsVersion())
                    {
                        ArgumentsUsageDisplay::ShowVersion(_app_details);
                        SetError("", 2);
                        return 2;
                    }

                    CommandsUsageDisplay::ShowUsage(_app_commands, _parser_config, _app_details);
                    return 1;
                }

                if (!_app_commands.IsValid())
                {
                    ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), _parser_config, _app_details);
                    ArgumentsUsageDisplay::ShowErrors(command.GetArguments(), 1);
                    return 2;
                }

                if (command.GetArguments().IsHelp())
                {
                    ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), _parser_config, _app_details, command.GetName());
                    return 3;
                }
                if (command.GetArguments().IsVersion())
                {
                    ArgumentsUsageDisplay::ShowVersion(_app_details);
                    SetError("", 2);
                    return 4;
                }

                if (!command.GetArguments().IsValid())
                {
                    ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), _parser_config, _app_details, command.GetName());
                    ArgumentsUsageDisplay::ShowErrors(command.GetArguments(), 1);
                    return 5;
                }

                _app_commands.Execute(command);

                return 0;
            }
            catch (exception& ex)
            {
                cerr << ArgumentsUsageDisplay::ErrorLinePrefix << ": " << ex.what() << endl;
                return 99;
            }
            catch (...)
            {
                cerr << ArgumentsUsageDisplay::ErrorLinePrefix << ": Unknown error occurred" << endl;
                return 98;
            }
        }
    };
}
