test_that("keypress() times out", {
  skip_on_cran()

  opts <- callr::r_process_options(
    func = function() {
      # Make sure has_keypress_support() is happy in the child.
      Sys.setenv(TERM = "xterm")
      cat("READY\n")
      flush(stdout())
      t0 <- Sys.time()
      res <- keypress::keypress(timeout = 1)
      dt <- as.numeric(Sys.time() - t0, units = "secs")
      list(res = res, elapsed = dt)
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

  # We never press a key, so keypress() should return NA on its own after
  # roughly one second, and the process should finish without hanging.
  p$wait(timeout = 5000)
  expect_false(p$is_alive())
  res <- p$get_result()
  expect_true(is.na(res$res))
  expect_gte(res$elapsed, 1)
})
