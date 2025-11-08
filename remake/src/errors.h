#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <stdarg.h>

// Obligé d'avoir une version sans paramètres variable, car autrement si on ne passe qu'un seul
// paramètre il y aurait une "," :
// kd_error_format("texte") => kd_set_error(1, "main.c", "main", "details blabla", )
// Et je n'ai pas trouvé (à ce jour) de solution standard à ce problème
// https://qastack.fr/programming/5588855/standard-alternative-to-gccs-va-args-trick
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0306r2.html
#define     kd_error(details)               kd_set_error(__LINE__, __FILE__, __FUNCTION__, details)
#define     kd_error_format(details, ...)   kd_set_error(__LINE__, __FILE__, __FUNCTION__, details, __VA_ARGS__)
#define     KD_ERROR_MAX_LENGTH             1024

void kd_set_error(int line, const char *file, const char *function, const char *details, ...);

#endif // __ERRORS_H__
