
#ifndef KEYPRESS_INTERNAL_H
#define KEYPRESS_INTERNAL_H

SEXP save_term_status(void);
SEXP restore_term_status(void);
SEXP set_term_echo(SEXP s_echo);

#endif
