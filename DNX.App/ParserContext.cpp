#include "stdafx.h"
#include "ParserContext.h"
#include "AppDetails.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;

const ParserContext ParserContext::_empty_context = ParserContext();

ParserContext::ParserContext()
    : ParserContext("")
{
}

ParserContext::ParserContext(const string& command_name)
{
    _command_name = command_name;
}

const string& ParserContext::GetCommandName() const
{
    return _command_name;
}

string ParserContext::GetDefaultArgumentsFileName() const
{
    if (_command_name.empty())
        return AppDetails::GetDefaultArgumentsFileName();
    return AppDetails::GetDefaultCommandArgumentsFileName(_command_name);
}

string ParserContext::GetLocalArgumentsFileName() const
{
    if (_command_name.empty())
        return AppDetails::GetLocalArgumentsFileName();
    return AppDetails::GetLocalCommandArgumentsFileName(_command_name);
}

const ParserContext& ParserContext::Empty() { return _empty_context; }
