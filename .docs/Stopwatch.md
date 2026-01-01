# Stopwatch

## Overview

Linux has a command called `time` for timing how long commands take and displaying the output. It takes the command
to be excuted as arguments and executes it itself in-process, then displays how long it took.

I wanted to time a variety of things, executable commands, batch files, scripts of various languages, and Windows doesn't
really have a nice consistent process interface for executing process (ShellExecute or not anyone?), and so to replicate
that exact functionality would have required more options and complexity than I wanted to get involved with.

So I came up with Stopwatch, which is an out-of-process way of timing arbitrary things.

The basic flow would be :

 - Call `stopwatch` to start a named timer
 - Do whatever you wanted
 - Call `stopwatch` to stop the named timer and report elapsed time

 E.g.

 ```cmd
 stopwatch start myprocess
 myprocess
 stopwatch stop myprocess
 ```

 And you'd see something along the lines of :

 ```text
 Stopwatch: myprocess started at 18:27:10
 ...
 ... output from 'myprocess'
 ...
 myprocess: Stopped - 32 seconds
 ```

 The Stopwatch name is just an identifier and there aren't any current restrictions on what you can use there -
 just whatever makes sense for your output.

 E.g.
 ```cmd
stopwatch start my-long-running-job
jobpart1
stopwatch elapsed my-long-running-job -at "Part 1 complete"
jobpart2
stopwatch elapsed my-long-running-job -at "Part 2 complete"
jobpart3
stopwatch stop my-long-running-job
```

Might show something like :

```text
my-long-running-job started at 18:14:13
...
my-long-running-job: Active - 28 seconds - Part 1 complete
...
my-long-running-job: Active - 50 seconds - Part 2 complete
...
my-long-running-job: Inactive - 1 minutes, 4 seconds
```

You can Cancel stopwatches (Stop without outputting elapsed time), Pause and Resume them too, as well as just
Show the current Elapsed time without interrupting it at all.

I use this in a number of batch scripts to track how long various parts take.

## Dev Notes

This uses a file on filesystem in the users `AppData` folder (by default).

It uses a simple delimited format and just stores :

- Name
- Start Time (Seconds since Epoch time)
- State (Running / Paused)
- Accumulated Elapsed Time (to support multiple pausing / resuming)

And looks something like :

```csv
timer1|1736190111|0|0.000000
timer2|1736190115|0|0.000000
```

## Latest Version

