#include "stdafx.h"
#include "ParserConfig.h"
#include "WordWrappingHelpTextWriter.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppMemberFunctionMayBeConst

using namespace DNX::App;

const string ParserConfig::DefaultShortNamePrefix           = "-";
const string ParserConfig::DefaultLongNamePrefix            = "--";
const string ParserConfig::DefaultCustomArgumentsFilePrefix = "@";

HelpTextWriter* ParserConfig::GetHelpTextWriter() const
{
    return _helpTextWriter;
}

void ParserConfig::SetHelpTextWriter(HelpTextWriter* help_text_writer)
{
    _helpTextWriter = help_text_writer;
}

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
    : _helpTextWriter(&WordWrappingHelpTextWriter::GetDefaultInstance())
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
