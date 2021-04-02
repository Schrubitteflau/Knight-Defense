#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "ennemy.h"
#include "error.h"
#include "way.h"


PROJECT_ENNEMY* pr_create_ennemy(ALLEGRO_BITMAP *bitmaps[4], PROJECT_WAY *way)
{
	int i;
	PROJECT_ENNEMY *ennemy;

	ennemy = (PROJECT_ENNEMY*)malloc(sizeof(PROJECT_ENNEMY));
	if (ennemy == NULL)
	{
		pr_error("malloc()");
		return NULL;
	}

	for (i = 0; i < 4; i++)
		ennemy->mvt[i] = bitmaps[i];

	ennemy->current_image = 0;
	ennemy->way = way;
	ennemy->way_index = 0;

	ennemy->width = al_get_bitmap_width(ennemy->mvt[0]);
	ennemy->height = al_get_bitmap_height(ennemy->mvt[0]);

	ennemy->max_life = 200;
	ennemy->life = ennemy->max_life;

	return ennemy;
}


void pr_ennemy_change_image(PROJECT_ENNEMY *ennemy)
{
	if (ennemy->current_image == 3)
		ennemy->current_image = 0;
	else
		ennemy->current_image++;
}


void pr_print_life_bar(PROJECT_ENNEMY *ennemy)
{
	PROJECT_COORD position = ennemy->way->all_points[ennemy->way_index];
	int separate = (float)ennemy->life / ennemy->max_life * ennemy->width;
	
	position.y += ennemy->height + 10;

	al_draw_filled_rectangle(position.x, position.y, position.x + separate, position.y + 5, RED);
}



/*PROJECT_ENNEMY** pr_add_new_ennemy(PROJECT_ENNEMY** tab, int *size, PROJECT_WAY* way, ALLEGRO_BITMAP *anim_bmps[4])
{
	PROJECT_ENNEMY **temp = NULL;
	PROJECT_BOOL ennemy_added = FALSE;
	int i;

	for (i = 0; i < *size; i++)
	{
		if (tab[i]->life <= 0)
		{
			// Si un ennemi est mort ou qu'il a atteint son objectif, alors on le recycle
			// On lui remet sa vie à fond
			//tab[i]->life = tab[i]->max_life;
			// On restaure sa position sur le chemin à 0
			//tab[i]->way_index = 0;
			// Et on lui attribut un nouveau chemin
			//tab[i]->way = way;

			free(tab[i]);
			tab[i] == pr_create_ennemy(anim_bmps, way);
			if (tab[i] == NULL)
				pr_exit_with_last_error();
			ennemy_added = TRUE;
			break;
		}
	}


	if (!ennemy_added)
	{
		(*size)++;

		temp = (PROJECT_ENNEMY**)realloc(tab, (*size) * sizeof(PROJECT_ENNEMY*));
		if (temp == NULL)
		{
			pr_error("realloc()");
			return NULL;
		}
		else
		{
			tab = temp;
			tab[*size - 1] = pr_create_ennemy(anim_bmps, way);
		}
	}

	// On nettoie le tableau des ennemis non valables
	//tab = pr_clean_tab(tab, size);

	return tab;
}*/



/*PROJECT_ENNEMY** pr_clean_tab(PROJECT_ENNEMY **tab, int *size_tab)
{
	int i;
	int pointeurs_valables = 0;
	PROJECT_ENNEMY** new_tab;

	// On parcours le tableau entier et on libère la mémoire des ennemis plus valables
	for (i = 0; i < *size_tab; i++)
	{
		// Pour chaque ennemi du tableau, si celui-ci est mort ou qu'il est arrivé à sa destination, alors
		// on libère sa mémoire et on écrit la valeur NULL à la place de son pointeur
		if (tab[i]->life <= 0 || tab[i]->way_index == tab[i]->way->size)
		{
			free(tab[i]);
			tab[i] = NULL;
		}
		// Sinon, on incrémente le nombre de pointeurs valables
		else
			pointeurs_valables++;
	}

	// On peut donc désormais allouer de la mémoire en fonction du nombre exact d'ennemis valables
	new_tab = (PROJECT_ENNEMY**)malloc((pointeurs_valables + 1) * sizeof(PROJECT_ENNEMY*));
	if (new_tab == NULL)
	{
		pr_error("malloc()");
		return NULL;
	}

	// Puis on copie tous les pointeurs valables dans le nouveau tableau
	for (i = 0; i < *size_tab; i++)
	{
		// Si le pointeur est valables, on le copie, sinon on passe au suivant
		if (tab[i] != NULL)
			new_tab[i] = tab[i];
	}

	// Maintenant, on libère la mémoire du permier tableau et on retourne celui fraîchement créé

	free(tab);

	return new_tab;
}*/