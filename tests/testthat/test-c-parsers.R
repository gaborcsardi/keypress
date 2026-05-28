single_char <- function(byte) {
  .Call(C_test_single_char, as.raw(byte))
}

fn_key <- function(bytes) {
  .Call(C_test_function_key, as.raw(bytes))
}

# Convenience: turn a string into the bytes that follow the leading ESC
seq_bytes <- function(s) charToRaw(s)

test_that("single_char maps control keys", {
  skip_on_os("windows")
  expect_identical(single_char(1L), "ctrl-a")
  expect_identical(single_char(2L), "ctrl-b")
  expect_identical(single_char(3L), "ctrl-c")
  expect_identical(single_char(4L), "ctrl-d")
  expect_identical(single_char(5L), "ctrl-e")
  expect_identical(single_char(6L), "ctrl-f")
  expect_identical(single_char(8L), "ctrl-h")
  expect_identical(single_char(11L), "ctrl-k")
  expect_identical(single_char(12L), "ctrl-l")
  expect_identical(single_char(14L), "ctrl-n")
  expect_identical(single_char(16L), "ctrl-p")
  expect_identical(single_char(20L), "ctrl-t")
  expect_identical(single_char(21L), "ctrl-u")
  expect_identical(single_char(23L), "ctrl-w")
})

test_that("single_char maps named keys", {
  skip_on_os("windows")
  expect_identical(single_char(9L), "tab")
  expect_identical(single_char(10L), "enter")
  expect_identical(single_char(13L), "enter")
  expect_identical(single_char(27L), "escape")
  expect_identical(single_char(127L), "backspace")
})

test_that("single_char returns printable ASCII as UTF-8", {
  skip_on_os("windows")
  expect_identical(single_char(charToRaw("a")), "a")
  expect_identical(single_char(charToRaw("Z")), "Z")
  expect_identical(single_char(charToRaw("0")), "0")
  expect_identical(single_char(charToRaw(" ")), " ")
})

test_that("function_key parses xterm CSI arrows", {
  skip_on_os("windows")
  expect_identical(fn_key(seq_bytes("[A")), "up")
  expect_identical(fn_key(seq_bytes("[B")), "down")
  expect_identical(fn_key(seq_bytes("[C")), "right")
  expect_identical(fn_key(seq_bytes("[D")), "left")
  expect_identical(fn_key(seq_bytes("[F")), "end")
  expect_identical(fn_key(seq_bytes("[H")), "home")
})

test_that("function_key parses SS3 arrows (xterm/gnome)", {
  skip_on_os("windows")
  expect_identical(fn_key(seq_bytes("OA")), "up")
  expect_identical(fn_key(seq_bytes("OB")), "down")
  expect_identical(fn_key(seq_bytes("OC")), "right")
  expect_identical(fn_key(seq_bytes("OD")), "left")
  expect_identical(fn_key(seq_bytes("OF")), "end")
  expect_identical(fn_key(seq_bytes("OH")), "home")
})

test_that("function_key parses xterm/rxvt navigation keys", {
  skip_on_os("windows")
  expect_identical(fn_key(seq_bytes("[1~")), "home")
  expect_identical(fn_key(seq_bytes("[2~")), "insert")
  expect_identical(fn_key(seq_bytes("[3~")), "delete")
  expect_identical(fn_key(seq_bytes("[4~")), "end")
  expect_identical(fn_key(seq_bytes("[5~")), "pageup")
  expect_identical(fn_key(seq_bytes("[6~")), "pagedown")
})

test_that("function_key parses putty page-nav sequences", {
  skip_on_os("windows")
  expect_identical(fn_key(seq_bytes("[[5~")), "pageup")
  expect_identical(fn_key(seq_bytes("[[6~")), "pagedown")
})

test_that("function_key parses rxvt home/end sequences", {
  skip_on_os("windows")
  expect_identical(fn_key(seq_bytes("[[7~")), "home")
  expect_identical(fn_key(seq_bytes("[[8~")), "end")
})

test_that("function_key parses SS3 F1-F4", {
  skip_on_os("windows")
  expect_identical(fn_key(seq_bytes("OP")), "f1")
  expect_identical(fn_key(seq_bytes("OQ")), "f2")
  expect_identical(fn_key(seq_bytes("OR")), "f3")
  expect_identical(fn_key(seq_bytes("OS")), "f4")
})

test_that("function_key parses CSI F5-F12", {
  skip_on_os("windows")
  expect_identical(fn_key(seq_bytes("[15~")), "f5")
  expect_identical(fn_key(seq_bytes("[17~")), "f6")
  expect_identical(fn_key(seq_bytes("[18~")), "f7")
  expect_identical(fn_key(seq_bytes("[19~")), "f8")
  expect_identical(fn_key(seq_bytes("[20~")), "f9")
  expect_identical(fn_key(seq_bytes("[21~")), "f10")
  expect_identical(fn_key(seq_bytes("[23~")), "f11")
  expect_identical(fn_key(seq_bytes("[24~")), "f12")
})

test_that("function_key parses xterm/rxvt F1-F4", {
  skip_on_os("windows")
  expect_identical(fn_key(seq_bytes("[11~")), "f1")
  expect_identical(fn_key(seq_bytes("[12~")), "f2")
  expect_identical(fn_key(seq_bytes("[13~")), "f3")
  expect_identical(fn_key(seq_bytes("[14~")), "f4")
})

test_that("function_key returns escape when buffer contains only ESC", {
  skip_on_os("windows")
  expect_identical(fn_key(raw()), "escape")
})
