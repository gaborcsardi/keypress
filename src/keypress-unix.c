
/* Avoid warning about empty compilation unit. */
void keypress_unix_dummy() { }

#ifndef _WIN32

#include "keypress.h"
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>

keypress_key_t single_char(const char *buf) {

  int ch = buf[0];

  switch(ch) {
  case   1: return keypress_special(KEYPRESS_CTRL_A);
  case   2: return keypress_special(KEYPRESS_CTRL_B);
  case   3: return keypress_special(KEYPRESS_CTRL_C);
  case   4: return keypress_special(KEYPRESS_CTRL_D);
  case   5: return keypress_special(KEYPRESS_CTRL_E);
  case   6: return keypress_special(KEYPRESS_CTRL_F);
  case   8: return keypress_special(KEYPRESS_CTRL_H);
  case   9: return keypress_special(KEYPRESS_TAB);
  case  10: return keypress_special(KEYPRESS_ENTER);
  case  11: return keypress_special(KEYPRESS_CTRL_K);
  case  12: return keypress_special(KEYPRESS_CTRL_L);
  case  13: return keypress_special(KEYPRESS_ENTER);
  case  14: return keypress_special(KEYPRESS_CTRL_N);
  case  16: return keypress_special(KEYPRESS_CTRL_P);
  case  20: return keypress_special(KEYPRESS_CTRL_T);
  case  21: return keypress_special(KEYPRESS_CTRL_U);
  case  23: return keypress_special(KEYPRESS_CTRL_W);
  case  27: return keypress_special(KEYPRESS_ESCAPE);
  case 127: return keypress_special(KEYPRESS_BACKSPACE);
  default : return keypress_utf8(buf);
  }
}

keypress_key_t function_key(const char *buf, size_t buf_size) {
  buf++;			/* escape character */
  buf_size -= 2;		/* minus first escape and trailing zero */

  /* xterm */
  if (! strncmp(buf, "[A", buf_size)) {
    return keypress_special(KEYPRESS_UP);
  } else if (!strncmp(buf, "[B", buf_size)) {
    return keypress_special(KEYPRESS_DOWN);
  } else if (!strncmp(buf, "[C", buf_size)) {
    return keypress_special(KEYPRESS_RIGHT);
  } else if (!strncmp(buf, "[D", buf_size)) {
    return keypress_special(KEYPRESS_LEFT);
  } else if (!strncmp(buf, "[F", buf_size)) {
    return keypress_special(KEYPRESS_END);
  } else if (!strncmp(buf, "[H", buf_size)) {
    return keypress_special(KEYPRESS_HOME);

    /* xterm/gnome */
  } else if (!strncmp(buf, "OA", buf_size)) {
    return keypress_special(KEYPRESS_UP);
  } else if (!strncmp(buf, "OB", buf_size)) {
    return keypress_special(KEYPRESS_DOWN);
  } else if (!strncmp(buf, "OC", buf_size)) {
    return keypress_special(KEYPRESS_RIGHT);
  } else if (!strncmp(buf, "OD", buf_size)) {
    return keypress_special(KEYPRESS_LEFT);
  } else if (!strncmp(buf, "OF", buf_size)) {
    return keypress_special(KEYPRESS_END);
  } else if (!strncmp(buf, "OH", buf_size)) {
    return keypress_special(KEYPRESS_HOME);

    /* xterm/rxvt */
  } else if (!strncmp(buf, "[1~", buf_size)) {
    return keypress_special(KEYPRESS_HOME);
  } else if (!strncmp(buf, "[2~", buf_size)) {
    return keypress_special(KEYPRESS_INSERT);
  } else if (!strncmp(buf, "[3~", buf_size)) {
    return keypress_special(KEYPRESS_DELETE);
  } else if (!strncmp(buf, "[4~", buf_size)) {
    return keypress_special(KEYPRESS_END);
  } else if (!strncmp(buf, "[5~", buf_size)) {
    return keypress_special(KEYPRESS_PAGEUP);
  } else if (!strncmp(buf, "[6~", buf_size)) {
    return keypress_special(KEYPRESS_PAGEDOWN);

    /* putty */
  } else if (!strncmp(buf, "[[5~", buf_size)) {
    return keypress_special(KEYPRESS_PAGEUP);
  } else if (!strncmp(buf, "[[6~", buf_size)) {
    return keypress_special(KEYPRESS_PAGEDOWN);

    /* rxvt */
  } else if (!strncmp(buf, "[[7~", buf_size)) {
    return keypress_special(KEYPRESS_HOME);
  } else if (!strncmp(buf, "[[8~", buf_size)) {
    return keypress_special(KEYPRESS_END);

    /* xterm/gnome */
  } else if (!strncmp(buf, "OP", buf_size)) {
    return keypress_special(KEYPRESS_F1);
  } else if (!strncmp(buf, "OQ", buf_size)) {
    return keypress_special(KEYPRESS_F2);
  } else if (!strncmp(buf, "OR", buf_size)) {
    return keypress_special(KEYPRESS_F3);
  } else if (!strncmp(buf, "OS", buf_size)) {
    return keypress_special(KEYPRESS_F4);

    /* common */
  } else if (!strncmp(buf, "[15~", buf_size)) {
    return keypress_special(KEYPRESS_F5);
  } else if (!strncmp(buf, "[17~", buf_size)) {
    return keypress_special(KEYPRESS_F6);
  } else if (!strncmp(buf, "[18~", buf_size)) {
    return keypress_special(KEYPRESS_F7);
  } else if (!strncmp(buf, "[19~", buf_size)) {
    return keypress_special(KEYPRESS_F8);
  } else if (!strncmp(buf, "[20~", buf_size)) {
    return keypress_special(KEYPRESS_F9);
  } else if (!strncmp(buf, "[21~", buf_size)) {
    return keypress_special(KEYPRESS_F10);
  } else if (!strncmp(buf, "[23~", buf_size)) {
    return keypress_special(KEYPRESS_F11);
  } else if (!strncmp(buf, "[24~", buf_size)) {
    return keypress_special(KEYPRESS_F12);

    /* xterm / rxvt */
  } else if (!strncmp(buf, "[11~", buf_size)) {
    return keypress_special(KEYPRESS_F1);
  } else if (!strncmp(buf, "[12~", buf_size)) {
    return keypress_special(KEYPRESS_F2);
  } else if (!strncmp(buf, "[13~", buf_size)) {
    return keypress_special(KEYPRESS_F3);
  } else if (!strncmp(buf, "[14~", buf_size)) {
    return keypress_special(KEYPRESS_F4);

  } else if (strlen(buf) == 0) {
    return keypress_special(KEYPRESS_ESCAPE);

  } else {
    return keypress_utf8(buf - 1);
  }

  return keypress_special(KEYPRESS_UNKNOWN);
}

