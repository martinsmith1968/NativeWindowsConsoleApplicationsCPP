# PauseN

## Overview

Windows has a built in command called `Pause` which when executed displays a message on the console and waits until the current user presses a key.

```text
Press any key to continue . . .
```

`PauseN` extends this to automatically continue as if a key had been pressed after a configurable timeout.

By default, `PauseN` will display

```text
Press any key to continue (or wait 30 seconds) . . .
```

Both the `message-text` and the `timeout-seconds` to wait can be configured (as well as other things).

## Latest Version

[//]: # (APP_LATESTVERSION)

> PauseN -!

```text
v2.0.0.0-dev
```

## Full Help Text

The full help text looks something like this :

[//]: # (APP_HELPOUTPUT)

> PauseN -?

```text
PauseN v2.0.0.0-dev - Pause console output for a keypress, or a timeout
Copyright ® 2018-2025 Martin Smith

Usage:
PauseN [message-text] [OPTIONS]

Options:
[message-text] Text                   The Text to display (Default:Press any key to continue (or wait {timeout} seconds) . . . )
-t, --timeout Integer                 The timeout to wait for in seconds (Default:30)
-s, --sleep Integer                   The period to sleep for between checks for in milliseconds (Default:200)
-c, --timeout-cancelled-text Text     The text to show when a timeout is cancelled (Default:(Timeout Cancelled))
-e, --escape-cancels-timeout +/-      Allow ESC to cancel timeout (Default:true)
-!, --version +/-                     Show App Version details (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (PauseN.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (PauseN.options) (Default:true)

Default App arguments can be specified in : C:\Temp\8202bbc2ba474df4bef2a146fe9bb927\PauseN.options
Local App arguments can be specified in : C:\Temp\PauseN.options
```
