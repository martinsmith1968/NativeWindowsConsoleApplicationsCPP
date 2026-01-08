#include "stdafx.h"
#include "App.h"
#include "AppArguments.h"
#include "AppInfo.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;
using namespace ShowDateTime;

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
