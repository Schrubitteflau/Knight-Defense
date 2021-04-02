#ifndef __BASE_H__
#define __BASE_H__

#include <stdbool.h>

#include <allegro5/allegro.h>

// Quelques couleurs de base
#define     BLACK               al_map_rgb(0, 0, 0)
#define     WHITE               al_map_rgb(255, 255, 255)
#define     RED                 al_map_rgb(255, 0, 0)
#define     GREEN               al_map_rgb(0, 255, 0)
#define     BLUE                al_map_rgb(0, 0, 255)
#define     TRANSPARENT         al_map_rgba(0, 0, 0, 0)

#define     FRAMERATE           1.0 / 45
#define     ANIMRATE            1.0 / 8

typedef struct KD_COORD
{
	short x, y;
} KD_COORD;

bool kd_init_allegro(void);
bool kd_init_display(ALLEGRO_DISPLAY **display);

#endif
