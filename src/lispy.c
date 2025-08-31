#include <stdio.h>
#include <stdlib.h>

#include "eval/eval.h"
#include "parser/mpc.h"
#include "print/print.h"
#include "types/types.h"

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
    // Create a parser
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Symbol = mpc_new("symbol");
    mpc_parser_t *Sexpr = mpc_new("sexpr");
    mpc_parser_t *Qexpr = mpc_new("qexpr");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                        \
    number : /-?[0-9]+/ ;                                  \
    symbol : \"list\" | \"head\" | \"tail\"                \
           | \"join\" | \"eval\" | '+' | '-' | '*' | '/' ; \
    sexpr  : '(' <expr>* ')' ;                             \
    qexpr  : '{' <expr>* '}' ;                             \
    expr   : <number> | <symbol> | <sexpr> | <qexpr> ;     \
    lispy  : /^/ <expr>* /$/ ;                             \
  ",
              Number, Symbol, Sexpr, Qexpr, Expr, Lispy);

    // print Version and Exit information
    puts("C-Lisp \"Lispy\" Version 0.0.0.0.6");
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
            lval *x = lval_eval(lval_read(r.output));
            lval_println(x);
            lval_del(x);
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
    mpc_cleanup(6, Number, Symbol, Sexpr, Expr, Lispy);
    return 0;
}