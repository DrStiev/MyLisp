#include "builtin.h"

// Builtin

lval *builtin_op(lenv *e, lval *a, char *op) {
    // ensure all arguments are numbers
    for (int i = 0; i < a->count; i++) { LASSERT_TYPE(op, a, i, LVAL_NUM); }

    // pop the first element
    lval *x = lval_pop(a, 0);

    // if no arguments and sub then perform unary negation
    if ((strcmp(op, "-") == 0) && a->count == 0) { x->num = -x->num; }

    // while there are still elemtns remaining
    while (a->count > 0) {
        // pop the next element
        lval *y = lval_pop(a, 0);

        if (strcmp(op, "+") == 0) { x->num += y->num; }
        if (strcmp(op, "-") == 0) { x->num -= y->num; }
        if (strcmp(op, "*") == 0) { x->num *= y->num; }
        if (strcmp(op, "/") == 0) {
            if (y->num == 0) {
                lval_del(x);
                lval_del(y);
                x = lval_err("Division by Zero!");
                break;
            }
            x->num /= y->num;
        }
        lval_del(y);
    }
    lval_del(a);
    return x;
}

lval *builtin_add(lenv *e, lval *a) { return builtin_op(e, a, "+"); }

lval *builtin_sub(lenv *e, lval *a) { return builtin_op(e, a, "-"); }

lval *builtin_mul(lenv *e, lval *a) { return builtin_op(e, a, "*"); }

lval *builtin_div(lenv *e, lval *a) { return builtin_op(e, a, "/"); }

lval *builtin_def(lenv *e, lval *a) {
    LASSERT_TYPE("def", a, 0, LVAL_QEXPR);
    // First argument is symbol list
    lval *syms = a->cell[0];
    // Ensure all  elements of fisrt list are symbols
    for (int i = 0; i < syms->count; i++) {
        LASSERT(a, (syms->cell[i]->type == LVAL_SYM),
                "Function 'def' cannot define non-symbol. Got %s, Expected %s.",
                ltype_name(syms->cell[i]->type), ltype_name(LVAL_SYM));
    }
    //  Check correct number of symbols and values
    LASSERT(a, (syms->count == a->count - 1),
            "Function 'def' passed too many arguments for symbols. Got %i, "
            "Expected %i.",
            syms->count, a->count - 1);
    // Assign copies of values to symbols
    for (int i = 0; i < syms->count; i++) {
        lenv_put(e, syms->cell[i], a->cell[i + 1]);
    }
    lval_del(a);
    return lval_sexpr();
}

// can repearedly pop and delete item at index 1 until there is nothing else
// left in the list
lval *builtin_head(lenv *e, lval *a) {
    // check error conditions
    LASSERT_NUM("head", a, 1);
    LASSERT_TYPE("head", a, 0, LVAL_QEXPR);
    LASSERT_NOT_EMPTY("head", a, 0);

    lval *v = lval_take(a, 0);
    while (v->count > 1) { lval_del(lval_pop(v, 1)); }
    return v;
}

// can pop and delete the item at index 0, leaving the tail remaining.
lval *builtin_tail(lenv *e, lval *a) {
    // check error conditions
    LASSERT_NUM("tail", a, 1);
    LASSERT_TYPE("tail", a, 0, LVAL_QEXPR);
    LASSERT_NOT_EMPTY("tail", a, 0);

    lval *v = lval_take(a, 0);
    lval_del(lval_pop(v, 0));
    return v;
}

// list function converts the input S-Expression to a Q-Expression and returns
// it
lval *builtin_list(lenv *e, lval *a) {
    a->type = LVAL_QEXPR;
    return a;
}

lval *builtin_eval(lenv *e, lval *a) {
    LASSERT_NUM("eval", a, 1);
    LASSERT_TYPE("eval", a, 0, LVAL_QEXPR);

    lval *x = lval_take(a, 0);
    x->type = LVAL_SEXPR;
    return lval_eval(e, x);
}

lval *builtin_join(lenv *e, lval *a) {
    for (int i = 0; i < a->count; i++) {
        LASSERT_TYPE("join", a, i, LVAL_QEXPR);
    }

    lval *x = lval_pop(a, 0);
    while (a->count) { x = lval_join(x, lval_pop(a, 0)); }
    lval_del(a);
    return x;
}

void lenv_add_builtin(lenv *e, char *name, lbuiltin func) {
    lval *k = lval_sym(name);
    lval *v = lval_fun(func);
    lenv_put(e, k, v);
    lval_del(k);
    lval_del(v);
}

// need a function that can call the correct builtin function depending on what
// symbol it encounters in evaluation
void lenv_add_builtins(lenv *e) {
    // Variable Functions
    lenv_add_builtin(e, "def", builtin_def);

    /* List Functions */
    lenv_add_builtin(e, "list", builtin_list);
    lenv_add_builtin(e, "head", builtin_head);
    lenv_add_builtin(e, "tail", builtin_tail);
    lenv_add_builtin(e, "eval", builtin_eval);
    lenv_add_builtin(e, "join", builtin_join);

    /* Mathematical Functions */
    lenv_add_builtin(e, "+", builtin_add);
    lenv_add_builtin(e, "-", builtin_sub);
    lenv_add_builtin(e, "*", builtin_mul);
    lenv_add_builtin(e, "/", builtin_div);
}