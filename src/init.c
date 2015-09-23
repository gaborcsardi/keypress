
#include <Rinternals.h>

#include "keypress.h"

void R_init_keypress(DllInfo *info) {
  R_RegisterCCallable("keypress", "keypress", (DL_FUNC) keypress);
}
