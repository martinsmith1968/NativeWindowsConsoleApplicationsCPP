#include "stdafx.h"

#include "App.h"
#include "AppInfo.h"
#include "AppCommands.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;
using namespace Stopwatch;

//------------------------------------------------------------------------------
int main(const int argc, char* argv[])
{
    const AppInfo app_info;
    const ParserConfig parser_config;
    AppCommands commands;

    auto app = App(app_info, commands, parser_config);
    return app.Run(argc, argv);
}
