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

## Full Help Text

### Command List

```text
Stopwatch v1.7.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
Stopwatch [command-name] { [command-arguments] }

Commands:

List       List all active Stopwatches
Start      Start a named Stopwatch
Stop       Start an active Stopwatch
Cancel     Cancel an active Stopwatch
Elapsed    Display the current elapsed time of an active Stopwatch
Pause      Pause an active Stopwatch
Resume     Resume a paused Stopwatch
```

### List

Sample Output :

```text
timer1: Running - 3 days, 1 hours, 55 minutes, 51 seconds
timer2: Running - 3 days, 1 hours, 39 minutes, 11 seconds

Found: 2 stopwatches
```

```text
Stopwatch v1.7.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
Stopwatch List [OPTIONS]

Options:
-o, --output-format Text              Control output format of list (Default:Display, Values: Display, CSV, Custom)
-fmt, --custom-format-text Text       A custom format string for the Timer details
-df, --data-filename Text             The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-v, --verbose-output +/-              Control verbosity of output messages (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (Stopwatch.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (Stopwatch.options) (Default:true)

Default App arguments can be specified in : C:\...\Stopwatch.options
```

### Start

Sample Output :

```text
> stopwatch start timer1
timer1 started at 12:20:59
```

```text
Stopwatch v1.7.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
Stopwatch Start [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                 The name of the Stopwatch (Required)
-df, --data-filename Text             The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-v, --verbose-output +/-              Control verbosity of output messages (Default:true)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (Stopwatch.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (Stopwatch.options) (Default:true)

Default App arguments can be specified in : C:\...\Stopwatch.options
```

### Stop

Sample Output :

```text
> stopwatch stop timer1
timer1: Stopped - 2 minutes, 40 seconds
```

```text
Stopwatch v1.7.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
Stopwatch Stop [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                      The name of the Stopwatch (Required)
-etdf, --elapsed-time-display-format Text  The format string to use to display Elapsed Time (Default:{name}: {state} - {days} days, {hours} hours, {minutes} minutes, {seconds} seconds)
-df, --data-filename Text                  The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-set, --show-elapsed-time +/-              Show the Stopwatch Elapsed Time (Default:true)
-v, --verbose-output +/-                   Control verbosity of output messages (Default:false)
-?, --help +/-                             Show Help screen (Default:false)
-@, --use-default-arguments-file +/-       Use Default Arguments File (Stopwatch.options) (Default:true)
-$, --use-local-arguments-file +/-         Use Local Arguments File (Stopwatch.options) (Default:true)

Default App arguments can be specified in : C:\...\Stopwatch.options
```

### Elapsed

Sample Output :

```text
> Stopwatch.exe elapsed timer2
timer2: Running - 5 minutes, 46 seconds
```

```text
Stopwatch v1.7.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
Stopwatch Elapsed [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                      The name of the Stopwatch (Required)
-etdf, --elapsed-time-display-format Text  The format string to use to display Elapsed Time (Default:{name}: {state} - {days} days, {hours} hours, {minutes} minutes, {seconds} seconds)
-at, --additional-text Text                Additional text for the output
-df, --data-filename Text                  The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-?, --help +/-                             Show Help screen (Default:false)
-@, --use-default-arguments-file +/-       Use Default Arguments File (Stopwatch.options) (Default:true)
-$, --use-local-arguments-file +/-         Use Local Arguments File (Stopwatch.options) (Default:true)

Default App arguments can be specified in : C:\...\Stopwatch.options
```

### Cancel

Sample Output :

```text
> stopwatch cancel timer1
```

```text
Stopwatch v1.7.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
Stopwatch Cancel [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                 The name of the Stopwatch (Required)
-df, --data-filename Text             The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-v, --verbose-output +/-              Control verbosity of output messages (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (Stopwatch.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (Stopwatch.options) (Default:true)

Default App arguments can be specified in : C:\...\Stopwatch.options
```

### Pause

Sample Output :

```text
> stopwatch pause timer1
```

```text
Stopwatch v1.7.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
Stopwatch Pause [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                      The name of the Stopwatch (Required)
-etdf, --elapsed-time-display-format Text  The format string to use to display Elapsed Time (Default:{name}: {state} - {days} days, {hours} hours, {minutes} minutes, {seconds} seconds)
-at, --additional-text Text                Additional text for the output
-df, --data-filename Text                  The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-set, --show-elapsed-time +/-              Show the Stopwatch Elapsed Time (Default:true)
-i, --ignore-invalid-state +/-             Ignore errors of Stopwatch being in invalid state for the action (Default:false)
-v, --verbose-output +/-                   Control verbosity of output messages (Default:false)
-?, --help +/-                             Show Help screen (Default:false)
-@, --use-default-arguments-file +/-       Use Default Arguments File (Stopwatch.options) (Default:true)
-$, --use-local-arguments-file +/-         Use Local Arguments File (Stopwatch.options) (Default:true)

Default App arguments can be specified in : C:\...\Stopwatch.options
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

```text
Stopwatch v1.7.0.0-dev - Control named out-of-process Timers
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
Stopwatch Resume [stopwatch-name] [OPTIONS]

Options:
[stopwatch-name] Text                      The name of the Stopwatch (Required)
-etdf, --elapsed-time-display-format Text  The format string to use to display Elapsed Time (Default:{name}: {state} - {days} days, {hours} hours, {minutes} minutes, {seconds} seconds)
-at, --additional-text Text                Additional text for the output
-df, --data-filename Text                  The filename to store Stopwatch data in (Default:C:\Users\marti\AppData\Local\DNXSolutions\Stopwatch\Stopwatch.data)
-set, --show-elapsed-time +/-              Show the Stopwatch Elapsed Time (Default:false)
-i, --ignore-invalid-state +/-             Ignore errors of Stopwatch being in invalid state for the action (Default:false)
-v, --verbose-output +/-                   Control verbosity of output messages (Default:true)
-?, --help +/-                             Show Help screen (Default:false)
-@, --use-default-arguments-file +/-       Use Default Arguments File (Stopwatch.options) (Default:true)
-$, --use-local-arguments-file +/-         Use Local Arguments File (Stopwatch.options) (Default:true)

Default App arguments can be specified in : C:\...\Stopwatch.options
```
