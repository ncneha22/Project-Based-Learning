#include <stdio.h>

void log_execution(const char *message)
{
    FILE *file = fopen("logs/execution.log", "a");

    if (file == NULL)
    {
        printf("Error: Could not open execution log.\n");
        return;
    }

    fprintf(file, "[INFO] %s\n", message);

    fclose(file);
}

void log_error(const char *message)
{
    FILE *file = fopen("logs/error.log", "a");

    if (file == NULL)
    {
        printf("Error: Could not open error log.\n");
        return;
    }

    fprintf(file, "[ERROR] %s\n", message);

    fclose(file);
}

int main()
{
    printf("Logger started.\n");

    log_execution("Logger started successfully.");
    log_execution("Process execution completed.");

    log_error("Example error message.");

    return 0;
}
