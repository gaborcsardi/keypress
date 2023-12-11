
# keypress

> Read a single key press from the terminal

<!-- badges: start -->
[![R-CMD-check](https://github.com/gaborcsardi/keypress/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/gaborcsardi/keypress/actions/workflows/R-CMD-check.yaml)
[![](https://www.r-pkg.org/badges/version/cli)](https://www.r-pkg.org/pkg/cli)
[![CRAN RStudio mirror
downloads](https://cranlogs.r-pkg.org/badges/cli)](https://www.r-pkg.org/pkg/cli)
[![Codecov test
coverage](https://codecov.io/gh/gaborcsardi/keypress/branch/main/graph/badge.svg)](https://app.codecov.io/gh/gaborcsardi/keypress?branch=main)
<!-- badges: end -->

## Installation

```r
install.packages("keypress")
```

## Usage

```r
> has_keypress_support()
[1] TRUE
```

```r
> x <- keypress()
> cat("You pressed key", x, "\n")
You pressed key h
```

## Support

This is unfortunately impossible to do in
- RStudio,
- R.app (the builtin OSX GUI),
- RGui (the builtin Windows GUI),
- Emacs ESS, or
- if `stdin()` is not a terminal.

In other words, it pretty much only works interactively, in a terminal.
The terminal can be pretty much any Linux, OSX, or other Unix terminal or
Windows cmd (also called RTerm in R).

## Keys

Note that not all keys are supported, in particular most of the modifiers
(alt, shift, etc.) are not. Supported special keys are:
- Arrow keys: `up`, `down`, `right`, `left`. Available on most keyboards.
- Function keys: from `f1` to `f12`. Available on most keyboards.
- Other special keys: `enter`, `backspace`, `escape`, `tab`.
- Some key combinations with the `CONTROL` key: `ctrl-a`, `ctrl-b`,
  `ctrl-c`, `ctrl-d`, `ctrl-e`, `ctrl-f`, `ctrl-h`, `ctrl-k`, `ctrl-l`,
  `ctrl-n`, `ctrl-p`, `ctrl-t`, `ctrl-u`, `ctrl-w`.
- Keys typically not availale on OS X keyboards: `home`, `end`, `insert`,
  `delete`, `pageup`, `pagedown`.
- On Linux and OS X Unicode characters are supported, but not (yet) on
  Windows. Unicode characters are returned UTF-8 encoded.

## License

MIT © [Gábor Csárdi](https://github.com/gaborcsardi)
