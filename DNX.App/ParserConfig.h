#pragma once
#include "stdafx.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: ParserConfig
    //--------------------------------------------------------------------------
    class ParserConfig {
        static const string DefaultShortNamePrefix;
        static const string DefaultLongNamePrefix;
        static const string DefaultCustomArgumentsFilePrefix;
        static constexpr bool DefaultUseGlobalArgumentsFile    = true;
        static constexpr bool DefaultUseLocalArgumentsFile     = true;
        static constexpr bool DefaultUseCustomArgumentsFiles   = true;
        static constexpr char DefaultSwitchOnSuffix            = '+';
        static constexpr char DefaultSwitchOffSuffix           = '-';
        static constexpr bool DefaultIgnoreAdditionalArguments = false;

        string _shortNamePrefix;
        string _longNamePrefix;
        string _customArgumentsFilePrefix;
        bool _useGlobalArgumentsFile;
        bool _useLocalArgumentsFile;
        bool _useCustomArgumentsFile;
        char _switchOnSuffix;
        char _switchOffSuffix;
        bool _ignoreAdditionalArguments;

    public:
        [[nodiscard]] const string& GetShortNamePrefix() const { return _shortNamePrefix; }
        [[nodiscard]] const string& GetLongNamePrefix() const { return _longNamePrefix; }
        [[nodiscard]] const string& GetCustomArgumentsFilePrefix() const { return _customArgumentsFilePrefix; }
        [[nodiscard]] bool GetUseGlobalArgumentsFile() const { return _useGlobalArgumentsFile; }
        [[nodiscard]] bool GetUseLocalArgumentsFile() const { return _useLocalArgumentsFile; }
        [[nodiscard]] bool GetUseCustomArgumentsFile() const { return _useCustomArgumentsFile; }
        [[nodiscard]] char GetSwitchOnSuffix() const { return _switchOnSuffix; }
        [[nodiscard]] char GetSwitchOffSuffix() const { return _switchOffSuffix; }
        [[nodiscard]] bool GetIgnoreAdditionalArguments() const { return _ignoreAdditionalArguments; }

        void SetIgnoreAdditionalArguments(const bool value) {_ignoreAdditionalArguments = value; }

        ParserConfig()
            : ParserConfig(
                DefaultShortNamePrefix,
                DefaultLongNamePrefix,
                DefaultCustomArgumentsFilePrefix,
                DefaultUseGlobalArgumentsFile,
                DefaultUseLocalArgumentsFile,
                DefaultUseCustomArgumentsFiles,
                DefaultSwitchOnSuffix,
                DefaultSwitchOffSuffix,
                DefaultIgnoreAdditionalArguments
            )
        {
        }

        ParserConfig(
            const string& shortNamePrefix,
            const string& longNamePrefix,
            const string& customArgumentsFilePrefix,
            const bool useGlobalArgumentsFile,
            const bool useLocalArgumentsFile,
            const bool useCustomArgumentsFile,
            const char switchOnSuffix,
            const char switchOffSuffix,
            const bool ignoreAdditionalArguments
        )
        {
            _shortNamePrefix           = shortNamePrefix;
            _longNamePrefix            = longNamePrefix;
            _customArgumentsFilePrefix = customArgumentsFilePrefix;
            _useGlobalArgumentsFile    = useGlobalArgumentsFile;
            _useLocalArgumentsFile     = useLocalArgumentsFile;
            _useCustomArgumentsFile    = useCustomArgumentsFile;
            _switchOnSuffix            = switchOnSuffix;
            _switchOffSuffix           = switchOffSuffix;
            _ignoreAdditionalArguments = ignoreAdditionalArguments;
        }
    };
}
