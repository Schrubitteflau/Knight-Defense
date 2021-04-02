#ifndef __HERO_H__
#define __HERO_H__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "base.h"

#define			FRB_ATTACK_COOLDOWN			(double)3.0
#define			CAC_ATTACK_COOLDOWN			(double)1.0

#define			HERO_IS_NOT_ATTACKING		0
#define			HERO_IS_ATTACKING_CAC		1
#define			HERO_IS_ATTACKING_FRB		2

#define			ATTACK_MODE_CAC				0
#define			ATTACK_MODE_FRB				1

typedef enum PROJECT_MOVE_KEY { KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_MAX } PROJECT_MOVE_KEY;

typedef struct PROJECT_HERO
{
	ALLEGRO_BITMAP *current_image;		// Pointeur sur la prochaine image du personnage à afficher
	ALLEGRO_BITMAP *mvt[4];				// Images du personnage qui se déplace
	ALLEGRO_BITMAP *atq[4];				// Images du personnage qui attaque
	PROJECT_COORD position;				// Position
	int attack;							// Vaut 0 si le héro n'est pas en train d'attaquer, sinon 1
										// pour une attaque au corps à corps et 2 pour une attaque fireball
	int attack_mode;					// Indique le mode d'attaque actuellement activé
	int width, height;					// Taille en pixel du héro
	int cac_damages;					// Dégâts du héro
	float speed;						// Vitesse : mutliplicateur initialisé à 1.0
} PROJECT_HERO;


// Prototypes

// Créé un personnage de type héro qui sera contrôlé par l'utilisateur
PROJECT_HERO* pr_create_hero(void);
// Déplace le héro en fonction des touches du clavier pressées
void pr_move_hero(PROJECT_BOOL key[KEY_MAX], PROJECT_HERO *hero);
// Change l'image actuelle du héro afin de faire une animation
void pr_hero_change_image(PROJECT_HERO *hero, int specified_value);
// Libère la mémoire allouée par le héro ainsi que les images qui lui sont associées
void pr_free_hero(PROJECT_HERO *hero);


#endif
