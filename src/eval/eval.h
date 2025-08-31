#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../parser/mpc.h"
#include "../types/types.h"

lval *lval_read_num(mpc_ast_t *t);
lval *lval_add(lval *v, lval *x);
lval *lval_read(mpc_ast_t *t);

lval *lval_eval(lval *v);
lval *lval_eval_sexpr(lval *v);