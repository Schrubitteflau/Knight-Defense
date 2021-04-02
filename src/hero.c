#include <stdarg.h>
#include <stdio.h>

#include "hero.h"
#include "error.h"
#include "display.h"

// Créé un personnage de type héro qui sera contrôlé par l'utilisateur
PROJECT_HERO* pr_create_hero(void)
{
	PROJECT_HERO *hero;
	char filename[256];
	int i, j;

	hero = (PROJECT_HERO*)malloc(sizeof(PROJECT_HERO));
	if (hero == NULL)
	{
		pr_error("malloc()");
		return NULL;
	}

	hero->position = (PROJECT_COORD){ 610, 310 };
	hero->attack = HERO_IS_NOT_ATTACKING;
	hero->speed = 1.0f;

	hero->attack_mode = ATTACK_MODE_CAC;
	hero->cac_damages = 90;

	for (i = 0; i < 4; i++)
	{
		sprintf_s(filename, 256, "resources/images/mvt%d.png", i + 1);
		hero->mvt[i] = al_load_bitmap(filename);
		
		if (hero->mvt[i] == NULL)
		{
			pr_error("al_load_bitmap()");

			for (j = 0; j < i; j++)
				al_destroy_bitmap(hero->mvt[j]);

			free(hero);

			return NULL;
		}
	}

	for (i = 0; i < 4; i++)
	{
		sprintf_s(filename, 256, "resources/images/atq%d.png", i + 1);
		hero->atq[i] = al_load_bitmap(filename);

		if (hero->atq[i] == NULL)
		{
			pr_error("al_load_bitmap()");

			for (j = 0; j < 4; j++)
				al_destroy_bitmap(hero->mvt[j]);

			for (int j = 0; j < i; j++)
				al_destroy_bitmap(hero->atq[j]);

			free(hero);

			return NULL;
		}
	}

	hero->width = al_get_bitmap_width(hero->mvt[0]);
	hero->height = al_get_bitmap_height(hero->mvt[0]);

	hero->current_image = hero->mvt[0];

	return hero;
}


// Déplace le héro en fonction des touches du clavier pressées
void pr_move_hero(PROJECT_BOOL key[KEY_MAX], PROJECT_HERO *hero)
{
	PROJECT_COORD min, max;
	int current_speed = 4;

	min = (PROJECT_COORD){ 0, 0 };
	max = (PROJECT_COORD){ 1920 - hero->width, 1080 - hero->height };

		if (key[KEY_UP] || key[KEY_DOWN])
			if (key[KEY_LEFT] || key[KEY_RIGHT])
				current_speed = 3;

	hero->position.y -= (int)(key[KEY_UP] * current_speed * hero->speed);
	hero->position.y += (int)(key[KEY_DOWN] * current_speed * hero->speed);
	hero->position.x += (int)(key[KEY_RIGHT] * current_speed * hero->speed);
	hero->position.x -= (int)(key[KEY_LEFT] * current_speed * hero->speed);

	if (hero->position.x > max.x)
		hero->position.x = max.x;

	if (hero->position.x < min.x)
		hero->position.x = min.x;

	if (hero->position.y > max.y)
		hero->position.y = max.y;

	if (hero->position.y < min.y)
		hero->position.y = min.y;
}


// Change l'image actuelle du héro afin de faire une animation
void pr_hero_change_image(PROJECT_HERO *hero, int specified_value)
{
	static int inc = 1, index = 0;
	static int compt = 0;
	static PROJECT_COORD last_coords;
	// On sauvegardera les coordonnées du personnage au moment de l'appel
	// de la fonction pour les comparer aux nouvelles : et donc si elles
	// sont différentes on peut déclencher l'animation de mouvement

	// Pour modifier l'index directement en cas de besoin
	if (specified_value != -1)
		index = specified_value;

	if (compt == 7)
	{
		compt = 0;
		hero->attack = HERO_IS_NOT_ATTACKING;
	}

	if (index == 0)
		inc = 1;
	else if (index == 3)
		inc = -1;

	// Si le héro est en train d'attaquer, alors on utilise le tableau d'images
	// correspondant à l'animation d'attaque
	if (hero->attack)
	{
		hero->current_image = hero->atq[index];
		compt++;	// Compteur pour pouvoir stopper l'animation d'attaque au bon moment
	}
	// Si le personnage a bougé alors on change son image, sinon il garde celle d'avant
	else if (last_coords.x != hero->position.x || last_coords.y != hero->position.y)
		hero->current_image = hero->mvt[index];

	last_coords = hero->position;

	index += inc;
}


// Libère la mémoire allouée par le héro ainsi que les images qui lui sont associées
void pr_free_hero(PROJECT_HERO *hero)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		if (hero->mvt[i] != NULL)
			al_destroy_bitmap(hero->mvt[i]);
		if (hero->atq[i] != NULL)
			al_destroy_bitmap(hero->atq[i]);
	}

	free(hero);
}