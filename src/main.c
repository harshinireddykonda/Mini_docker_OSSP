#include <stdio.h>
#include <string.h>

int main()
{
    char input[1024];

    printf("=====================================\n");
    printf(" Mini Docker Container Manager\n");
    printf("=====================================\n");

    while (1)
    {
        printf("MiniDocker> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting MiniDocker...\n");
            break;
        }

        printf("You entered: %s\n", input);
    }

    return 0;
}
