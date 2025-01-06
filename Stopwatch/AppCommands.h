#pragma once
#include "stdafx.h"
#include "../DNX.App/Commands.h"
#include "Commands/ElapsedCommand.h"
#include "Commands/ListCommand.h"
#include "Commands/StartCommand.h"
#include "Commands/StopCommand.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace std;
using namespace DNX::Utils;

namespace Stopwatch
{
    //------------------------------------------------------------------------------
    // Commands
    class AppCommands final : public Commands
    {
        ListArguments  _list_arguments;

        ListCommand m_list_command;
        StartCommand m_start_command;
        StopCommand m_stop_command;
        ElapsedCommand m_elapsed_command;

    public:
        AppCommands()
        {
            const CommandTypeTextResolver commandTypeTextResolver;

            AddCommand(m_list_command);
            AddCommand(m_start_command);
            AddCommand(m_stop_command);
            AddCommand(m_elapsed_command);

            //AddCommand(&_list_arguments, commandTypeTextResolver.GetText(CommandType::LIST), "List the current active Stopwatches");
            //AddCommand(&_start_arguments, commandTypeTextResolver.GetText(CommandType::START), "Start a named Stopwatch");
            //AddCommand(&_stop_arguments, commandTypeTextResolver.GetText(CommandType::STOP), "Stop a named Stopwatch");
            //AddCommand(AppArgumentsStop(),    "stop",    "Stop an active Timer");
            //AddCommand(AppArgumentsCancel(),  "cancel",  "Cancel an active Timer");
            //AddCommand(AppArgumentsPause(),   "pause",   "Pause an active Timer");
            //AddCommand(AppArgumentsElapsed(), "elapsed", "Get the elapsed time of an active timer");
        }

        void Execute(const Command& command)
        {
            if (command.GetName() == m_list_command.GetName())
            {
                m_list_command.Execute();
            }
            else if (command.GetName() == m_start_command.GetName())
            {
                m_start_command.Execute();
            }
            else if (command.GetName() == m_stop_command.GetName())
            {
                m_stop_command.Execute();
            }
            else if (command.GetName() == m_elapsed_command.GetName())
            {
                m_elapsed_command.Execute();
            }
            else
            {
                const string message = "Invalid or Unsupported Command Target: " + command.GetName();
                throw exception(message.c_str());
            }
        }
    };
}
