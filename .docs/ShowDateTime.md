# ShowDateTime

## Overview

I wanted a reliable way of getting the Current Date and/or Time into a batch file. There are some existing methods
but I have found reliability and/or consistency issues as I move from machine to machine.
E.g.

- `DATE /T`
  - :white_check_mark: Built-in Windows command
  - :x: Does not always return in the same format making parsing difficult
- `TIME /T`
  - :white_check_mark: Built-in Windows command
  - :x: Does not always return in the same format making parsing difficult
  - :x: Does not report seconds or milliseconds
- `wmic os get LocalDateTime /VALUE`
  - :x: Not available on all Windows machines
  - :x: Does not always return in the same format making parsing difficult

So I came up with ShowDateTime, which allows you to control the format of the output via parameters.

> NOTE: The default output format is a full unambiguous consistently parseable date and time

## Basic Usage

```txt
> ShowDateTime
2025-03-01 09:53:40.880
```

```txt
> showdatetime -f "%Y"
2025
```

## Latest Version

[//]: # (APP_LATESTVERSION)

> ShowDateTime -!

```text
v2.0.0.0-dev
```

## Full Help Text

The full help text looks something like this :

[//]: # (APP_HELPOUTPUT)

> ShowDateTime -?

```text
ShowDateTime v2.0.0.0-dev - Show current date and time formatted
Copyright ® 2018-2026 Martin Smith

Usage:
ShowDateTime [OPTIONS]

Options:
-f, --format Text                     The format to use to display the datetime value (Default:%Y-%m-%d %H:%M:%S.{fff})
-t, --type Value                      The time value to use (Default:Local, Values: Local, UTC)
-!, --version +/-                     Show App Version details (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (ShowDateTime.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (ShowDateTime.options) (Default:true)

Default App arguments can be specified in : C:\Temp\e40f117bddc7411ab9797a4bc6f1fa42\ShowDateTime.options
Local App arguments can be specified in : C:\Temp\ShowDateTime.options

Notes:
This uses 'strftime' internally, so see the following for supported date formats:
  https://cplusplus.com/reference/ctime/strftime/
As well as:
  {fff} - Milliseconds - always 3 digits
  {f}   - Milliseconds - as few digits as possible
  {qq}  - Month Quarter (left padded with 0 to 2 digits
  {q}   - Month Quarter
  {M}   - Month number (1 - 12)
  {d}   - Day number (1 - 31)
  {H}   - 24 Hour number (0 - 23)
  {h}   - Hour number (0 - 12)
```
