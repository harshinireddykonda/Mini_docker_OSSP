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
#include "../include/thread.h"

int main(void)
{
    char *line;
    char **tokens;

    initialize_signals();

    /* Start Week 10 background monitoring thread */
    start_monitor_thread();

    while (1)
    {
        printf("myshell> ");
        fflush(stdout);

        line = read_line();

        if (line == NULL)
        {
            printf("\n");
            break;
        }

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        /*
         * Week 8: Pipe handling
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
         * Exit command
         */
        if (strcmp(line, "exit") == 0)
        {
            free(line);
            break;
        }

        tokens = parse_line(line);

        if (tokens == NULL)
        {
            free(line);
            continue;
        }

        /*
         * Built-in commands
         * Redirection
         * External commands
         */
        if (execute_builtin(tokens) == 0)
        {
            if (execute_redirection(tokens) == 0)
            {
                execute(tokens);
            }
        }

        free_tokens(tokens);
        free(line);
    }

    return 0;
}
