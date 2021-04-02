#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "base.h"
#include "errors.h"

bool kd_init_allegro(void)
{
	if (!al_init())
	{
		kd_error("al_init()");
		return false;
	}

	if (!al_init_image_addon())
	{
		kd_error("al_init_image_addon()");
		return false;
	}

	if (!al_init_font_addon())
	{
		kd_error("al_init_font_addon()");
		return false;
	}

	if (!al_init_ttf_addon())
	{
		kd_error("al_init_ttf_addon()");
		return false;
	}

	if (!al_install_mouse())
	{
		kd_error("al_install_mouse()");
		return false;
	}

	if (!al_install_keyboard())
	{
		kd_error("al_install_keyboard()");
		return false;
	}

	if (!al_init_primitives_addon())
	{
		kd_error("al_primitives_addon");
		return false;
	}

	srand(time(NULL));

	return true;
}

bool kd_init_display(ALLEGRO_DISPLAY **display)
{
    ALLEGRO_DISPLAY_MODE mode;

    // Résolution médiane entre la plus petite et la plus grande supportée
    al_get_display_mode(al_get_num_display_modes() / 2, &mode);

    *display = al_create_display(mode.width, mode.height);
    if (display == NULL)
    {
        kd_error("al_create_display()");
        return false;
    }

    return true;
}