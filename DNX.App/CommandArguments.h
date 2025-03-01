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
        const string CommandNameLongName = "command-name";

        const string CommandNameDescription = "The command to execute";

    public:
        CommandArguments() = default;

        void BuildFromCommands(list<Command> commands);
                string GetCommandNameValue();
    };
}
