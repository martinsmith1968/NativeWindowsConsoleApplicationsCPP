#pragma once

#include "AppArguments.h"
#include "../../libs/DNX.App/SimpleApp.h"
#include "../../libs/DNX.Utils/Crypto/Guid.h"

// ReSharper disable CppInconsistentNaming

namespace GuidGenerator
{
    class App : public SimpleApp<AppArguments>
    {
        static string FormatGuid(Crypto::Guid guid, const string& format);

    public:
        App(const int argc, char* argv[], const AppDetails& app_details, AppArguments& arguments, const ParserConfig& parser_config)
            : SimpleApp(argc, argv, app_details, arguments, parser_config)
        {
        }

        void Execute(AppArguments& arguments) override;
    };
}
