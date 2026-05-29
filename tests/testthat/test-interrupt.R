test_that("keypress() is interruptible", {
  skip_on_cran()

  opts <- callr::r_process_options(
    func = function() {
      # Make sure has_keypress_support() is happy in the child.
      Sys.setenv(TERM = "xterm")
      cat("READY\n")
      flush(stdout())
      tryCatch(keypress::keypress(), interrupt = function(...) "interrupted")
    },
    stdout = NULL,
    stderr = NULL
  )
  opts$extra$pty <- TRUE

  p <- callr::r_process$new(opts)
  on.exit(p$kill(), add = TRUE)

  # on Windows R adds a "set title" ANSI sequence
  strip_ansi <- function(x) {
    gsub("\033(?:\\[[0-9;?]*[A-Za-z]|\\][^\007]*\007)", "", x, perl = TRUE)
  }
  out <- ""
  deadline <- Sys.time() + 10
  while (
    Sys.time() < deadline && !grepl("READY", strip_ansi(out), fixed = TRUE)
  ) {
    p$poll_io(200)
    out <- paste0(out, p$read_output())
  }
  expect_match(strip_ansi(out), "READY", fixed = TRUE)

  # Give keypress() a moment to enter its poll loop, then interrupt it.
  Sys.sleep(0.5)
  p$interrupt()

  # The interrupt should be caught and turned into "interrupted", and the
  # process should finish on its own (rather than hanging).
  p$wait(timeout = 5000)
  expect_false(p$is_alive())
  res <- p$get_result()
  expect_equal(res, "interrupted")
})
