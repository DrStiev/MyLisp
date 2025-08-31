
#pragma once

#include <stdio.h>

// with Lispy an expression will evaluate to either a number or an error

// enumeration of possible lval types
enum { LVAL_NUM, LVAL_ERR };

// enumeration of possible error types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

typedef struct {
    int type;
    long num;
    int err;
} lval;

// print lval
void lval_print(lval v);
void lval_println(lval v);