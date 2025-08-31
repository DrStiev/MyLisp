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
lval *lval_err(char *m) {
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_ERR;
    v->err = malloc(strlen(m) + 1);
    strcpy(v->err, m);
    return v;
}

// special function to delete lval*
void lval_del(lval *v) {
    switch (v->type) {
        // Do nothing special for number type
        case LVAL_NUM: break;

        // for Err or Sym free the string data
        case LVAL_ERR: free(v->err); break;
        case LVAL_SYM: free(v->sym); break;

        // if Qexpr or Sexpr then delete all elements inside
        case LVAL_QEXPR:
        case LVAL_SEXPR:
            for (int i = 0; i < v->count; i++) {
                lval_del(v->cell[i]);
            }
            // also free memory allocated to contain the pointers
            free(v->cell);
    }
    // free the memory allocated for the lval struct itself
    free(v);
}