#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "base.h"
#include "error.h"
#include "menu.h"
#include "display.h"


// Fonction pour libérer la mémoire d'un objet de type PROJECT_TEXT_ZONE
void pr_destroy_text_zone(PROJECT_TEXT_ZONE *block)
{
	// On supprime l'image de la mémoire
	al_destroy_bitmap(block->image);
	// Puis on libère le bloc mémoire de la structure
	free(block);
}


// Fonction qui créé un bouton de texte, d'une police dont le fichier est à font_path, la couleur du texte est font_color, la taille du bouton est tx et ty
// La couleur de fond est background_color et la position posy et posy
PROJECT_BOOL pr_create_button(PROJECT_TEXT_ZONE *button, const char *font_path, ALLEGRO_COLOR font_color, int tx, int ty, const char *txt, ALLEGRO_COLOR background_color, int posx, int posy)
{
	ALLEGRO_DISPLAY *d;
	ALLEGRO_BITMAP *previous_target;
	ALLEGRO_FONT *font;
	int display_width, display_height;

	d = al_is_system_installed() ? al_get_current_display() : NULL;
	if (d == NULL)
	{
		pr_error("al_get_current_display() || al_is_system_installed()");
		return FALSE;
	}

	// Pour restaurer l'ancienne cible
	previous_target = al_get_target_bitmap();

	// Les dimensions de la fenêtre
	display_width = al_get_display_width(d);
	display_height = al_get_display_height(d);

	// La taille du bouton
	button->tx = tx;
	button->ty = ty;

	// Cette fonction charge la police à la bonne taille pour que le texte ait les dimensions
	// demandées (tx et ty). On multiplie par 2 pour ajuster la taille, pour que celle-ci
	// corresponde à la taille voulue en pixel.
	font = al_load_ttf_font_stretch(font_path, tx * 2 / strlen(txt), ty, 0);
	if (font == NULL)
	{
		pr_error("al_load_font()");
		return FALSE;
	}

	// Si une image est déjà associée au bouton, on la supprime de la mémoire avant de la remplacer
	//if (button->image != NULL)
	//	al_destroy_bitmap(button->image);

	// On créé une image de la taille demandée
	button->image = al_create_bitmap(tx, ty);
	if (button->image == NULL)
	{
		al_destroy_font(font);
		pr_error("al_create_bitmap()");
		return FALSE;
	}

	// On la colorie de la couleur de fond (peut être transparent)
	al_set_target_bitmap(button->image);
	al_clear_to_color(background_color);

	// On écrit le texte de la bonne taille dans l'image
	al_draw_text(font, font_color, 0, 0, ALLEGRO_ALIGN_LEFT, txt);

	// La position du bouton
	button->x = posx;
	button->y = posy;

	// Et on détruit la police chargée en mémoire
	al_destroy_font(font);

	al_set_target_bitmap(previous_target);

	return TRUE;
}

