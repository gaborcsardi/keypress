
#ifdef WIN32

#include "keypress.h"
#include <conio.h>
#include <ctype.h>

#include <Rinternals.h>

SEXP single_char(const char *buf) {

  int ch = buf[0];

  switch(ch) {
  case   1: return mkString("ctrl-a");
  case   2: return mkString("ctrl-b");
  case   3: return mkString("ctrl-c");
  case   4: return mkString("ctrl-d");
  case   5: return mkString("ctrl-e");
  case   6: return mkString("ctrl-f");
  case   8: return mkString("ctrl-h");
  case   9: return mkString("tab");
  case  10: return mkString("enter");
  case  11: return mkString("ctrl-k");
  case  12: return mkString("ctrl-l");
  case  13: return mkString("enter");
  case  14: return mkString("ctrl-n");
  case  16: return mkString("ctrl-p");
  case  20: return mkString("ctrl-t");
  case  21: return mkString("ctrl-u");
  case  23: return mkString("ctrl-w");
  case  27: return mkString("escape");
  case 127: return mkString("backspace");
  default:
    return mkString(buf);
  }
}

SEXP keypress(SEXP s_block){
  int block = LOGICAL(s_block)[0];
  int ch;
  char buf[2] = { 0, 0 };

  if (!block) {
    if (_kbhit()) {
      ch = _getch();
    } else {
      return ScalarString(R_NaString);
    }
  }

  ch = _getch();
  buf[0] = (char) ch;

  if (ch != 0 && ch != 224) {
    return single_char(buf);

  } else if (ch == 0) {
    int ch2 = _getch();

    switch (ch2) {
    case 59: return mkString("f1");
    case 60: return mkString("f2");
    case 61: return mkString("f3");
    case 62: return mkString("f4");
    case 63: return mkString("f5");
    case 64: return mkString("f6");
    case 65: return mkString("f7");
    case 66: return mkString("f8");
    case 67: return mkString("f9");
    case 68: return mkString("f10");
    default:
	error("Unknown key pressed");
	return R_NilValue;
    }

  } else if (ch == 224) {
    int ch2 = _getch();

    switch(ch2) {
    case 71: return mkString("home");
    case 72: return mkString("up");
    case 73: return mkString("pageup");
    case 75: return mkString("left");
    case 77: return mkString("right");
    case 79: return mkString("end");
    case 80: return mkString("down");
    case 81: return mkString("pagedown");
    case 82: return mkString("insert");
    case 83: return mkString("delete");
    case 133: return mkString("f11");
    case 134: return mkString("f12");
    default:
	error("Unknown key pressed");
	return R_NilValue;
    }

  } else {
    return mkString(buf);
  }
}

#endif
