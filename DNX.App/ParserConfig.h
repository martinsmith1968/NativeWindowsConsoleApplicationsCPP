#pragma once

#include "stdafx.h"
#include <string>

#include "HelpTextWriter.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyClangDiagnosticPadded

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
        static constexpr bool DefaultUseDefaultArgumentsFile = true;
        static constexpr bool DefaultUseLocalArgumentsFile = true;
        static constexpr bool DefaultUseCustomArgumentsFiles = true;
        static constexpr char DefaultSwitchOnSuffix = '+';
        static constexpr char DefaultSwitchOffSuffix = '-';
        static constexpr bool DefaultIgnoreAdditionalArguments = false;

        string _shortNamePrefix;
        string _longNamePrefix;
        string _customArgumentsFilePrefix;
        bool _useDefaultArgumentsFile;
        bool _useLocalArgumentsFile;
        bool _useCustomArgumentsFile;
        char _switchOnSuffix;
        char _switchOffSuffix;
        bool _ignoreAdditionalArguments;
        HelpTextWriter* _helpTextWriter;

    public:
        [[nodiscard]] const string& GetShortNamePrefix() const { return _shortNamePrefix; }
        [[nodiscard]] const string& GetLongNamePrefix() const { return _longNamePrefix; }
        [[nodiscard]] const string& GetCustomArgumentsFilePrefix() const { return _customArgumentsFilePrefix; }
        [[nodiscard]] bool GetUseDefaultArgumentsFile() const { return _useDefaultArgumentsFile; }
        [[nodiscard]] bool GetUseLocalArgumentsFile() const { return _useLocalArgumentsFile; }
        [[nodiscard]] bool GetUseCustomArgumentsFile() const { return _useCustomArgumentsFile; }
        [[nodiscard]] char GetSwitchOnSuffix() const { return _switchOnSuffix; }
        [[nodiscard]] char GetSwitchOffSuffix() const { return _switchOffSuffix; }
        [[nodiscard]] bool GetIgnoreAdditionalArguments() const { return _ignoreAdditionalArguments; }

        void SetIgnoreAdditionalArguments(const bool value) { _ignoreAdditionalArguments = value; }

        [[nodiscard]] HelpTextWriter* GetHelpTextWriter() const;
        void SetHelpTextWriter(HelpTextWriter* help_text_writer);

        ParserConfig();

        ParserConfig(
            const string& shortNamePrefix,
            const string& longNamePrefix,
            const string& customArgumentsFilePrefix,
            bool useDefaultArgumentsFile,
            bool useLocalArgumentsFile,
            bool useCustomArgumentsFile,
            char switchOnSuffix,
            char switchOffSuffix,
            bool ignoreAdditionalArguments
        );
    };
}
