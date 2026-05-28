library(testthat)
library(keypress)

if (Sys.getenv("NOT_CRAN") == "true") {
  test_check("keypress")
}
