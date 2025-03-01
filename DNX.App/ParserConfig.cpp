#include "stdafx.h"
#include "ParserConfig.h"

// ReSharper disable CppInconsistentNaming

using namespace DNX::App;

const string ParserConfig::DefaultShortNamePrefix           = "-";
const string ParserConfig::DefaultLongNamePrefix            = "--";
const string ParserConfig::DefaultCustomArgumentsFilePrefix = "@";

ParserConfig::ParserConfig()
    : ParserConfig(
        DefaultShortNamePrefix,
        DefaultLongNamePrefix,
        DefaultCustomArgumentsFilePrefix,
        DefaultUseDefaultArgumentsFile,
        DefaultUseLocalArgumentsFile,
        DefaultUseCustomArgumentsFiles,
        DefaultSwitchOnSuffix,
        DefaultSwitchOffSuffix,
        DefaultIgnoreAdditionalArguments
    )
{
}

ParserConfig::ParserConfig(
    const string& shortNamePrefix,
    const string& longNamePrefix,
    const string& customArgumentsFilePrefix,
    const bool useDefaultArgumentsFile,
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
    _useDefaultArgumentsFile   = useDefaultArgumentsFile;
    _useLocalArgumentsFile     = useLocalArgumentsFile;
    _useCustomArgumentsFile    = useCustomArgumentsFile;
    _switchOnSuffix            = switchOnSuffix;
    _switchOffSuffix           = switchOffSuffix;
    _ignoreAdditionalArguments = ignoreAdditionalArguments;
}
