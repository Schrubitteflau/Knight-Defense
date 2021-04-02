#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "menu.h"

ALLEGRO_BITMAP* pr_resize_bitmap(ALLEGRO_BITMAP *original, int size_x, int size_y);
PROJECT_COORD pr_convert_mouse(PROJECT_COORD mouse, int width, int height);
void pr_game_over(ALLEGRO_DISPLAY *d, PROJECT_TEXT_ZONE *game_over, clock_t duration, ALLEGRO_BITMAP *fond);

#endif