#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "base.h"
#include "error.h"


PROJECT_BOOL pr_init_allegro(void)
{
	if (!al_init())
	{
		pr_error("al_init()");
		return FALSE;
	}

	if (!al_init_image_addon())
	{
		pr_error("al_init_image_addon()");
		return FALSE;
	}

	if (!al_init_font_addon())
	{
		pr_error("al_init_font_addon()");
		return FALSE;
	}

	if (!al_init_ttf_addon())
	{
		pr_error("al_init_ttf_addon()");
		return FALSE;
	}

	if (!al_install_mouse())
	{
		pr_error("al_install_mouse()");
		return FALSE;
	}

	if (!al_install_keyboard())
	{
		pr_error("al_install_keyboard()");
		return FALSE;
	}

	if (!al_init_primitives_addon())
	{
		pr_error("al_primitives_addon");
		return FALSE;
	}

	srand(time(NULL));

	return TRUE;
}


void pr_init_display(ALLEGRO_DISPLAY **display)
{
	ALLEGRO_DISPLAY_MODE mode;

	// On créé un display, par défaut la résolution sera la résolution moyenne entre la plus petite et la plus grande
	al_get_display_mode(al_get_num_display_modes() / 2, &mode);

	*display = al_create_display(mode.width, mode.height);
	if (display == NULL)
		pr_error("al_create_display()");
}


PROJECT_BOOL pr_init_event_queue(ALLEGRO_EVENT_QUEUE **queue, ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer1, ALLEGRO_TIMER *timer2, ALLEGRO_TIMER *timer3, ALLEGRO_TIMER *timer4, ALLEGRO_TIMER *timer5)
{
	*queue = al_create_event_queue();
	if (*queue == NULL)
	{
		pr_error("al_create_event_queue()");
		return FALSE;
	}

	al_register_event_source(*queue, al_get_keyboard_event_source());
	al_register_event_source(*queue, al_get_mouse_event_source());
	al_register_event_source(*queue, al_get_display_event_source(display));
	al_register_event_source(*queue, al_get_timer_event_source(timer1));
	al_register_event_source(*queue, al_get_timer_event_source(timer2));
	al_register_event_source(*queue, al_get_timer_event_source(timer3));
	al_register_event_source(*queue, al_get_timer_event_source(timer4));
	al_register_event_source(*queue, al_get_timer_event_source(timer5));

	return TRUE;
}