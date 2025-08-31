#pragma once

#include <stdlib.h>
#include <string.h>

// with Lispy an expression will evaluate to either a number or an error

// enumeration of possible lval types
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR, LVAL_QEXPR };

// enumeration of possible error types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

typedef struct lval {
    int type;
    long num;
    // Error and Symbol types have some string data
    char *err;
    char *sym;
    // Count and Pointer to a list of "lval"
    int count;
    struct lval **cell;
} lval;

lval *lval_num(long x);
lval *lval_sym(char *s);
lval *lval_sexpr(void);
lval *lval_qexpr(void);
lval *lval_err(char *m);

void lval_del(lval *v);