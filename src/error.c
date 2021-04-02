#include <stdio.h>
#include <stdlib.h>

#include "error.h"


PROJECT_ERROR* pr_get_err_stream(void)
{
	static PROJECT_ERROR err = { .line = 0, .file = NULL, .function = NULL, .details = NULL };
	return &err;
}


void pr_set_error(const int line, char *file, char *function, char *details)
{
	PROJECT_ERROR *current_err;

	current_err = pr_get_err_stream();

	current_err->line = line;
	current_err->file = file;
	current_err->function = function;
	current_err->details = details;
}


void pr_exit_with_last_error(void)
{ 
	PROJECT_ERROR *err = pr_get_err_stream();
	FILE *ferror;

	fopen_s(&ferror, "errors.log", "a");
	if (ferror != NULL)
		fprintf(ferror, "Erreur, ligne %d, dans la fonction %s, du fichier \"%s\"\nDetails : %s\n", err->line, err->function, err->file, err->details);

	fprintf(stderr, "Erreur, ligne %d, dans la fonction %s, du fichier \"%s\"\nDetails : %s\n", err->line, err->function, err->file, err->details);
	fprintf(stderr, "Appuyez sur une touche pour terminer le programme.");
	system("pause > nul");
	exit(EXIT_FAILURE);
}