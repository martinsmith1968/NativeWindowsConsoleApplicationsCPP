#include "stdafx.h"
#include "App.h"
#include "AppInfo.h"
#include "AppArguments.h"
#include "../DNX.Utils/StringUtils.h"

#include <regex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppTooWideScope

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
