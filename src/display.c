#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "base.h"
#include "error.h"
#include "menu.h"


// Foncion qui retourne une image redimensionnée selon la taille size_x x size_y, et à partir du fichier filename
ALLEGRO_BITMAP* pr_resize_bitmap(ALLEGRO_BITMAP *original, int size_x, int size_y)
{
	ALLEGRO_BITMAP *resized_bmp, *previous_target;

	// On créé la bitmap qui sera de la taille voulue
	resized_bmp = al_create_bitmap(size_x, size_y);
	if (resized_bmp == NULL)
	{
		pr_error("al_create_bitmap()");
		return NULL;
	}

	// On sauvegarde la cible précédente de dessin pour la restaurer plus tard
	previous_target = al_get_target_bitmap();

	// On définit comme cible actuelle la bitmap qui va être modifiée
	al_set_target_bitmap(resized_bmp);
	// Puis on redimensionne l'image source (loaded_bitmap) et l'on stocke le résultat
	// (image redimensionnée) dans la cible (resized_bitmap)
	al_draw_scaled_bitmap(original, 0, 0, al_get_bitmap_width(original), al_get_bitmap_height(original), 0, 0, size_x, size_y, 0);

	// On restaure la précédente cible de dessin
	al_set_target_bitmap(previous_target);

	// Et on retourne l'image redimensionnée
	return resized_bmp;
}



// Fonction qui convertit la position de la souris en fonction des dimensions de la fenêtre
PROJECT_COORD pr_convert_mouse(PROJECT_COORD mouse, int width, int height)
{
	return (PROJECT_COORD) { (short)(((float)1280 / width) * mouse.x), (short)(((float)768 / height) * mouse.y) };
}


// Fonction qui affiche l'écran de game over
void pr_game_over(ALLEGRO_DISPLAY *d, PROJECT_TEXT_ZONE *game_over, int score, ALLEGRO_BITMAP *fond)
{
	char text[256];

	sprintf_s(text, 256, "Perdu ! Votre score est de %d", score);

	al_set_target_backbuffer(d);

	game_over->image = NULL;
	pr_create_button(game_over, "resources/fonts/DonkeyStone.otf", BLUE, al_get_display_width(d) * 2, al_get_display_height(d) / 5, text, TRANSPARENT, 0, al_get_display_height(d) / 2);

	al_draw_bitmap(fond, 0, 0, 0);
	al_draw_bitmap(game_over->image, game_over->x, game_over->y, 0);

	al_flip_display();

	al_rest(3.0);
}