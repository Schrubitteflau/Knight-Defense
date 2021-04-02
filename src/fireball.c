#include "fireball.h"
#include "hero.h"
#include "display.h"
#include "way.h"


PROJECT_FIREBALL* pr_create_fireball(const char *image)
{
	ALLEGRO_DISPLAY *display;
	PROJECT_FIREBALL *fireball;

	display = al_is_system_installed() ? al_get_current_display() : NULL;
	if (display == NULL)
	{
		pr_error("al_is_system_installed() || al_get_current_display()");
		return NULL;
	}

	fireball = (PROJECT_FIREBALL*)malloc(sizeof(PROJECT_FIREBALL));
	if (fireball == NULL)
	{
		pr_error("malloc()");
		return NULL;
	}

	fireball->image = al_load_bitmap("resources/images/fireball.png");
	if (fireball->image == NULL)
	{
		pr_error("al_load_bitmap()");
		free(fireball);
		return NULL;
	}

	fireball->width = al_get_bitmap_width(fireball->image);
	fireball->height = al_get_bitmap_height(fireball->image);
	
	fireball->position = (PROJECT_COORD){ 0, 0 };
	fireball->is_active = FALSE;
	fireball->range = 300;
	fireball->damages = 2;
	fireball->trajectory = NULL;

	return fireball;
}


void pr_move_fireball(PROJECT_FIREBALL *fireball)
{
	static int i = 0;

	if (i < fireball->trajectory_size)
		fireball->position = fireball->trajectory[i++];
	else
	{
		i = 0;
		fireball->is_active = FALSE;
	}
}


// Libère la mémoire réservée pour la boule de feu
void pr_free_fireball(PROJECT_FIREBALL *fireball)
{
	if (fireball->image != NULL)
		al_destroy_bitmap(fireball->image);
	if (fireball->trajectory != NULL)
		free(fireball->trajectory);
	free(fireball);
}


// Déclenche une attaque de type boule de feu et retourne l'instant précis à laquelle
// cette attaque a été effectuée afin de permettre un "cooldown" ou un temps de rechargement
clock_t pr_fireball_attack(PROJECT_FIREBALL *fireball, PROJECT_HERO *hero, PROJECT_COORD mouse)
{
	if (fireball->trajectory != NULL)
		free(fireball->trajectory);

	// Si la souris est à la gauche du personnage
	if (mouse.x < hero->position.x + hero->width / 2)
	{
		fireball->trajectory = pr_get_all_points_between(
		(PROJECT_COORD) { hero->position.x + hero->width / 2, hero->position.y + hero->height / 2 },
		pr_get_point_direction_with_range(
		(PROJECT_COORD) { hero->position.x + hero->width / 2, hero->position.y + hero->height / 2 },
		(PROJECT_COORD) { mouse.x - fireball->width / 2, mouse.y - fireball->height / 2 },
		fireball->range), &(fireball->trajectory_size)
		);
	}
	// Le héros est orienté vers la droite
	else
	{
		fireball->trajectory = pr_get_all_points_between(
		(PROJECT_COORD) { hero->position.x + hero->width / 2, hero->position.y + hero->height / 2 },
		pr_get_point_direction_with_range(
		(PROJECT_COORD) { hero->position.x + hero->width / 2, hero->position.y + hero->height / 2 },
		(PROJECT_COORD) { mouse.x - fireball->width / 2, mouse.y - fireball->height / 2 },
		fireball->range), &(fireball->trajectory_size)
		);
	}
	if (fireball->trajectory == NULL)
		pr_exit_with_last_error();

	hero->attack = HERO_IS_ATTACKING_FRB;
	fireball->is_active = TRUE;

	pr_hero_change_image(hero, 0);

	return (double)clock() / CLOCKS_PER_SEC;
}