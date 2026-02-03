# PauseN

## Overview

There is no built-in easy way to generate Guids within Windows.
Visual Studio comes with a tool, but it's a Windows UI app and not easy to operate via the keyboard or via batch scripting.

This tool with generate Guids and output them to the console, using a variety of formatting options.

By default, `GuidGenerator` will display (something similar to)

```text
a2ab804b-b2c0-4f3b-b5e4-51711f57fc80
```

A `count` argument allows generating more than a single Guid :

> GuidGenerator -c 5

```text
07493705-8026-4143-9421-6708e9f09786
8c473e62-a033-4652-b20a-4d7f093de272
7f663fbe-daee-4849-bdb7-c531eefd50b6
d786e70e-d3bc-4cc2-ad24-d4c13ef3d6a3
d7cbd535-f4c8-445b-a930-7b4e67f9277d
```

You can control how the Guid(s) are rendered:

> GuidGenerator.exe -c 5 -f D

```text
10ABD6E3-ACDD-44AA-BCB2-69AA43B46D8B
DF9C80C9-B32C-4581-B98F-C185C48489E0
87CB5B40-FFF8-4730-95FF-560197413AE2
102A7B15-C1C2-4C53-AE46-1C359771E3EC
A7775C85-8FEA-4F32-8D8F-7DA371266319
```

And the general output :

> GuidGenerator.exe -c 5 -f D -of "{seq}: {guid}"

```text
1: 2A8912C2-9B54-46A4-A588-F1FBCD272CFC
2: C9818C60-22DB-49E9-AAC8-1B0867A9B0FE
3: E1E08E45-18CE-4A33-A6BB-E6B4DBA05BCA
4: 87F8A149-3FF5-45F9-B3AE-ACE960EA29EE
5: 307ABF7B-DFB8-4642-897D-1BABEE69FB13
```

## Latest Version

[//]: # (APP_LATESTVERSION)

> GuidGenerator -!

```text
v2.2.0.0-dev
```

## Full Help Text

The full help text looks something like this :

[//]: # (APP_HELPOUTPUT)

> GuidGenerator -?

```text
GuidGenerator v2.2.0.0-dev - Generate Guid(s) and output to the console
Copyright ® 2018-2026 Martin Smith

Usage:
GuidGenerator [OPTIONS]

Options:
-g, --generator Value                 The Guid generation method to use (Default:OS, Values: OS, DateTime)
-c, --count Integer                   How many Guids to generate (Default:1)
-f, --format Text                     The .NET format to use to output the Guid value(s)
-of, --output Text                    The output format to use when writing the value(s) (Default:{guid})
-!, --version +/-                     Show App Version details (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (GuidGenerator.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (GuidGenerator.options) (Default:true)

Default App arguments can be specified in : C:\Temp\a6e4aa6c3091438b97abf6d2b52171ed\GuidGenerator.options
Local App arguments can be specified in : C:\Temp\GuidGenerator.options

Notes:
Guid Format options:
 N - 00000000000000000000000000000000
 D - 00000000-0000-0000-0000-000000000000
 B - {00000000-0000-0000-0000-000000000000}
 P - (00000000-0000-0000-0000-000000000000)
(Lower case format specifiers generate values in Lowercase format)
See also : https://learn.microsoft.com/en-us/dotnet/api/system.guid.tostring

Output Format options:
 {guid} - The Guid value
 {seq}  - The incrementing sequence number of generated values (From: 1)
 {id}   - {sequence} left padded with zeroes
```
