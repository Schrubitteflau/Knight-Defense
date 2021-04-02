#ifndef __BASE_H__
#define __BASE_H__

#include <allegro5/allegro.h>

// Quelques couleurs basiques
#define		BLACK				al_map_rgb(0, 0, 0)
#define		WHITE				al_map_rgb(255, 255, 255)
#define		RED					al_map_rgb(255, 0, 0)
#define		GREEN				al_map_rgb(0, 255, 0)
#define		BLUE				al_map_rgb(0, 0, 255)
#define		TRANSPARENT			al_map_rgba(0, 0, 0, 0)

#define		FRAMERATE			1.0 / 45
#define		ANIMRATE			1.0 / 8


typedef enum PROJECT_BOOL { FALSE, TRUE } PROJECT_BOOL;

typedef struct PROJECT_COORD
{
	short x, y;
} PROJECT_COORD;


// Prototypes
PROJECT_BOOL pr_init_allegro(void);
void pr_init_display(ALLEGRO_DISPLAY **display);
PROJECT_BOOL pr_init_event_queue(ALLEGRO_EVENT_QUEUE **queue, ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer1, ALLEGRO_TIMER *timer2, ALLEGRO_TIMER *timer3, ALLEGRO_TIMER *timer4, ALLEGRO_TIMER *timer5);

#endif
