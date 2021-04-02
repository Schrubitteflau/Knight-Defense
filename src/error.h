#ifndef __ERROR_H__
#define __ERROR_H__


// Pour appeler la fonction de gestion d'erreurs		
#define		pr_error(details)	pr_set_error(__LINE__, __FILE__, __FUNCTION__, details)


typedef struct PROJECT_ERROR
{
	int line;
	char *file, *function, *details;
} PROJECT_ERROR;


void pr_set_error(const int line, char *file, char *function, char *details);
PROJECT_ERROR* pr_get_err_stream(void);
void pr_exit_with_last_error(void);

#endif