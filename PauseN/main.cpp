#include "stdafx.h"
#include "App.h"
#include "AppInfo.h"
#include "AppArguments.h"
#include "../DNX.Utils/StringUtils.h"
#include "../DNX.App/SimpleApp.h"
#include "../DNX.App/ArgumentsParser.h"
#include <regex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;
using namespace PauseN;

//------------------------------------------------------------------------------
// main
int main(const int argc, char* argv[])
{
    AppInfo appInfo;
    AppArguments arguments;
    ParserConfig parser_config;

    auto app = App(argc, argv, appInfo, arguments, parser_config);
    return app.Run();
}