// Fonction qui créée tous les boutons de résolution et les ajuste (taille, position) en fonction de leur nombre
PROJECT_BOOL pr_create_all_resolutions_buttons(PROJECT_TEXT_ZONE **buttons, const char *font_path)
{
	ALLEGRO_DISPLAY *d;
	ALLEGRO_DISPLAY_MODE d_mode;

	int display_width, display_height;
	int num_resolutions;
	int current_x, current_y;
	int size_x, size_y;

	int reste;
	int c1, c2, c3;
	int i;
	char text_button[256];

	// Ici, on récupère le nombre de résolutions différentes possibles, donc à afficher
	num_resolutions = al_get_num_display_modes();
	// Et pour l'ensemble des 3 colonnes on assigne le nombre de résolutions à afficher
	reste = num_resolutions % COLUMNS_TO_PRINT;

	// Pour bien répartir les résolutions sur le nombre de colonnes (COLUMNS_TO_PRINT), ici 3
	c1 = num_resolutions / COLUMNS_TO_PRINT + (reste > 0);
	reste /= 2;
	c2 = num_resolutions / COLUMNS_TO_PRINT + (reste > 0);
	reste /= 2;
	c3 = num_resolutions / COLUMNS_TO_PRINT + (reste > 0);

	// On récupère le display courant
	d = al_is_system_installed() ? al_get_current_display() : NULL;
	if (d == NULL)
	{
		pr_error("al_get_current_display() || al_is_system_installed()");
		return FALSE;
	}

	// On récupère dans 2 variables les dimensions de la fenêtre
	display_width = al_get_display_width(d);
	display_height = al_get_display_height(d);

	// On définit les coordonnées de départ
	current_x = 0;
	current_y = 0.2 * display_height;

	// Et la taille en x et y des boutons
	size_x = display_width / 3;
	size_y = 0.7 * display_height / c1;

	// On créé les boutons que l'on stocke dans un tableau de pointeurs de boutons
	for (i = 0; i < c1; i++)
	{
		// On récupère la résolution de la carte graphique
		al_get_display_mode(i, &d_mode);
		// Que l'on transforme en chaîne de caractère du type "largeurxhauteur"
		sprintf_s(text_button, 256, "%dx%d", d_mode.width, d_mode.height);
		// Chaîne que l'on passe à la fonction pour en créer un bouton
		if (!pr_create_button(buttons[i], font_path, WHITE, size_x, size_y, text_button, TRANSPARENT, current_x, current_y))
		{
			for (int j = 0; j < i; j++)
				pr_destroy_text_zone(buttons[j]);
			return FALSE;
		}
		// Et on augmente les coordonnées en y pour que le bouton suivant soit collé juste sous celui tout fraîchement créé
		current_y += size_y;
	}

	// On ajoute i à c2 (pour que le compte parcoure toutes les colonnes et donc toutes les résolutions)
	c2 += i;
	// On augmente la coordonnée x d'1/nombre de colonnes de la largeur de l'écran
	current_x += display_width / COLUMNS_TO_PRINT;
	// Puis on remet y à sa valeur initiale, car on passe à la colonne suivante donc on part du haut pour aller vers le bas
	current_y = 0.2 * display_height;
	// Et on recommence...
	// Cette zone du code est à améliorer car elle ne fonctionne que si COLUMNS_TO_PRINT vaut 3, car elle ne prend en compte
	// que 3 colonnes (c1, c2 et c3)
	for (; i < c2; i++)
	{
		al_get_display_mode(i, &d_mode);
		sprintf_s(text_button, 256, "%dx%d", d_mode.width, d_mode.height);
		if (!pr_create_button(buttons[i], font_path, WHITE, size_x, size_y, text_button, TRANSPARENT, current_x, current_y))
		{
			for (int j = 0; j < i; j++)
				pr_destroy_text_zone(buttons[j]);
			return FALSE;
		}
		current_y += size_y;
	}

	c3 += i;
	current_x += display_width / 3;
	current_y = 0.2 * display_height;
	for (; i < c3; i++)
	{
		al_get_display_mode(i, &d_mode);
		sprintf_s(text_button, 256, "%dx%d", d_mode.width, d_mode.height);
		if (!pr_create_button(buttons[i], font_path, WHITE, size_x, size_y, text_button, TRANSPARENT, current_x, current_y))
		{
			for (int j = 0; j < i; j++)
				pr_destroy_text_zone(buttons[j]);
			return FALSE;
		}
		current_y += size_y;
	}

	return TRUE;
}


