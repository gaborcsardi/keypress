test_that("keypress() errors when platform has no support", {
  fake(keypress, "has_keypress_support", function() FALSE)
  expect_error(keypress(), "does not support keypress")
})

test_that("keypress() rejects non-scalar block", {
  fake(keypress, "has_keypress_support", function() TRUE)
  expect_error(
    keypress(block = c(TRUE, FALSE)),
    "'block' must be a logical scalar"
  )
  expect_error(keypress(block = logical()), "'block' must be a logical scalar")
})

test_that("keypress() rejects non-logical block", {
  fake(keypress, "has_keypress_support", function() TRUE)
  # "yes"/"no" are accepted as logicals by as.logical(); a bare string is NA,
  # length 1 — passes the length check and reaches the C call. So test with
  # something that as.logical() turns into a vector of wrong length instead.
  expect_error(
    keypress(block = c("TRUE", "FALSE")),
    "'block' must be a logical scalar"
  )
})
