#ifndef __FIREBALL_H__
#define __FIREBALL_H__

#include <time.h>

#include "base.h"
#include "hero.h"
#include "error.h"

#define			FIRERATE			(double)1.0 / 300

typedef struct PROJECT_FIREBALL
{
	ALLEGRO_BITMAP *image;				// Un pointeur sur son image
	PROJECT_COORD position;				// Sa position actuelle
	PROJECT_COORD *trajectory;			// Ensemble de points qui définissent sa trajectoire
	int trajectory_size;
	PROJECT_BOOL is_active;				// Si la boule de feu n'est pas active, elle n'est pas affichée
										// et il n'y a pas de collision
	int width, height;					// Taille de l'image
	int range;							// Portée en pixels
	int damages;						// Dégâts que la boule de feu inflige
} PROJECT_FIREBALL;

// Prototypes
PROJECT_FIREBALL *pr_create_fireball(const char *image);
void pr_move_fireball(PROJECT_FIREBALL *fireball);
void pr_free_fireball(PROJECT_FIREBALL *fireball);
clock_t pr_fireball_attack(PROJECT_FIREBALL* fireball, PROJECT_HERO* hero, PROJECT_COORD mouse);


#endif