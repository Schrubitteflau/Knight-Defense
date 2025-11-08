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
    //ALLEGRO_EVENT_QUEUE *queue;
    bool end = false;

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

    // Titre de la fenêtre
    al_set_window_title(display, "Knight defense");

    while (!end)
    {
        /* Menu principal :
            - affichage images et boutons, on subscribe onClick et tout sur les boutons
        */
    }

    error_exit:
    //clean_exit:

    al_destroy_display(display);

    return EXIT_SUCCESS;
}