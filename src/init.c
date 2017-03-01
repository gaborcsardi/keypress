
#include <Rinternals.h>

#include "keypress.h"

static const R_CallMethodDef callMethods[]  = {
  {"keypress", (DL_FUNC) &keypress, 1},
  {NULL, NULL, 0}
};

void R_init_keypress(DllInfo *dll) {
  R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);

  R_RegisterCCallable("keypress", "keypress", (DL_FUNC) keypress);
  R_RegisterCCallable("keypress_read", "keypress_read", (DL_FUNC) keypress_read);
}
