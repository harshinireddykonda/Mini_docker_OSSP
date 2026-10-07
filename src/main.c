#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/shell.h"
#include "../include/input.h"
#include "../include/parser.h"
#include "../include/process.h"
#include "../include/builtin.h"
#include "../include/signals.h"
#include "../include/pipes.h"
#include "../include/redirect.h"

int main(void)
{
    char *line;
    char **tokens;

    /* Initialize signal handling */
    initialize_signals();

    while (1)
    {
        /* Display shell prompt */
        printf("myshell> ");
        fflush(stdout);

        /* Read command */
        line = read_line();

        if (line == NULL)
        {
            printf("\n");
            break;
        }

        /* Ignore empty commands */
        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        /*
         * Handle pipe commands
         * Example:
         * ls | wc
         */
        if (strchr(line, '|') != NULL)
        {
            char *left;
            char *right;
            char **tokens1;
            char **tokens2;

            left = strtok(line, "|");
            right = strtok(NULL, "|");

            if (left == NULL || right == NULL)
            {
                printf("Invalid pipe command\n");
                free(line);
                continue;
            }

            tokens1 = parse_line(left);
            tokens2 = parse_line(right);

            if (tokens1 != NULL && tokens2 != NULL)
            {
                execute_pipe(tokens1, tokens2);
            }

            free_tokens(tokens1);
            free_tokens(tokens2);

            free(line);
            continue;
        }

        /*
         * Handle exit command
         */
        if (strcmp(line, "exit") == 0)
        {
            free(line);
            break;
        }

        /*
         * Parse normal command
         */
        tokens = parse_line(line);

        if (tokens == NULL)
        {
            free(line);
            continue;
        }

        /*
         * First check built-in commands.
         * If it is not a built-in, check for redirection.
         * If there is no redirection, execute normally.
         */
        if (execute_builtin(tokens) == 0)
        {
            if (execute_redirection(tokens) == 0)
            {
                execute(tokens);
            }
        }

        /* Free allocated memory */
        free_tokens(tokens);
        free(line);
    }

    return 0;
}
