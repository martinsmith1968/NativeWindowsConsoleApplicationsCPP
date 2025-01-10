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

## Full Help Text

The full help text looks something like this :

```text
PauseN v1.5.0.0-dev - Pause with a timeout
Copyright ® 2018-2025 DNX Solutions Ltd

Usage:
PauseN [message-text] [OPTIONS]

OPTIONS:
[message-text] Text                   The Text to display (Default:Press any key to continue (or wait {timeout} seconds) . . . )
-t, --timeout Integer                 The timeout to wait for in seconds (Default:30)
-s, --sleep Integer                   The timeout to sleep for between checks for in milliseconds (Default:200)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (PauseN.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (PauseN.options) (Default:true)
```
