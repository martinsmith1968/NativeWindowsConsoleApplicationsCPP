#include "stdafx.h"
#include "App.h"
#include "AppInfo.h"
#include "AppArguments.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;
using namespace BannerText;

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
