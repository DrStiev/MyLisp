#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../environment/env.h"
#include "../parser/mpc.h"
#include "../types/types.h"

lval *lval_read_num(mpc_ast_t *t);
lval *lval_read(mpc_ast_t *t);

lval *lval_eval(lenv *e, lval *v);
lval *lval_eval_sexpr(lenv *e, lval *v);