[//]: # (APP_LATESTVERSION)

> Stopwatch -!

```text
v2.0.0.0-dev
```

## Full Help Text

The full help text looks something like this :

[//]: # (APP_HELPOUTPUT)

> Stopwatch -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch [command-name] { [command-arguments] }

Commands:

List     List all active Stopwatches
Start    Start a named Stopwatch
Stop     Start an active Stopwatch
Cancel   Cancel an active Stopwatch
Elapsed  Display the current elapsed time of an active Stopwatch
Pause    Pause an active Stopwatch
Purge    Purge old Stopwatches
Resume   Resume a paused Stopwatch

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.options
Local App arguments can be specified in : C:\Temp\Stopwatch.options
```

### List

Sample Output :

```text
timer1: Running - 3 days, 1 hours, 55 minutes, 51 seconds
timer2: Running - 3 days, 1 hours, 39 minutes, 11 seconds

Found: 2 stopwatches
```

[//]: # (CMD_HELPOUTPUT List)

> Stopwatch List -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch List [OPTIONS]

Options:
-o, --output-format Text              Control output format of list (Default:Display, Values: Display, CSV, Custom)
-fmt, --custom-format-text Text       A custom format string for the Timer details
-s, --sort-by Text                    The field to order the Timers by (Default:Name, Values: Name, Start, State, Elapsed)
-r, --sort-reverse +/-                Sort in reverse order (Default:false)
-v, --verbose-output +/-              Control verbosity of output messages (Default:false)
-df, --data-filename Text             The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-!, --version +/-                     Show App Version details (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (Stopwatch.list.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (Stopwatch.list.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.list.options
Local App arguments can be specified in : C:\Temp\Stopwatch.list.options
```

### Start

Sample Output :

```text
> stopwatch start timer1
timer1 started at 12:20:59
```

[//]: # (CMD_HELPOUTPUT Start)

> Stopwatch Start -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch Start [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                 The name of the Stopwatch (Required)
-f, --force +/-                       Force starting even if already exists (Default:false)
-q, --quiet +/-                       Suppress output messages (Default:false)
-v, --verbose-output +/-              Control verbosity of output messages (Default:false)
-df, --data-filename Text             The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-!, --version +/-                     Show App Version details (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (Stopwatch.start.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (Stopwatch.start.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.start.options
Local App arguments can be specified in : C:\Temp\Stopwatch.start.options
```

### Stop

Sample Output :

```text
> stopwatch stop timer1
timer1: Stopped - 2 minutes, 40 seconds
```

[//]: # (CMD_HELPOUTPUT Stop)

> Stopwatch Stop -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch Stop [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                       The name of the Stopwatch (Required)
-etadf, --elapsed-time-display-format Text  The format string to use to display Elapsed Time (Default:{name}: {action} - {days} days, {hours} hours, {minutes} minutes, {seconds} seconds)
-set, --show-elapsed-time +/-               Show the Stopwatch Elapsed Time (Default:true)
-v, --verbose-output +/-                    Control verbosity of output messages (Default:false)
-df, --data-filename Text                   The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-!, --version +/-                           Show App Version details (Default:false)
-?, --help +/-                              Show Help screen (Default:false)
-@, --use-default-arguments-file +/-        Use Default Arguments File (Stopwatch.stop.options) (Default:true)
-$, --use-local-arguments-file +/-          Use Local Arguments File (Stopwatch.stop.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.stop.options
Local App arguments can be specified in : C:\Temp\Stopwatch.stop.options
```

### Elapsed

Sample Output :

```text
> Stopwatch.exe elapsed timer2
timer2: Running - 5 minutes, 46 seconds
```

[//]: # (CMD_HELPOUTPUT Elapsed)

> Stopwatch Elapsed -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch Elapsed [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                      The name of the Stopwatch (Required)
-etdf, --elapsed-time-display-format Text  The format string to use to display Elapsed Time (Default:{name}: {state} - {days} days, {hours} hours, {minutes} minutes, {seconds} seconds)
-at, --additional-text Text                Additional text for the output
-df, --data-filename Text                  The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-!, --version +/-                          Show App Version details (Default:false)
-?, --help +/-                             Show Help screen (Default:false)
-@, --use-default-arguments-file +/-       Use Default Arguments File (Stopwatch.elapsed.options) (Default:true)
-$, --use-local-arguments-file +/-         Use Local Arguments File (Stopwatch.elapsed.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.elapsed.options
Local App arguments can be specified in : C:\Temp\Stopwatch.elapsed.options
```

### Cancel

Sample Output :

```text
> stopwatch cancel timer1
```

[//]: # (CMD_HELPOUTPUT Cancel)

> Stopwatch Cancel -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch Cancel [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                 The name of the Stopwatch (Required)
-v, --verbose-output +/-              Control verbosity of output messages (Default:false)
-df, --data-filename Text             The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-!, --version +/-                     Show App Version details (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (Stopwatch.cancel.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (Stopwatch.cancel.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.cancel.options
Local App arguments can be specified in : C:\Temp\Stopwatch.cancel.options
```

### Pause

Sample Output :

```text
> stopwatch pause timer1
```

[//]: # (CMD_HELPOUTPUT Pause)

> Stopwatch Pause -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch Pause [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                       The name of the Stopwatch (Required)
-etadf, --elapsed-time-display-format Text  The format string to use to display Elapsed Time (Default:{name}: {action} - {days} days, {hours} hours, {minutes} minutes, {seconds} seconds)
-set, --show-elapsed-time +/-               Show the Stopwatch Elapsed Time (Default:true)
-i, --ignore-invalid-state +/-              Ignore errors of Stopwatch being in invalid state for the action (Default:false)
-v, --verbose-output +/-                    Control verbosity of output messages (Default:false)
-at, --additional-text Text                 Additional text for the output
-df, --data-filename Text                   The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-!, --version +/-                           Show App Version details (Default:false)
-?, --help +/-                              Show Help screen (Default:false)
-@, --use-default-arguments-file +/-        Use Default Arguments File (Stopwatch.pause.options) (Default:true)
-$, --use-local-arguments-file +/-          Use Local Arguments File (Stopwatch.pause.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.pause.options
Local App arguments can be specified in : C:\Temp\Stopwatch.pause.options
```

### Resume

Sample Output :

```text
> stopwatch resume timer1
timer1 resumed

> stopwatch resume timer1 -set
timer1 resumed
timer1: Active - 33 seconds
```

[//]: # (CMD_HELPOUTPUT Resume)

> Stopwatch Resume -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch Resume [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                      The name of the Stopwatch (Required)
-set, --show-elapsed-time +/-              Show the Stopwatch Elapsed Time (Default:false)
-etdf, --elapsed-time-display-format Text  The format string to use to display Elapsed Time (Default:{name}: {state} - {days} days, {hours} hours, {minutes} minutes, {seconds} seconds)
-i, --ignore-invalid-state +/-             Ignore errors of Stopwatch being in invalid state for the action (Default:false)
-v, --verbose-output +/-                   Control verbosity of output messages (Default:true)
-at, --additional-text Text                Additional text for the output
-df, --data-filename Text                  The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-!, --version +/-                          Show App Version details (Default:false)
-?, --help +/-                             Show Help screen (Default:false)
-@, --use-default-arguments-file +/-       Use Default Arguments File (Stopwatch.resume.options) (Default:true)
-$, --use-local-arguments-file +/-         Use Local Arguments File (Stopwatch.resume.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.resume.options
Local App arguments can be specified in : C:\Temp\Stopwatch.resume.options
```

### Purge

Sample Output :

```text
> stopwatch purge -d 30
Deleting Timer: bob
```

[//]: # (CMD_HELPOUTPUT Purge)

> Stopwatch Purge -?

```text
Stopwatch v2.0.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2026 Martin Smith

Usage:
Stopwatch Purge [OPTIONS]

Options:
-d, --days Integer                    Remove Timers started before specified days
-h, --hours Integer                   Remove Timers started before specified hours
-s, --state Text                      Limit which Timer states can be removed (Values: Active, Inactive)
-v, --verbose-output +/-              Control verbosity of output messages (Default:true)
-df, --data-filename Text             The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-!, --version +/-                     Show App Version details (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (Stopwatch.pause.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (Stopwatch.pause.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\Stopwatch.pause.options
Local App arguments can be specified in : C:\Temp\Stopwatch.pause.options
```
