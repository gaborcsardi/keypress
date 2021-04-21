
#' @theme assets/extra.css assets/rd.js
NULL

#' Read a single keypress at the terminal
#'
#' It currently only works at Linux/Unix and OSX terminals,
#' and at the Windows command line. see \code{\link{has_keypress_support}}.
#'
#' The following special keys are supported:
#' * Arrow keys: 'up', 'down', 'right', 'left'.
#' * Function keys: from 'f1' to 'f12'.
#' * Others: 'home', 'end', 'insert', 'delete', 'pageup', 'pagedown',
#'     'tab', 'enter', 'backspace' (same as 'delete' on OSX keyboards),
#'     'escape'.
#' * Control with one of the following keys: 'a', 'b', 'c', 'd', 'e', 'f',
#'     'h', 'k', 'l', 'n', 'p', 't', 'u', 'w'.
#'
#' @param block Whether to wait for a key press, if there is none
#'   available now.
#' @return The key pressed, a character scalar. For non-blocking reads
#'   NA is returned if no keys are available.
#'
#' @family keypress function
#' @useDynLib keypress, .registration = TRUE, .fixes = "C_"
#' @export
#' @examplesIf FALSE
#' x <- keypress()
#' cat("You pressed key", x, "\n")

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
#' @details
#' Supported platforms:
#' * Terminals in Windows and Unix.
#' * RStudio terminal.
#'
#' Not supported:
#' * RStudio (if not in the RStudio terminal).
#' * R.app on macOS.
#' * Rgui on Windows.
#' * Emacs ESS.
#' * Others.
#'
#' @return Whether there is support for waiting for individual
#' keypressses.
#'
#' @family keypress function
#' @export
#' @examples
#' has_keypress_support()

has_keypress_support <- function() {
  ## Supported if we have a terminal or RStudio terminal.
  ## Not supported otherwise in RStudio, R.app, Rgui or Emacs

  rs <- rstudio$detect()

  if (rs$type != "not_rstudio") {
    rs$has_canonical_mode

  } else {
    isatty(stdin()) &&
      Sys.getenv("R_GUI_APP_VERSION") == "" &&
      .Platform$GUI != "Rgui" &&
      ! identical(getOption("STERM"), "iESS") &&
      Sys.getenv("EMACS") != "t" &&
      Sys.getenv("TERM") != "dumb"
  }
}

#' Save current terminal state
#'
#' For Linux/Unix and OSX terminals, save the current flags relating to
#' behaviour of the terminal used to call the R script.
#'
#' For Linux/Unix and OSX terminals using non-blocking keypress, special
#' settings need applying to prevent characters being echoed to the terminal
#' as well as being interpreted by keypress. However, any changes made
#' to the settings will by default persist after exiting your R script.
#' Therefore, should you need non-blocking keypresses from one of these
#' terminals, you should save the terminal settings and turn off echo near
#' the beginning of your code, and then restore the original terminal
#' settings on exit.
#'
#' This will be safely ignored on Windows, as it is not needed.
#'
#' @family terminal functions
#' @useDynLib keypress, .registration = TRUE, .fixes = "C_"
#' @export

save_term_status <- function() {
  .Call(C_save_term_status)
  invisible()
}

#' Restore previously saved terminal state
#'
#' For Linux/Unix and OSX terminals, restore the current flags relating to
#' behaviour of the terminal used to call the R script.
#'
#' For Linux/Unix and OSX terminals using non-blocking keypress, special
#' settings need applying to prevent characters being echoed to the terminal
#' as well as being interpreted by keypress. However, any changes made
#' to the settings will by default persist after exiting your R script.
#' Therefore, should you need non-blocking keypresses from one of these
#' terminals, you should save the terminal settings and turn off echo near
#' the beginning of your code, and then restore the original terminal
#' settings on exit.
#'
#' This will be safely ignored on Windows, as it is not needed.
#'
#' @family terminal functions
#' @useDynLib keypress, .registration = TRUE, .fixes = "C_"
#' @export

restore_term_status <- function() {
  .Call(C_restore_term_status)
  invisible()
}

#' Set echo flag of terminal.
#'
#' For Linux/Unix and OSX terminals, set the echo flag to ON or OFF.
#'
#' You will need to set echo to FALSE if you want the R script to handle all
#' of the keypress-related behaviour, without any keys being echoed in the
#' terminal by the operating system, when using non-blocking keypress. This is
#' not necessary when running in a Windows command prompt, and will be ignored.
#'
#' @param echo logical, where FALSE (the default) means turn echo off.
#'
#' @family terminal functions
#' @useDynLib keypress, .registration = TRUE, .fixes = "C_"
#' @export

set_term_echo <- function(echo = FALSE) {
  echo <- as.logical(echo)
  if (length(echo) != 1) stop("'echo' must be a logical scalar")
  .Call(C_set_term_echo, echo)
  invisible()
}
