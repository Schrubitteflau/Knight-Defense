#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "base.h"
#include "errors.h"
#include "resource/resource.h"

int main(void)
{
    ALLEGRO_DISPLAY *display;

    // Initialisation d'Allegro
    if (!kd_init_allegro())
    {
        goto error_exit;
    }

    // Création et initialisation du display
    if (!kd_init_display(&display))
    {
        goto error_exit;
    }

    al_set_window_title(display, "Knight defense");

    if (kd_load_font(KD_FONT_DONKEYSTONE))
    {
        printf("font chargeee");
    }
    else
    {
        printf("font pas chargee");
    }

    error_exit:
    //clean_exit:

    al_destroy_display(display);

    return EXIT_SUCCESS;
}