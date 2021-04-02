#ifndef __ENNEMY_H__
#define __ENNEMY_H__

#include <allegro5/allegro.h>

#include "base.h"
#include "way.h"

// Fr�quence de mouvement de l'ennemi, fr�quence d'incr�mentation de sa position sur son chemin
#define			ENNEMYRATE			(double)1.0 / 30

typedef struct PROJECT_ENNEMY
{
	ALLEGRO_BITMAP *mvt[4];			// Tableau de pointeurs d'images de l'ennemi pour les animations
	int current_image;				// Index du tableau ci-dessus donnant l'image � afficher
	int width, height;				// Pour les collisions
	PROJECT_WAY *way;				// Pointeur sur le chemin actuellement suivi par l'ennemi
	int way_index;					// Position sur ce chemin
	int life, max_life;				// Nombre de points de vie
} PROJECT_ENNEMY;


// Prototypes
PROJECT_ENNEMY* pr_create_ennemy(ALLEGRO_BITMAP *bitmaps[4], PROJECT_WAY *way);
void pr_ennemy_change_image(PROJECT_ENNEMY *ennemy);
void pr_print_life_bar(PROJECT_ENNEMY *ennemy);
//PROJECT_ENNEMY** pr_add_new_ennemy(PROJECT_ENNEMY** tab, int *size, PROJECT_WAY* way, ALLEGRO_BITMAP *anim_bmps[4]);
//PROJECT_ENNEMY** pr_clean_tab(PROJECT_ENNEMY **tab, int *size_tab);

#endif