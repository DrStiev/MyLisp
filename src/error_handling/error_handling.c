#include "error_handling.h"

void lval_print(lval v) {
    switch (v.type) {
        // the type is a number: print it
        case LVAL_NUM:
            printf("%li", v.num);
            break;

        // the type is an error
        case LVAL_ERR:
            // check type of error
            if (v.err == LERR_DIV_ZERO) {
                printf("Error: Division by Zero!");
            }
            if (v.err == LERR_BAD_OP) {
                printf("Error: Invalid Operator!");
            }
            if (v.err == LERR_BAD_NUM) {
                printf("Error: Invalid Number!");
            }
            break;
    }
}

void lval_println(lval v) {
    lval_print(v);
    putchar('\n');
}