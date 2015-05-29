
#ifndef WIN32

#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>

#include <Rinternals.h>

SEXP function_key(const char *buf, size_t buf_size) {
  buf++;			/* escape character */
  buf_size -= 2;		/* minus first escape and trailing zero */

  /* xterm */
  if (! strncmp(buf, "[A", buf_size)) {
    return mkString("up");
  } else if (!strncmp(buf, "[B", buf_size)) {
    return mkString("down");
  } else if (!strncmp(buf, "[C", buf_size)) {
    return mkString("right");
  } else if (!strncmp(buf, "[D", buf_size)) {
    return mkString("left");
  } else if (!strncmp(buf, "[E", buf_size)) {
    return mkString("clear");
  } else if (!strncmp(buf, "[F", buf_size)) {
    return mkString("end");
  } else if (!strncmp(buf, "[H", buf_size)) {
    return mkString("home");

    /* xterm/gnome */
  } else if (!strncmp(buf, "OA", buf_size)) {
    return mkString("up");
  } else if (!strncmp(buf, "OB", buf_size)) {
    return mkString("down");
  } else if (!strncmp(buf, "OC", buf_size)) {
    return mkString("right");
  } else if (!strncmp(buf, "OD", buf_size)) {
    return mkString("left");
  } else if (!strncmp(buf, "OE", buf_size)) {
    return mkString("clear");
  } else if (!strncmp(buf, "OF", buf_size)) {
    return mkString("end");
  } else if (!strncmp(buf, "OH", buf_size)) {
    return mkString("home");

    /* xterm/rxvt */
  } else if (!strncmp(buf, "[1~", buf_size)) {
    return mkString("home");
  } else if (!strncmp(buf, "[2~", buf_size)) {
    return mkString("insert");
  } else if (!strncmp(buf, "[3~", buf_size)) {
    return mkString("delete");
  } else if (!strncmp(buf, "[4~", buf_size)) {
    return mkString("end");
  } else if (!strncmp(buf, "[5~", buf_size)) {
    return mkString("pageup");
  } else if (!strncmp(buf, "[6~", buf_size)) {
    return mkString("pagedown");

    /* putty */
  } else if (!strncmp(buf, "[[5~", buf_size)) {
    return mkString("pageup");
  } else if (!strncmp(buf, "[[6~", buf_size)) {
    return mkString("pagedown");

    /* rxvt */
  } else if (!strncmp(buf, "[[7~", buf_size)) {
    return mkString("home");
  } else if (!strncmp(buf, "[[8~", buf_size)) {
    return mkString("end");

    /* xterm/gnome */
  } else if (!strncmp(buf, "OP", buf_size)) {
    return mkString("f1");
  } else if (!strncmp(buf, "OQ", buf_size)) {
    return mkString("f2");
  } else if (!strncmp(buf, "OR", buf_size)) {
    return mkString("f3");
  } else if (!strncmp(buf, "OS", buf_size)) {
    return mkString("f4");

    /* common */
  } else if (!strncmp(buf, "[15~", buf_size)) {
    return mkString("f5");
  } else if (!strncmp(buf, "[17~", buf_size)) {
    return mkString("f6");
  } else if (!strncmp(buf, "[18~", buf_size)) {
    return mkString("f7");
  } else if (!strncmp(buf, "[19~", buf_size)) {
    return mkString("f8");
  } else if (!strncmp(buf, "[20~", buf_size)) {
    return mkString("f9");
  } else if (!strncmp(buf, "[21~", buf_size)) {
    return mkString("f10");
  } else if (!strncmp(buf, "[23~", buf_size)) {
    return mkString("f11");
  } else if (!strncmp(buf, "[24~", buf_size)) {
    return mkString("f12");

    /* xterm / rxvt */
  } else if (!strncmp(buf, "[11~", buf_size)) {
    return mkString("f1");
  } else if (!strncmp(buf, "[12~", buf_size)) {
    return mkString("f2");
  } else if (!strncmp(buf, "[13~", buf_size)) {
    return mkString("f3");
  } else if (!strncmp(buf, "[14~", buf_size)) {
    return mkString("f4");

  } else {
    return mkString(buf - 1);
  }

  return R_NilValue;
}

SEXP keypress() {
  char buf[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  struct termios old = { 0 };

  if (tcgetattr(0, &old) < 0) {
    error("Cannot query terminal flags");
  }

  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;

  if (tcsetattr(0, TCSANOW, &old) < 0) {
    error("Cannot set canonical mode");
  }

  if (read(0, buf, 1) < 0) {
    error("Cannot read key");
  }

  /* If an escape sequence, then we read the rest.
     We cannot read everything in one go, because of
     copy-and-paste. This way for a copy-and-paste
     we only read the first character. */

  if (buf[0] == '\033') {
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);
    read(0, buf + 1, sizeof(buf) - 2L);
    fcntl(0, F_SETFL, flags);
  }

  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;

  if (tcsetattr(0, TCSADRAIN, &old) < 0) {
    error("Cannot reset terminal flags");
  }

  if (buf[0] == '\033') {
    /* Some excape sequence */
    return function_key(buf, sizeof(buf));
  }  else {
    /* Single character */
    return mkString(buf);
  }
}

#endif

/* -------------------------------------------------------------- */

#ifdef WIN32

#include <conio.h>
#include <ctype.h>

#include <Rinternals.h>

SEXP keypress(){
  int ch;
  char buf[2] = { 0, 0 };

  ch = _getch();
  buf[0] = (char) ch;

  if (ch == 0) {
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
