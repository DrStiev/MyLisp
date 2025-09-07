#include "types.h"

// construct a pointer to a new Number lval
lval *lval_num(long x) {
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_NUM;
    v->num = x;
    return v;
}

// construct a pointer to a new Symbol lval
lval *lval_sym(char *s) {
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_SYM;
    v->sym = malloc(strlen(s) + 1);
    strcpy(v->sym, s);
    return v;
}

lval *lval_fun(lbuiltin func) {
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_FUN;
    v->fun = func;
    return v;
}

// construct a pointer to a new Sexpr lval
lval *lval_sexpr(void) {
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_SEXPR;
    v->count = 0;
    v->cell = NULL;
    return v;
}

// construct a pointer to a new Qexpr lval
lval *lval_qexpr(void) {
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_QEXPR;
    v->count = 0;
    v->cell = NULL;
    return v;
}

// construct a pointer to a new Error lval
lval *lval_err(char *fmt, ...) {
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_ERR;
    // Create  a va list and initialize it
    va_list va;
    va_start(va, fmt);
    // Allovate 512 bytes of space
    v->err = malloc(512);
    // Print the error string with a maximum of 511 characters
    vsnprintf(v->err, 511, fmt, va);
    // Reallocate to number of bytes actually used
    v->err = realloc(v->err, strlen(v->err) + 1);
    // Cleanup our va list
    va_end(va);
    return v;
}

lval *lval_copy(lval *v) {
    lval *x = malloc(sizeof(lval));
    x->type = v->type;

    switch (v->type) {
        // Copy Functions and Numbers Directly
        case LVAL_FUN: x->fun = v->fun; break;
        case LVAL_NUM: x->num = v->num; break;

        // Copy Strings using maloc and strcpy
        case LVAL_ERR:
            x->err = malloc(strlen(v->err) + 1);
            strcpy(x->err, v->err);
            break;
        case LVAL_SYM:
            x->sym = malloc(strlen(v->sym) + 1);
            strcpy(x->sym, v->sym);
            break;

        // Copy Lists by copying each sub-expression
        case LVAL_SEXPR:
        case LVAL_QEXPR:
            x->count = v->count;
            x->cell = malloc(sizeof(lval *) * x->count);
            for (int i = 0; i < x->count; i++) {
                x->cell[i] = lval_copy(v->cell[i]);
            }
            break;
    }
    return x;
}

lval *lval_add(lval *v, lval *x) {
    v->count++;
    v->cell = realloc(v->cell, sizeof(lval *) * v->count);
    v->cell[v->count - 1] = x;
    return v;
}

lval *lval_pop(lval *v, int i) {
    // find the item at i
    lval *x = v->cell[i];
    // shift memory after the item at i over the top
    memmove(&v->cell[i], &v->cell[i + 1], sizeof(lval *) * (v->count - i - 1));
    // decrease the count of items in the list
    v->count--;
    // reallocate the memory used
    v->cell = realloc(v->cell, sizeof(lval *) * v->count);
    return x;
}

lval *lval_take(lval *v, int i) {
    lval *x = lval_pop(v, i);
    lval_del(v);
    return x;
}

// first check all arguemtns are Q-Expression then join them togheter one by one
lval *lval_join(lval *x, lval *y) {
    // for each cell in 'y' add it to 'x'
    while (y->count) { x = lval_add(x, lval_pop(y, 0)); }

    // delete the empty 'y' and return 'x'
    lval_del(y);
    return x;
}

// special function to delete lval*
void lval_del(lval *v) {
    switch (v->type) {
        // Do nothing special for number type
        case LVAL_NUM: break;

        // for Err or Sym free the string data
        case LVAL_ERR: free(v->err); break;
        case LVAL_SYM: free(v->sym); break;
        case LVAL_FUN: break;

        // if Qexpr or Sexpr then delete all elements inside
        case LVAL_QEXPR:
        case LVAL_SEXPR:
            for (int i = 0; i < v->count; i++) { lval_del(v->cell[i]); }
            // also free memory allocated to contain the pointers
            free(v->cell);
    }
    // free the memory allocated for the lval struct itself
    free(v);
}

char *ltype_name(int t) {
    switch (t) {
        case LVAL_FUN: return "Function";
        case LVAL_NUM: return "Number";
        case LVAL_ERR: return "Error";
        case LVAL_SYM: return "Symbol";
        case LVAL_SEXPR: return "S-Expression";
        case LVAL_QEXPR: return "Q-Expression";
        default: return "Unknown";
    }
}
