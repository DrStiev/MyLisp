#include <stdio.h>
#include <stdlib.h>

// if we are compiling on windows compile these functions
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

// fake readline function
char *readline(char *prompt)
{
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char *cpy = malloc(strlen(buffer) + 1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

// fake add_history function
void add_history(char *unused)
{
}

// otherwise include editline headers
#else

// the library editline provides two useful functions:
// 1. readline to read user input from some prompt allowing for editing of that input
// 2. add_history let record the history of inputs so that can be retrieved with the up and down arrows
#include <editline/history.h>
#include <editline/readline.h>
#endif

// Start with an interactive prompt

int main(int argc, char **argv)
{
    // print Version and Exit information
    puts("C-Lisp \"Lispy\" Version 0.0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

    while (1)
    {
        // output our prompt and get input
        char *input = readline("Lispy> ");
        // add input to history
        add_history(input);
        // echo input back to user
        printf("User text: %s", input);
        // free retrieved input
        free(input);
    }
    return 0;
}