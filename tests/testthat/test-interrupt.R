test_that("keypress() is interruptible", {
  skip_on_os("windows")
  skip_on_cran()

  # Run keypress() in a child R process, attached to a pseudo-terminal so
  # that has_keypress_support() is satisfied and keypress() actually blocks.
  # We then send a SIGINT and check that the blocking read is interrupted
  # cleanly, i.e. the interrupt condition is raised and can be caught.
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

  # Wait until the child signals that it is about to block in keypress().
  out <- ""
  deadline <- Sys.time() + 10
  while (Sys.time() < deadline && !grepl("READY", out, fixed = TRUE)) {
    p$poll_io(200)
    out <- paste0(out, p$read_output())
  }
  expect_match(out, "READY", fixed = TRUE)

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