PROJECT_BOOL pr_print_resolution_menu(PROJECT_TEXT_ZONE *choose_button, PROJECT_TEXT_ZONE *fullscreen_button, PROJECT_TEXT_ZONE *continue_button, PROJECT_TEXT_ZONE **resolution_buttons)
{
	ALLEGRO_DISPLAY *d;

	int display_width, display_height;
	int i;

	d = al_is_system_installed() ? al_get_current_display() : NULL;
	if (d == NULL)
	{
		pr_error("al_is_system_installed() || al_get_current_display()");
		return FALSE;
	}

	// Les dimensions de la fenêtre
	display_width = al_get_display_width(d);
	display_height = al_get_display_height(d);

	// On créé les bouttons
	if (!pr_create_button(choose_button, "resources/fonts/leadcoat.ttf", GREEN, display_width / 1.5, display_height / 10, "Choisir une resolution :", TRANSPARENT, 0, 0))
		return FALSE;

	if (!pr_create_button(fullscreen_button, "resources/fonts/leadcoat.ttf", BLUE, display_width / 3, display_height / 10, "Plein ecran", TRANSPARENT, display_width - display_width / 3, 0))
	{
		pr_destroy_text_zone(choose_button);
		return FALSE;
	}

	if (!pr_create_button(continue_button, "resources/fonts/leadcoat.ttf", RED, display_width / 3, display_height / 10, "Continuer", TRANSPARENT, display_width / 2, display_height - display_height / 10))
	{
		pr_destroy_text_zone(choose_button);
		pr_destroy_text_zone(fullscreen_button);
		return FALSE;
	}

	if (!pr_create_all_resolutions_buttons(resolution_buttons, "resources/fonts/leadcoat.ttf"))
	{
		pr_destroy_text_zone(choose_button);
		pr_destroy_text_zone(fullscreen_button);
		pr_destroy_text_zone(continue_button);
		free(resolution_buttons);
		return FALSE;
	}

	// Et on les affiche
	al_set_target_backbuffer(d);
	al_draw_bitmap(choose_button->image, choose_button->x, choose_button->y, 0);
	al_draw_bitmap(fullscreen_button->image, fullscreen_button->x, fullscreen_button->y, 0);
	al_draw_bitmap(continue_button->image, continue_button->x, continue_button->y, 0);
	for (i = 0; i < al_get_num_display_modes(); i++)
		al_draw_bitmap(resolution_buttons[i]->image, resolution_buttons[i]->x, resolution_buttons[i]->y, 0);

	al_flip_display();

	return TRUE;
}


// Fonction qui retourne l'index du boutton de résolution cliqué, sinon -1 si le clic n'est sur aucun bouton de résolution
int pr_get_index_button(int x, int y, PROJECT_TEXT_ZONE **buttons)
{
	int max, i;

	max = al_get_num_display_modes();

	// On parcours chaque bouton de résolution pour savoir sur lequel on a cliqué (x et y correspondent aux coordonnées
	// de clic de la souris)
	for (i = 0; i < max; i++)
	{
		if (x >= buttons[i]->x && x <= buttons[i]->x + buttons[i]->tx &&
			y >= buttons[i]->y && y <= buttons[i]->y + buttons[i]->ty)
			return i;
	}

	// On retourne -1 si le clic n'est sur aucun de ces boutons
	return -1;
}


