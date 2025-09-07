#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../parser/mpc.h"
#include "../types/types.h"

struct lenv {
    int count;
    char **syms;
    lval **vals;
};

lenv *lenv_new(void);
void lenv_del(lenv *e);

lval *lenv_get(lenv *e, lval *k);
void lenv_put(lenv *e, lval *k, lval *v);
