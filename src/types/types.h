#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;

// with Lispy an expression will evaluate to either a number or an error

// enumeration of possible lval types
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

// enumeration of possible error types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

typedef lval *(*lbuiltin)(lenv *, lval *);

typedef struct lval {
    int type;
    long num;
    // Error and Symbol types have some string data
    char *err;
    char *sym;
    lbuiltin fun;
    // Count and Pointer to a list of "lval"
    int count;
    struct lval **cell;
} lval;

char *ltype_name(int t);

lval *lval_num(long x);
lval *lval_sym(char *s);
lval *lval_fun(lbuiltin func);
lval *lval_sexpr(void);
lval *lval_qexpr(void);
lval *lval_err(char *fmt, ...);

lval *lval_copy(lval *v);
lval *lval_add(lval *v, lval *x);
lval *lval_pop(lval *v, int i);
lval *lval_take(lval *v, int i);
lval *lval_join(lval *x, lval *y);

void lval_del(lval *v);