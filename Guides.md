# Guides

## Options file handling

You can ovveride the default parameter values for any app by adding a `.options` file to specific places and populating it
with default values.

E.g. On Windows, `BannerText` will :
- Check for a file called `BannerText.options` in the same folder the `.exe` runs from and loads values from there if found
- Check for a file called `BannerText.options` in the Users `Home` folder and loads values from there if found, overwriting any values it has already
- Check for a file called `Bannertext.options` in the current folder and loads values from there if found, overwriting any values it has already
- Read arguments supplied on the command line and loads values from there
  - >NOTE: Command line arguments also support adding a file of parameters via an argument on the format '@{filename}'

### Options file format

The format of the file should be one line per argument.

E.g. Each line should either be

- A single positional argument (quoted with " if it contains whitespace)
- A single option with option name and value separated by whitespace

#### Example 1

A sample options file for `PauseN` might be :

`PauseN.options`
```txt
"Pls wait"
-t 600
```

#### Example 2

A sample options file for `BannerText` might be :

`BannerText.options`
```txt
-minl 78
-tsgs 0
-tsc 0
-hlc "="
-flc "="
-tlc "-"
```
