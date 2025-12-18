# PauseN

## Overview

FigLet is way of generating text in a large ASCII art style using various fonts.
This application, `FigLetText`, allows you to generate such text in a Windows console application.

FigLetText has a few enhancements over the original `FigLet` application, such as:

- Multiple output texts in a single call
- Text Alignment in the console window
- Text Framing with single or double line border

Uses a third party embedded library for the text generation : [ebertolazzi/embedFiglet](https://github.com/ebertolazzi/embedFiglet)

Other libraries are available, E.g. :

- github.com/srilakshmikanthanp/libfiglet

### Related Links and History

- [Wikipedia](https://en.wikipedia.org/wiki/FIGlet)
- [History of FiGlet](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwj_59fup-eOAxXQQEEAHduePfgQFnoECCAQAw&url=http%3A%2F%2Fwww.figlet.org%2Ffiglet_history.html%23%3A~%3Atext%3DIn%2520Spring%25201991%252C%2520inspired%2520by%2Cfor%2520a%2520couple%2520of%2520years.&usg=AOvVaw12GQIv6A2xCa9LP9ljHxD9&opi=89978449)

## Examples

```cmd
> FigLetText "Hello World"
```

```text
 _   _       _ _        __        __          _     _
| | | | ___ | | | ___   \ \      / /___  _ __| | __| |
| |_| |/ _ \| | |/ _ \   \ \ /\ / // _ \| '__| |/ _` |
|  _  |  __/| | | (_) |   \ V  V /| (_) | |  | | (_| |
|_| |_|\___||_|_|\___/     \_/\_/  \___/|_|  |_|\__,_|

```

```cmd
> FigLetText Hello World
```

```text
 _   _       _ _
| | | | ___ | | | ___
| |_| |/ _ \| | |/ _ \
|  _  |  __/| | | (_) |
|_| |_|\___||_|_|\___/

__        __          _     _
\ \      / /___  _ __| | __| |
 \ \ /\ / // _ \| '__| |/ _` |
  \ V  V /| (_) | |  | | (_| |
   \_/\_/  \___/|_|  |_|\__,_|

```

## Font Files

FigLet fonts can be found defined as `.flf` files, throughtout the internet. A couple of example stores :

- https://github.com/xero/figlet-fonts
- http://www.figlet.org/examples.html
- https://github.com/hIMEI29A/FigletFonts

The spec can be found here : http://jave.de/figlet/figfont.html:

## Latest Version

[//]: # (APP_LATESTVERSION)

> FigLetText -!

```text
v2.0.0.0-dev
```

## Full Help Text

The full help text looks something like this :

[//]: # (APP_HELPOUTPUT)

> FigLetText -?

```text
FigLetText v2.0.0.0-dev - Generate Text in FigLet format
Copyright ® 2018-2025 Martin Smith

Usage:
FigLetText [message-text] [OPTIONS]

Options:
[message-text] Text                   The Text(s) to display (Required)
-n, --font-type Value                 The font to use (Default:standard, Values: banner, big, doom, larry3d, mini, script, small, standard, straight)
-f, --framing-type Value              The text framing outline to use (Default:none, Values: none, single, double)
-s, --spacing-type Value              The character spacing to use (Default:smushed, Values: smushed, packed, fullwidth, monospaced)
-ta, --text-alignment-type Value      The Text Alignment (Default:Left, Values: Left, Right, Center)
-w, --output-width Integer            Specify a fixed output width (for alignment) (Default:0)
-!, --version +/-                     Show App Version details (Default:false)
-?, --help +/-                        Show Help screen (Default:false)
-@, --use-default-arguments-file +/-  Use Default Arguments File (FigLetText.options) (Default:true)
-$, --use-local-arguments-file +/-    Use Local Arguments File (FigLetText.options) (Default:true)

Default App arguments can be specified in : C:\Temp\ae7d2ec50b3d4cddbd96d7f7076f2173\FigLetText.options
Local App arguments can be specified in : C:\Temp\FigLetText.options
```