PROJECT_BOOL pr_choose_resolution(ALLEGRO_DISPLAY *d)
{
	ALLEGRO_DISPLAY_MODE mode;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_EVENT e;

	PROJECT_TEXT_ZONE *choose_button, *continue_button, *fullscreen_button;
	PROJECT_TEXT_ZONE **resolution_buttons;

	int i, index, num_display_modes;

	num_display_modes = al_get_num_display_modes();

	// On créé les boutons et on alloue la mémoire qui leur est requise
	choose_button = (PROJECT_TEXT_ZONE*)malloc(sizeof(PROJECT_TEXT_ZONE));
	if (choose_button == NULL)
	{
		pr_error("malloc()");
		return FALSE;
	}

	fullscreen_button = (PROJECT_TEXT_ZONE*)malloc(sizeof(PROJECT_TEXT_ZONE));
	if (fullscreen_button == NULL)
	{
		free(choose_button);
		pr_error("malloc()");
		return FALSE;
	}

	continue_button = (PROJECT_TEXT_ZONE*)malloc(sizeof(PROJECT_TEXT_ZONE));
	if (continue_button == NULL)
	{
		free(choose_button);
		free(fullscreen_button);
		pr_error("malloc()");
		return FALSE;
	}

	resolution_buttons = (PROJECT_TEXT_ZONE**)malloc(sizeof(PROJECT_TEXT_ZONE*) * num_display_modes);
	if (resolution_buttons == NULL)
	{
		free(choose_button);
		free(fullscreen_button);
		free(continue_button);
		pr_error("malloc()");
		return FALSE;
	}

	// On initialise les images des boutons à NULL, pour éviter une erreur de type "access violation" en essayant
	// de supprimer les images associées pour libérer de la mémoire (voir fonction pr_create_button())
	choose_button->image = NULL;
	fullscreen_button->image = NULL;
	continue_button->image = NULL;

	for (i = 0; i < num_display_modes; i++)
	{
		resolution_buttons[i] = (PROJECT_TEXT_ZONE*)malloc(sizeof(PROJECT_TEXT_ZONE));
		if (resolution_buttons[i] == NULL)
		{
			for (int j = 0; j < i; j++)
				pr_destroy_text_zone(resolution_buttons[j]);
			pr_error("malloc()");
			return FALSE;
		}
		resolution_buttons[i]->image = NULL;
	}

	// On affiche le menu de résolution, en passant par référence les boutons dont les données seront initialisées
	if (!pr_print_resolution_menu(choose_button, fullscreen_button, continue_button, resolution_buttons))
	{
		pr_destroy_text_zone(choose_button);
		pr_destroy_text_zone(fullscreen_button);
		pr_destroy_text_zone(continue_button);
		for (i = 0; i < num_display_modes; i++)
			pr_destroy_text_zone(resolution_buttons[i]);
		free(resolution_buttons);
		return FALSE;
	}

	// Maintenant que tous les bouttons existent et son affichés à l'écran, il faut faire en sorte que l'utilisateur puisse choisir
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_mouse_event_source());

	while (1)
	{
		// Fonction bloquante qui attend un évènement de type clic de souris
		al_wait_for_event(queue, &e);

		// Si on a cliqué gauche, alors on vérifie si l'on a appuyé sur le bouton "continuer", dans ce cas on sort de la boucle
		// puis de la fonction, pour retourner dans le main et continuer le programme
		if (e.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 1)
		{
			// Si on a cliqué sur le bouton continuer
			if (e.mouse.x >= continue_button->x && e.mouse.x <= continue_button->x + continue_button->tx &&
				e.mouse.y >= continue_button->y && e.mouse.y <= continue_button->y + continue_button->ty)
			{
				break; // Et on sort de la boucle pour quitter la fonction
			}
			// Sinon, si on a cliqué sur le bouton de plein écran
			else if (e.mouse.x >= fullscreen_button->x && e.mouse.x <= fullscreen_button->x + fullscreen_button->tx &&
				e.mouse.y >= fullscreen_button->y && e.mouse.y <= fullscreen_button->y + fullscreen_button->ty)
			{
				// A faire : si on est déjà en plein écran, le bouton plein écran est remplacé par fenêtré et inversement
				// Si on est déjà en plein écran, on change juste la résolution
				int w, h;
				w = al_get_display_width(d);
				h = al_get_display_height(d);
				// Alors on met en plein écran ! On détruit le display, et on le récréé avec la résolution utilisée actuellement
				al_destroy_display(d);
				al_set_new_display_flags(ALLEGRO_FULLSCREEN);
				d = al_create_display(w, h);
				if (!pr_print_resolution_menu(choose_button, fullscreen_button, continue_button, resolution_buttons))
					return FALSE;
			}

			// Sinon, si l'index du bouton cliqué est valide, on redimensionne la fenêtre en conséquence, en réaffichant tout
			// Ce qu'elle contient avec la taille appropriée
			else
			{
				index = pr_get_index_button(e.mouse.x, e.mouse.y, resolution_buttons);
				if (index == -1) // Alors on a cliqué sur aucun bouton, nouveau tour de boucle
					continue;

				// On récupère le nouveau mode d'affichage en fonction de l'index
				al_get_display_mode(index, &mode);

				// On redimensionne la fenêtre en elle-même
				if (!al_resize_display(d, mode.width, mode.height))
				{
					pr_error("al_resize_display()");
					return FALSE;
				}

				al_destroy_bitmap(choose_button->image);
				al_destroy_bitmap(continue_button->image);
				al_destroy_bitmap(fullscreen_button->image);
				for (i = 0; i < al_get_num_display_modes(); i++)
					al_destroy_bitmap(resolution_buttons[i]->image);

				// Et on réaffiche son contenu dans les dimensions appropriées, que l'on récupère par référence
				if (!pr_print_resolution_menu(choose_button, fullscreen_button, continue_button, resolution_buttons))
					return FALSE;
			}
		}
	}

	// On détruit la file d'évènement
	al_destroy_event_queue(queue);

	// Ainsi que tous les boutons
	pr_destroy_text_zone(choose_button);
	pr_destroy_text_zone(fullscreen_button);
	pr_destroy_text_zone(continue_button);
	for (i = 0; i < num_display_modes; i++)
		pr_destroy_text_zone(resolution_buttons[i]);
	free(resolution_buttons);


	return TRUE;
}



