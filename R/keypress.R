
#' Read a single keypress at the terminal
#'
#' It currently only works at Linux/Unix and OSX terminals,
#' and at the Windows command line. see \code{\link{has_keypress_support}}.
#'
#' The following special keys are supported:
#' \itemize{
#'   \item Arrow keys: \sQuote{up}, \sQuote{down}, \sQuote{right},
#'     \sQuote{left}.
#'   \item Function keys: from \sQuote{f1} to \sQuote{f12}.
#'   \item Others: sQuote{home}, \sQuote{end},
#'     \sQuote{insert}, \sQuote{delete}, \sQuote{pageup},
#'     \sQuote{pagedown}, \sQuote{tab}, \sQuote{enter},
#'     \sQuote{backspace} (same as \sQuote{delete} on OSX keyboards),
#'     \sQuote{escape}.
#'   \item Control with one of the following keys: \sQuote{a}, \sQuote{b},
#'     \sQuote{c}, \sQuote{d}, \sQuote{e}, \sQuote{f}, \sQuote{h},
#'     \sQuote{k}, \sQuote{l}, \sQuote{n}, \sQuote{p}, \sQuote{t},
#'     \sQuote{u}, \sQuote{w}.
#' }
#'
#' @param block Whether to wait for a key press, if there is none
#'   available now.
#' @return The key pressed, a character scalar. For non-blocking reads
#'   NA is returned if no keys are available.
#'
#' @family keypress
#' @useDynLib keypress, .registration = TRUE, .fixes = "C_"
#' @export
#' @examples
#' \dontrun{
#' x <- keypress()
#' cat("You pressed key", x, "\n")
#' }

keypress <- function(block = TRUE) {
  if (!has_keypress_support()) {
    stop("Your platform/terminal does not support keypress")
  }
  block <- as.logical(block)
  if (length(block) != 1) stop("'block' must be a logical scalar")
  .Call(C_keypress, block)
}

#' Check if the current platform/terminal supports reading
#' single keys.
#'
#' @return Whether there is support for waiting for individual
#' keypressses.
#'
#' @family keypress
#' @export
#' @examples
#' has_keypress_support()

has_keypress_support <- function() {
  ## Supported if we have a terminal, and we are not in RStudio,
  ## not in R.app, not in Rgui, and not in Emacs.
  ## Yes, pretty limited.
  isatty(stdin()) &&
    Sys.getenv("RSTUDIO") != 1 &&
    Sys.getenv("R_GUI_APP_VERSION") == "" &&
    .Platform$GUI != "Rgui" &&
    ! identical(getOption("STERM"), "iESS") &&
    Sys.getenv("EMACS") != "t"
}
