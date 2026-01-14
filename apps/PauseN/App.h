#pragma once

#include "AppArguments.h"
#include "../../libs/DNX.App/SimpleApp.h"

// ReSharper disable CppInconsistentNaming

namespace PauseN
{
    class App : public SimpleApp<AppArguments>
    {
    public:
        App(const int argc, char* argv[], const AppDetails& app_details, AppArguments& arguments, const ParserConfig& parser_config)
            : SimpleApp(argc, argv, app_details, arguments, parser_config)
        {
        }

        void Execute(AppArguments& arguments) override;
    };
}