PROJECT_BOOL pr_print_scenario(ALLEGRO_DISPLAY *d)
{
	ALLEGRO_EVENT_QUEUE *queue;			// Boucle d'évènement propre au scénario
	ALLEGRO_EVENT event;
	ALLEGRO_BITMAP *scenario, *scenario_redim;

	// Chargement de l'image d'origine en mémoire
	scenario = al_load_bitmap("resources/images/scenario.png");
	if (scenario == NULL)
	{
		pr_error("al_load_bitmap()");
		return FALSE;
	}

	// Redimensionnement de celle-ci
	scenario_redim = pr_resize_bitmap(scenario, al_get_display_width(d), al_get_display_height(d));
	al_destroy_bitmap(scenario);
	if (!scenario_redim)
	{
		pr_error("al_load_bitmap()");
		return FALSE;
	}

	// Affichage de l'image redimensionnée et libération de la mémoire
	al_draw_bitmap(scenario_redim, 0, 0, 0);
	al_flip_display();
	al_destroy_bitmap(scenario_redim);

	// Créaation de la file d'évènement
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());

	// Si l'on a reçu un évènement clavier, alors on quitte la fonction (flemme de faire un bouton exprès)
	al_wait_for_event(queue, &event);

	// Sans oublier bien sûr de libérer la mémoire
	al_destroy_event_queue(queue);

	return TRUE;
}



