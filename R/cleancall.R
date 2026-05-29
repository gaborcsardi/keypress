call_with_cleanup <- function(ptr, ...) {
  .Call(C_cleancall_call, pairlist(ptr, ...), parent.frame())
}
