
#pragma once

#include <stdio.h>

#include "../types/types.h"

void lval_print(lval *v);
void lval_println(lval *v);
void lval_expr_print(lval *v, char open, char close);