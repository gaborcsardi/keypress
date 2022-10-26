# keypress 1.3.0

- Allow turning echo off for non-blocking Linux and macOS terminals
  (#11, @weshinsley)
- Support non-blocking press on Windows (Rterm)
- `keypress()` now returns `NA` if no key was pressed in non-blocking mode,
  as it is documented.

# 1.2.0

- Support RStudio terminal.
- Better detection of support in Emacs.

# 1.1.1

- Use native routine registration to avoid R CMD check warnings.
- Make sure compilation units are not empty, to avoid another R CMD check
  warning.

# 1.1.0

- Non-blocking reads.
- Better reading individual keys from a copy-paste, or the OS buffer
  in general.

# 1.0.0

First released version.
