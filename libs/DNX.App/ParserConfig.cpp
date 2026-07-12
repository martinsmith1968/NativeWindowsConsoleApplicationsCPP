#include "stdafx.h"
#include "ParserConfig.h"
#include "WordWrappingHelpTextWriter.h"

#include <stdexcept>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppMemberFunctionMayBeConst

using namespace DNX::App;

const string ParserConfig::DefaultShortNamePrefix           = "-";
const string ParserConfig::DefaultAlternateShortNamePrefix  = "/";
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
        DefaultAlternateShortNamePrefix,
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
    const string& alternateShortNamePrefix,
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
    SetShortNamePrefix(shortNamePrefix);
    SetAlternateShortNamePrefix(alternateShortNamePrefix);
    SetLongNamePrefix(longNamePrefix);
    _customArgumentsFilePrefix = customArgumentsFilePrefix;
    _useDefaultArgumentsFile   = useDefaultArgumentsFile;
    _useLocalArgumentsFile     = useLocalArgumentsFile;
    _useCustomArgumentsFile    = useCustomArgumentsFile;
    _switchOnSuffix            = switchOnSuffix;
    _switchOffSuffix           = switchOffSuffix;
    _ignoreAdditionalArguments = ignoreAdditionalArguments;
}

void ParserConfig::SetShortNamePrefix(const string& shortNamePrefix)
{
    if (shortNamePrefix.length() != 1)
        throw invalid_argument("shortNamePrefix must be a single character string");

    _shortNamePrefix = shortNamePrefix;
}

void ParserConfig::SetAlternateShortNamePrefix(const string& alternateShortNamePrefix)
{
    if (alternateShortNamePrefix.length() > 1)
        throw invalid_argument("alternateShortNamePrefix must be a single character string, or empty");

    _alternateShortNamePrefix = alternateShortNamePrefix;
}

void ParserConfig::SetLongNamePrefix(const string& longNamePrefix)
{
    if (longNamePrefix.empty())
        throw invalid_argument("longNamePrefix must not be empty");

    _longNamePrefix = longNamePrefix;
}

void ParserConfig::SetCustomArgumentsFilePrefix(const string& customArgumentsFilePrefix)
{
    if (customArgumentsFilePrefix.length() > 1)
        throw invalid_argument("customArgumentsFilePrefix must be a single character string, or empty");

    _customArgumentsFilePrefix = customArgumentsFilePrefix;
}
