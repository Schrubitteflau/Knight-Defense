#include <stdio.h>
#include <stdarg.h>

#include "errors.h"

void kd_set_error(int line, const char *file, const char *function, const char *details, ...)
{
    FILE *f = fopen("errors.log", "a");
    char error_message[KD_ERROR_MAX_LENGTH];
    va_list args;

    va_start(args, details);
    vsnprintf(error_message, KD_ERROR_MAX_LENGTH - 1, details, args);
    va_end(args);

    // Pas très optimisé car formatte 2 fois mais pas très grave
    fprintf(stderr, "ligne : %d, fonction : %s, fichier : %s\n=> %s",line, function, file, error_message);

    if (f != NULL)
    {
        fprintf(f, "ligne : %d, fonction : %s, fichier : %s\n=> %s",line, function, file, error_message);
        fclose(f);
    }
}
