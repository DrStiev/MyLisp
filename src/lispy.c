#include <stdio.h>
#include <stdlib.h>

#include "error_handling/error_handling.h"
#include "parser/mpc.h"

// new number type lval
lval lval_num(long x) {
    lval v;
    v.type = LVAL_NUM;
    v.num = x;
    return v;
}

// new error type lval
lval lval_err(int x) {
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

// use operator string to see which operation to perform
lval eval_op(lval x, char *op, lval y) {
    // if either value is an error return it
    if (x.type == LVAL_ERR) {
        return x;
    }
    if (y.type == LVAL_ERR) {
        return y;
    }

    // otherwise do maths on the number values
    if (strcmp(op, "+") == 0) {
        return lval_num(x.num + y.num);
    }
    if (strcmp(op, "-") == 0) {
        return lval_num(x.num - y.num);
    }
    if (strcmp(op, "*") == 0) {
        return lval_num(x.num * y.num);
    }
    if (strcmp(op, "/") == 0) {
        // if second operando is zero return error
        return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
    }

    return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t *t) {
    // if tagged as number return it directly
    if (strstr(t->tag, "number")) {
        // check if there is some error in conversion
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
    }

    // the operator is always second child
    char *op = t->children[1]->contents;
    lval x = eval(t->children[2]);

    // iterate the remaining children and combining them
    int i = 3;
    while (strstr(t->children[i]->tag, "expr")) {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }

    return x;
}

// if we are compiling on windows compile these functions
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

// fake readline function
char *readline(char *prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char *cpy = malloc(strlen(buffer) + 1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

// fake add_history function
void add_history(char *unused) {
}

// otherwise include editline headers
#else

// the library editline provides two useful functions:
// 1. readline to read user input from some prompt allowing for editing of that
// input
// 2. add_history let record the history of inputs so that can be retrieved with
// the up and down arrows
#include <editline/history.h>
#include <editline/readline.h>
#endif

// Start with an interactive prompt

int main(int argc, char **argv) {
    // use Polish Notation.
    // It's a notation for arithmetic
    // where the operator comes before the operands
    // 1 + 2 + 3 is + 1 2 3
    // 6 + (2 * 9) is + 6 (* 2 9)
    // (10 * 2) / (4 + 2) is / (* 10 2) (+ 4 2)

    // Create a parser
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | '-' | '*' | '/' ;                  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    lispy    : /^/ <operator> <expr>+ /$/ ;             \
  ",
              Number, Operator, Expr, Lispy);

    // print Version and Exit information
    puts("C-Lisp \"Lispy\" Version 0.0.0.0.3");
    puts("Press Ctrl+c to Exit\n");

    while (1) {
        // output our prompt and get input
        char *input = readline("Lispy> ");
        // add input to history
        add_history(input);
        // attempt to parse user input
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            // begin evaluation
            lval result = eval(r.output);
            lval_println(result);
            mpc_ast_delete(r.output);
        } else {
            // otherwise print error
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        // free retrieved input
        free(input);
    }

    // undefine and delete parser
    mpc_cleanup(4, Number, Operator, Expr, Lispy);
    return 0;
}