// Fonction qui affiche le menu principal du jeu, avec plusieurs boutons
PROJECT_BOOL pr_print_main_menu(ALLEGRO_DISPLAY *d)
{
	ALLEGRO_BITMAP *fond, *resized_fond;			// L'image de fond, d'origine et redimensionnée
	ALLEGRO_BITMAP *all_screen, *all_redim_screen;	// Tout l'écran enregistré en une image, et pouvant être redimensionné
	ALLEGRO_BITMAP *cursor;							// Curseur de souris personnalisé
	ALLEGRO_FONT *font;								// La police utilisée pour le texte des boutons
	ALLEGRO_EVENT_QUEUE *queue;						// File d'évènement pour la souris et les boutons
	ALLEGRO_EVENT event;							// Pour récupérer les évènements en question

	// Les boutons qui seront affichés
	PROJECT_TEXT_ZONE jouer = { .image = NULL }, resolutions = { .image = NULL };
	PROJECT_TEXT_ZONE scenario = { .image = NULL }, quitter = { .image = NULL };
	PROJECT_COORD mouse = { 0, 0 };					// Coordonnées de la souris converties en fonction des dimensions de la fenêtre
	PROJECT_BOOL fin = FALSE;						// Pour quitter la boucle d'évènement en cas de besoin

	// Variables utiles au bon placement et au redimensionnement des éléments du menu
	int display_width, display_height;
	int current_width, current_height;
	int font_size;

	// Variables utiles pour stocker la taille de la moitié du curseur
	int cursor_middle_x, cursor_middle_y;

	// Chargement du fond d'origine
	fond = al_load_bitmap("resources/images/fond.png");
	if (fond == NULL)
	{
		pr_error("al_load_bitmap()");
		return FALSE;
	}

	// Dimensions de la fenêtre
	display_width = al_get_display_width(d);
	display_height = al_get_display_height(d);

	// Redimensionnement de l'image de fond en fonction de la taille de la fenêtre
	resized_fond = pr_resize_bitmap(fond, display_width, display_height);
	al_destroy_bitmap(fond);
	if (resized_fond == NULL)
		return FALSE;


	// Affichage du fond
	al_draw_bitmap(resized_fond, 0, 0, 0);
	al_destroy_bitmap(resized_fond);

	// Déterminer la taille de police adaptée en fonction des dimensions de la fenêtre
	font_size = ((float)display_width / 1280 * 150);

	// Chargement de la police pour les textes des boutons en mémoire
	font = al_load_font("resources/fonts/DonkeyStone.otf", font_size, 0);

	// Déterminer les dimensions du bouton
	current_width = al_get_text_width(font, "Jouer");
	current_height = al_get_font_ascent(font);
	// Puis créer le bouton
	if (!pr_create_button(&jouer, "resources/fonts/DonkeyStone.otf", GREEN, current_width, current_height, "Jouer", TRANSPARENT, display_width / 2 - current_width / 4, display_height / 4))
		return FALSE;
	// Avant de l'afficher
	PRINT_BUTTON(jouer);
	// Et de libérer sa mémoire
	al_destroy_bitmap(jouer.image);


	current_width = al_get_text_width(font, "Resolutions d ecran");
	current_height = al_get_font_ascent(font);
	if (!pr_create_button(&resolutions, "resources/fonts/DonkeyStone.otf", GREEN, current_width, current_height, "Resolutions d ecran", TRANSPARENT, display_width / 2 - current_width / 4, display_height / 1.5))
	{
		al_destroy_font(font);
		return FALSE;
	}
	PRINT_BUTTON(resolutions);
	al_destroy_bitmap(resolutions.image);


	current_width = al_get_text_width(font, "Scenario");
	current_height = al_get_font_ascent(font);
	if (!pr_create_button(&scenario, "resources/fonts/DonkeyStone.otf", GREEN, current_width, current_height, "Scenario", TRANSPARENT, display_width / 2 - current_width / 4, display_height / 2.5))
	{
		al_destroy_font(font);
		return FALSE;
	}
	PRINT_BUTTON(scenario);
	al_destroy_bitmap(scenario.image);


	current_width = al_get_text_width(font, "Quitter");
	current_height = al_get_font_ascent(font);
	if (!pr_create_button(&quitter, "resources/fonts/DonkeyStone.otf", GREEN, current_width, current_height, "Quitter", TRANSPARENT, display_width / 2 - current_width / 4, display_height / 1.2))
	{
		al_destroy_font(font);
		return FALSE;
	}
	PRINT_BUTTON(quitter);
	al_destroy_bitmap(quitter.image);

	// Libérer de la mémoire qui ne servira plus
	al_destroy_font(font);

	// Affichage du tout à l'écran
	al_flip_display();
	al_set_target_backbuffer(d);
	all_screen = al_clone_bitmap(al_get_backbuffer(d));
	all_redim_screen = all_screen;


	// Maintenant, il faut activer les boutons et afficher ce qu'il faut en conséquence

	// Création de la file d'évènement et association des évènements de type souris et display
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(d));

	cursor = al_load_bitmap("resources/images/menu_cursor.png");
	if (cursor == NULL)
	{
		pr_error("al_load_bitmap()");
		return FALSE;
	}

	cursor_middle_x = al_get_bitmap_width(cursor) / 2;
	cursor_middle_y = al_get_bitmap_height(cursor) / 2;
	
	while (!fin)
	{
		al_draw_bitmap(all_redim_screen, 0, 0, 0);

		al_wait_for_event(queue, &event);
		al_draw_bitmap(cursor, event.mouse.x - cursor_middle_x, event.mouse.y - cursor_middle_y, 0);

		// Il faut convertir les coordonnées de la souris par rapport aux dimensions actuelles de la fenêtre
		// car sinon il n'est plus possible de cliquer sur les boutons, car les coordonnées sont absolues par rapport à la fenêtre tandis
		// que les positions des boutons ne changent pas de valeur, même lorsque la fenêtre est redimensionnée et qu'ils ne sont plus aux
		// mêmes coordonnées absolues
		mouse = pr_convert_mouse((PROJECT_COORD) { (short)event.mouse.x, (short)event.mouse.y }, display_width, display_height);

		al_flip_display();

		switch (event.type)
		{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			exit(EXIT_SUCCESS);
			break;

			// Si l'on a cliqué quelque part, on cherche sur quel bouton
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

			// Si l'on a cliqué sur jouer, alors on quitte la boucle puis la fonction pour retourner dans le main()
			// et démarrer le jeu
			if (mouse.x >= jouer.x && mouse.x <= jouer.x + jouer.tx &&
				mouse.y >= jouer.y && mouse.y <= jouer.y + jouer.ty)
			{
				fin = TRUE;
			}
			// Sinon, si l'on a cliqué sur le bouton "scénario", alors on affiche le scénario
			else if (mouse.x >= scenario.x && mouse.x <= scenario.x + scenario.tx &&
				mouse.y >= scenario.y && mouse.y <= scenario.y + scenario.ty)
			{
				if (!pr_print_scenario(d))
					pr_exit_with_last_error();

				// On vide la file d'évènement pour ne pas qu'il y ait d'erreur entre les coordonnées de la souris
				al_flush_event_queue(queue);

				// Obligation de tout réafficher puisqu'un nouvel affichage d'écran ne se déclenche que lorsqu'un évènement est reçu
				al_draw_bitmap(all_redim_screen, 0, 0, 0);
				al_draw_bitmap(cursor, event.mouse.x - cursor_middle_x, event.mouse.y - cursor_middle_y, 0);
				al_flip_display();
			}
			// Sinon, si l'on a cliqué sur "résolution d'écran", alors on affiche le menu de choix de la résolution d'écran
			// et on réaffiche tout l'écran
			else if (mouse.x >= resolutions.x && mouse.x <= resolutions.x + resolutions.tx &&
				mouse.y >= resolutions.y && mouse.y <= resolutions.y + resolutions.ty)
			{
				al_clear_to_color(BLACK);

				al_show_mouse_cursor(d);
				// Choix de la résolution
				if (!pr_choose_resolution(d))
					pr_exit_with_last_error();

				al_hide_mouse_cursor(d);

				// On met à jour les variables concernant les dimensions de la fenêtre
				display_width = al_get_display_width(d);
				display_height = al_get_display_height(d);

				// On redimensionne l'image du menu d'origine
				if (all_redim_screen != all_screen)
					al_destroy_bitmap(all_redim_screen);
				all_redim_screen = pr_resize_bitmap(all_screen, display_width, display_height);

				// On vide la file d'évènement pour ne pas qu'il y ait d'erreur entre les coordonnées de la souris
				al_flush_event_queue(queue);
			}

			// Sinon si l'on a cliqué sur quitter, alors on quitte
			else if (mouse.x >= quitter.x && mouse.x <= quitter.x + quitter.tx &&
				mouse.y >= quitter.y && mouse.y <= quitter.y + quitter.ty)
			{
				exit(EXIT_SUCCESS);
			}
		}
	}

	if (all_screen != all_redim_screen)
		al_destroy_bitmap(all_redim_screen);
	al_destroy_bitmap(all_screen);
	al_destroy_bitmap(cursor);

	al_destroy_event_queue(queue);

	// Tout c'est bien passé
	return TRUE;
}