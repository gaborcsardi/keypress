
#ifdef WIN32

#include "keypress.h"
#include <windows.h>
#include <ctype.h>

#include <Rinternals.h>

static HANDLE console_in, console_out;

static int enableRawMode() {
  if (!console_in) {
    console_in = GetStdHandle(STD_INPUT_HANDLE);
    console_out = GetStdHandle(STD_OUTPUT_HANDLE);
  }
  return 0;
}

static int disableRawMode() {
  /* Nothing to do yet */
  return 0;
}

SEXP getWinChar() {
  INPUT_RECORD rec;
  DWORD count;
  char buf[2] = { 0, 0 };
  int chr;

  for (;; Sleep(10)) {
    ReadConsoleInputA(console_in, &rec, 1, &count);
    if (rec.EventType != KEY_EVENT) continue;
    if (! rec.Event.KeyEvent.bKeyDown) continue;
    buf[0] = chr = rec.Event.KeyEvent.uChar.AsciiChar;

    switch (rec.Event.KeyEvent.wVirtualKeyCode) {

    case VK_RETURN: return mkString("enter");
    case VK_BACK:   return mkString("backspace");
    case VK_LEFT:   return mkString("left");
    case VK_RIGHT:  return mkString("right");
    case VK_UP:     return mkString("up");
    case VK_DOWN:   return mkString("down");
    case VK_INSERT: return mkString("insert");
    case VK_DELETE: return mkString("delete");
    case VK_HOME:   return mkString("home");
    case VK_END:    return mkString("end");
    case VK_ESCAPE: return mkString("escape");

    case VK_F1:     return mkString("f1");
    case VK_F2:     return mkString("f2");
    case VK_F3:     return mkString("f3");
    case VK_F4:     return mkString("f4");
    case VK_F5:     return mkString("f5");
    case VK_F6:     return mkString("f6");
    case VK_F7:     return mkString("f7");
    case VK_F8:     return mkString("f8");
    case VK_F9:     return mkString("f9");
    case VK_F10:    return mkString("f10");
    case VK_F11:    return mkString("f11");
    case VK_F12:    return mkString("f12");

    default:
      if (rec.Event.KeyEvent.dwControlKeyState &
	  (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) {
	switch (chr) {
	case 1: return mkString("ctrl-a");
	case 2: return mkString("ctrl-b");
	case 3: return mkString("ctrl-c");
	case 4: return mkString("ctrl-d");
	case 5: return mkString("ctrl-e");
	case 6: return mkString("ctrl-f");
	case 8: return mkString("ctrl-h");
	case 11: return mkString("ctrl-k");
	case 12: return mkString("ctrl-l");
	case 14: return mkString("ctrl-n");
	case 16: return mkString("ctrl-p");
	case 20: return mkString("ctrl-t");
	case 21: return mkString("ctrl-u");
	case 22: return mkString("ctrl-w");
	}
      } else if (buf[0]) {
	return mkString(buf);
      }
    }

  }
}

SEXP keypress(SEXP s_block) {
  SEXP res;
  CONSOLE_SCREEN_BUFFER_INFO inf;

  enableRawMode();

  GetConsoleScreenBufferInfo(console_out, &inf);

  res = getWinChar();

  disableRawMode();

  return res;
}

#endif
