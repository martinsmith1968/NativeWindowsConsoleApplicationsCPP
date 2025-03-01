# Native Windows Console Applications in C++

A set of native, self-contained console apps for Windows in C++.

> NOTE: For Visual Studio and Microsoft C++ Compiler

## Overview

This started out as a set of tools for me to have installed on every Windows PC I used (Home / Work / etc), as the complemented and are consumed by,
a set of scripts I also have for automating many repetitive tasks and providing a common command line experience.

They were originally in NET Framework 2.0 as that was a common runtime I always had available.

They have now morphed into a set of tools that provide a useful baseline for developing in a new language with something tangible to build towards
in order to learn the language concepts, tooling and framworks.

These include :

- Working with a suitable IDE
  - JetBrains have largely covered this one off with their suite of IDEs that all operate in an extremely similar way, but sometimes VSCode with
  extensions and learning the command line tooling is a better option for learning.
- Command Line Parsing, with individual Command support
  - _I rolled my own (rather crude one) for the C++ version as there are features I required which other libraries did not provide.
  This is not something I would likely repeat for porting to a different language_.
- Modules
  - Separating common code to be consumed across different executable outputs, and (optionally) packaging for wider sonsumption.
- Lite Framework
  - Generally this is not an idea I'm in favour of ('Organisations' having their own Framework layer on top of the language) but for this it made
  spinning up a new application simple and allowed focusing mainly on the implementation logic. The downside being that there are limited docs and
  so trying to revisit at a point in future will likely be tough.
- Compiling to a single self-contained executable
- Unit Testing
- Automated bulids and release publishing (currenly via GitHub actions)
- Classes (if relevant) and/or logic encapsulation / code organisation
- Console input reading (`PauseN`)
- Local file storage (per User)

## Applications

- [BannerText](.docs/BannerText.md)
- [PauseN](.docs/PauseN.md)
- [ShowDateTime](.docs/ShowDateTime.md)
- [Stopwatch](.docs/Stopwatch.md)