keypress_key_t keypress_read(int block) {

  char buf[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  struct termios old = { 0 };
  int flags = fcntl(0, F_GETFL, 0);

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

  if (! block) {
    fcntl(0, F_SETFL, flags | O_NONBLOCK);
  }

  if (read(0, buf, 1) < 0) {
    fcntl(0, F_SETFL, flags);
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    tcsetattr(0, TCSADRAIN, &old);
    if (block) {
      error("Cannot read key");
    } else {
      return keypress_special(KEYPRESS_NONE);
    }
  }

  /* If an escape sequence, then we read the rest.
     We cannot read everything in one go, because of
     copy-and-paste. This way for a copy-and-paste
     we only read the first character. */

  if (buf[0] == '\033') {
    /* At least two more characters are needed. We do a non-blocking
       read to detect if the user only pressed ESC */
    ssize_t chars = 0;
    fcntl(0, F_SETFL, flags | O_NONBLOCK);
    chars = read(0, buf + 1, 2);
    fcntl(0, F_SETFL, flags);
    if (chars == 2 && buf[1] == '[' && buf[2] >= '1' && buf[2] <= '6') {
      /* A third one is needed, too */
      read(0, buf + 3, 1);
      if (buf[3] >= '0' && buf[3] <= '9') {
	/* A fourth one is needed, too */
	read(0, buf + 4, 1);
      }
    } else if (chars == 2 && buf[1] == '[' && buf[2] == '[') {
      /* Two more is needed if it starts with [[ */
      read(0, buf + 3, 2);
    }
  }

  /* For UTF8 characters, we might need to read more bytes
     We don't handle errors here, if there are not enough bytes
     to read, we'll just return whatever is available. */
  if ((buf[0] & 0x80) == 0) {
    /* Nothing to do */
  } else if ((buf[0] & 0xe0) == 0xc0) {
    read(0, &buf[1], 1);
  } else if ((buf[0] & 0xf0) == 0xe0) {
    read(0, &buf[1], 2);
  } else if ((buf[0] & 0xf8) == 0xf0) {
    read(0, &buf[1], 3);
  }

  fcntl(0, F_SETFL, flags);

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
    return single_char(buf);
  }
}

#endif
