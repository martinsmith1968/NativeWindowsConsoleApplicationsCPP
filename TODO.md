# To Do

## New Apps

- [ ] Text File BOM Detector / Changer
  - https://en.wikipedia.org/wiki/Byte_order_mark#UTF-8
  - https://superuser.com/questions/1823317/how-to-identify-a-file-encoding

- [ ] Text File Line Ending Detector / Normaliser

## Fixes

- [ ] Utils - File Reading (ReadAllText / ReadAllLines) better line ending support

### General

- [ ] All Utils classes to have unit tests
- [ ] All Utils classes to have XML comments
- [ ] All Utils classes to move to namespaces and become simple static methods
- [X] @filename not working (E.g. bannertext @args.txt does not use lines from file)
- [X] Introduce some app output tests (Abandoned TextTest)
- [X] Invoke via CI

## Enhancements

- [X] Support arrays of arguments (multiple instances of a single Parameter)
- [ ] Separate some classes only in header files into header and implementation
- [X] Support arguments files at all levels in Help
- [X] (Optionally) Support default argument files per command ([appname].[commandname].options)
- [X] (Optionally) Support default argument files per command in Help text
- [ ] Add Help text for adding a file of arguments
- [X] Line Wrapping Help Writer for long argument descriptions to wrap better

### Enhancements - BannerText

- [ ] Introduce `LineWrappingType` parameter for lines that exceed max length (Can use LineWrappingWriter for help usage)

### Enhabcements - FigLetText

- [ ] More embedded fonts. See :
  - https://github.com/drewnoakes/figgle
  - [link text](https://github.com/WenceyWang/FIGlet.Net)
- [ ] Support external Font files

### Enhancements - Stopwatch

- [X] Introduce `SortOrder` parameter to `List` command
- [ ] Output Tests for `Stopwatch` app and commands
