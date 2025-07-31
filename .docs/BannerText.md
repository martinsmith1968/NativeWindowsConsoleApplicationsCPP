# BannerText

## Overview

Show a text message in the console, surrounded with lines to appear as a banner.

E.g
```cmd
BannerText "Hello World"
```

```text
*******************
**  Hello World  **
*******************
```

There are options to control almost all aspects of the output

E.g. Fix the output to 80 characters

```cmd
BannerText "Hello World" -minl 80
```

```text
********************************************************************************
**  Hello World                                                               **
********************************************************************************
```

And you can align the text output too

E.g. Fix the output to 80 characters

```cmd
BannerText "Hello World" -minl 80 -ta Center
```

```text
********************************************************************************
**                                Hello World                                 **
********************************************************************************
```

## Full Help Text


```text
BannerText v1.12.0.0-dev - Generate and display a Text Banner in the console
Copyright © 2018-2025 Martin Smith

Usage:
BannerText [message-text] [OPTIONS]

Options:
[message-text] Text                     The Text to display (Required)
-hlc, --header-line-char Char           The character to use for header lines (Default:*)
-hln, --header-line-count Integer       The number of header lines to print (Default:1)
-flc, --footer-line-char Char           The character to use for footer lines (Default:*)
-fln, --footer-line-count Integer       The number of footer lines to print (Default:1)
-tlc, --text-line-char Char             The character to use for text line prefix/suffix (Default:*)
-tpc, --title-prefix-count Integer      Set Title Prefix Count (Default:2)
-tsc, --title-suffix-count Integer      Set Title Suffix Count (Default:2)
-tpgs, --title-prefix-gap-size Integer  Set Title Prefix Gap Size (Default:2)
-tsgs, --title-suffix-gap-size Integer  Set Title Suffix Gap Size (Default:2)
-ta, --text-alignment Value             Set Text Alignment (Default:Left, Values: Left, Right, Center)
-minl, --min-total-length Integer       Set Minimum Total line length (Default:0)
-maxl, --max-total-length Integer       Set Maximum Total line length (Default:0)
-?, --help +/-                          Show Help screen (Default:false)
-@, --use-default-arguments-file +/-    Use Default Arguments File (BannerText.options) (Default:true)
-$, --use-local-arguments-file +/-      Use Local Arguments File (BannerText.options) (Default:true)

Default App arguments can be specified in : C:\dev\Github\martinsmith1968\NativeWindowsConsoleApplicationsCPP\Output\x64\Debug\BannerText.options
Local App arguments can be specified in : C:\dev\Github\martinsmith1968\NativeWindowsConsoleApplicationsCPP\.docs\BannerText.options
```
