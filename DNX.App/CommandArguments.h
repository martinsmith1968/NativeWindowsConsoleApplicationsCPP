#pragma once

#include "stdafx.h"
#include "Arguments.h"
#include "Command.h"

// ReSharper disable CppInconsistentNaming

namespace DNX::App
{
    //------------------------------------------------------------------------------
    // CommandArguments
    class CommandArguments final : public Arguments
    {
        static const string CommandNameLongName;
        static const string CommandNameDescription;

    public:
        CommandArguments() = default;

        void BuildFromCommands(list<Command> commands);
        string GetCommandNameValue();
    };
}
