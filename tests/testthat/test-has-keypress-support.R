test_that("returns has_canonical_mode if set in rstudio", {
  fake(
    has_keypress_support,
    "rstudio_detect",
    function() list(type = "rstudio_terminal", has_canonical_mode = TRUE)
  )
  expect_true(has_keypress_support())
})

not_rstudio <- function() list(type = "not_rstudio", has_canonical_mode = FALSE)

test_that("TRUE on a clean terminal outside RStudio", {
  fake(has_keypress_support, "rstudio_detect", not_rstudio)
  fake(has_keypress_support, "isatty", function(con) TRUE)
  fake(has_keypress_support, "platform_gui", "X11")
  withr::local_envvar(c(R_GUI_APP_VERSION = "", EMACS = "", TERM = "xterm"))
  withr::local_options(STERM = NULL)
  expect_true(has_keypress_support())
})

test_that("FALSE when stdin is not a tty", {
  fake(has_keypress_support, "rstudio_detect", not_rstudio)
  fake(has_keypress_support, "isatty", function(con) FALSE)
  fake(has_keypress_support, "platform_gui", "X11")
  withr::local_envvar(c(R_GUI_APP_VERSION = "", EMACS = "", TERM = "xterm"))
  withr::local_options(STERM = NULL)
  expect_false(has_keypress_support())
})

test_that("FALSE when R.app sets R_GUI_APP_VERSION", {
  fake(has_keypress_support, "rstudio_detect", not_rstudio)
  fake(has_keypress_support, "isatty", function(con) TRUE)
  fake(has_keypress_support, "platform_gui", "X11")
  withr::local_envvar(c(R_GUI_APP_VERSION = "1.70", EMACS = "", TERM = "xterm"))
  withr::local_options(STERM = NULL)
  expect_false(has_keypress_support())
})

test_that("FALSE on Windows Rgui", {
  fake(has_keypress_support, "rstudio_detect", not_rstudio)
  fake(has_keypress_support, "isatty", function(con) TRUE)
  fake(has_keypress_support, "platform_gui", "Rgui")
  withr::local_envvar(c(R_GUI_APP_VERSION = "", EMACS = "", TERM = "xterm"))
  withr::local_options(STERM = NULL)
  expect_false(has_keypress_support())
})

test_that("FALSE in Emacs ESS (STERM option)", {
  fake(has_keypress_support, "rstudio_detect", not_rstudio)
  fake(has_keypress_support, "isatty", function(con) TRUE)
  fake(has_keypress_support, "platform_gui", "X11")
  withr::local_envvar(c(R_GUI_APP_VERSION = "", EMACS = "", TERM = "xterm"))
  withr::local_options(STERM = "iESS")
  expect_false(has_keypress_support())
})

test_that("FALSE when EMACS=t", {
  fake(has_keypress_support, "rstudio_detect", not_rstudio)
  fake(has_keypress_support, "isatty", function(con) TRUE)
  fake(has_keypress_support, "platform_gui", "X11")
  withr::local_envvar(c(R_GUI_APP_VERSION = "", EMACS = "t", TERM = "xterm"))
  withr::local_options(STERM = NULL)
  expect_false(has_keypress_support())
})

test_that("FALSE when TERM=dumb", {
  fake(has_keypress_support, "rstudio_detect", not_rstudio)
  fake(has_keypress_support, "isatty", function(con) TRUE)
  fake(has_keypress_support, "platform_gui", "X11")
  withr::local_envvar(c(R_GUI_APP_VERSION = "", EMACS = "", TERM = "dumb"))
  withr::local_options(STERM = NULL)
  expect_false(has_keypress_support())
})
