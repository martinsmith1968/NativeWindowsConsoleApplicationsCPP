#pragma once

#include "AppCommands.h"
#include "../DNX.App/MultiCommandApp.h"

// ReSharper disable CppInconsistentNaming

namespace Stopwatch
{
    class App : public MultiCommandApp<AppCommands>
    {
    public:
        App(const AppDetails& app_details, AppCommands& commands, const ParserConfig& parser_config)
            : MultiCommandApp(app_details, commands, parser_config)
        {
        }
    };
}
