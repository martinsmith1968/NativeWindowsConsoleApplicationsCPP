#pragma once

#include "AppDetails.h"
#include "AppError.h"
#include "ArgumentsParser.h"
#include "ArgumentsUsageDisplay.h"
#include "ParserConfig.h"

#include <iostream>
#include <sstream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticPadded
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyModernizePassByValue
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppClangTidyPortabilityTemplateVirtualMemberFunction

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: SimpleApp
    //--------------------------------------------------------------------------
    template <typename TArguments>
    class SimpleApp
    {
    protected:
        const AppDetails& _app_details;
        TArguments& _app_arguments;
        const ParserConfig _parser_config;

        virtual void Execute(TArguments& arguments) = 0;

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
        virtual ~SimpleApp() = default;

        SimpleApp(int argc, char* argv[], const AppDetails& app_details, TArguments& app_arguments, const ParserConfig& parser_config)
            : _app_details(app_details)
            , _app_arguments(app_arguments)
            , _parser_config(parser_config)
        {
            try
            {
                ArgumentsParser::ParseArguments(_app_arguments, argc, argv);

                if (_app_arguments.IsHelp())
                {
                    ArgumentsUsageDisplay::ShowUsage(_app_arguments, _parser_config, _app_details);
                    SetError("", 1);
                    return;
                }
                if (_app_arguments.IsVersion())
                {
                    ArgumentsUsageDisplay::ShowVersion(_app_details);
                    SetError("", 2);
                    return;
                }
                if (!_app_arguments.IsValid())
                {
                    ArgumentsUsageDisplay::ShowUsage(_app_arguments, _parser_config, _app_details);
                    ArgumentsUsageDisplay::ShowErrors(_app_arguments, 1);
                    SetError("", 3);
                    return;
                }
            }
            catch (exception& ex)
            {
                stringstream text;
                text << ArgumentsUsageDisplay::ErrorLinePrefix << ": " << ex.what();
                SetError(text.str(), 98);
                return;
            }
            catch (...)
            {
                stringstream text;
                text << ArgumentsUsageDisplay::ErrorLinePrefix << ": Unknown error occurred" << endl;
                SetError(text.str(), 99);
                return;
            }
        }

        int Run()
        {
            try
            {
                if (!HasError())
                {
                    Execute(_app_arguments);
                }
            }
            catch (exception& ex)
            {
                stringstream text;
                text << ArgumentsUsageDisplay::ErrorLinePrefix << ": " << ex.what();
                SetError(text.str(), 98);
            }
            catch (...)
            {
                stringstream text;
                text << ArgumentsUsageDisplay::ErrorLinePrefix << ": Unknown error occurred" << endl;
                SetError(text.str(), 99);
            }

            if (HasError())
            {
                if (_app_error.HasDescription())
                    cerr << _app_error.GetDescription() << endl;
                return _app_error.GetReturnCode();
            }

            return 0;
        }
    };
}
