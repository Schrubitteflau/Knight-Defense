#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "base.h"
#include "error.h"
#include "menu.h"
#include "display.h"
#include "way.h"
#include "hero.h"
#include "fireball.h"
#include "ennemy.h"


int main(void)
{
	ALLEGRO_DISPLAY *display;			// Le display courant
	ALLEGRO_EVENT_QUEUE *queue;			// La file d'évènement
	ALLEGRO_EVENT event;				// Une structure évènement pour récupérer les évènements de type minuteur, souris, clavier, display

	ALLEGRO_TIMER *main_timer, *animation_timer;			// Différents minuteurs pour orchester le programme
	ALLEGRO_TIMER *fireball_timer, *ennemy_timer, *ennemy_frequency_timer;

	ALLEGRO_BITMAP *map, *screen, *redim_screen;			// Différents pointeurs servant au redimensionnement de l'écran et de l'image du jeu
	ALLEGRO_BITMAP *fond, *resized_fond;		// Le fond pour le game over
	ALLEGRO_BITMAP *ennemy_bmps[4];		// Les 4 images des ennemis sont chargées une seule fois dans le main() car tous les ennemis utiliseront ces mêmes images
	ALLEGRO_BITMAP *attack_bmps[3];		// Les 3 images montrant l'attaque du héro activée : une épée, une cible rouge et une cible grisée

	PROJECT_COORD mouse = { 0, 0 };		// Les coordonnées de la souris afin de récupérer les coordonnées du pointeur à chaque mouvement de souris

	PROJECT_HERO *hero;					// Un pointeur sur une structure hero : personnage que l'utilisateur contrôle
	PROJECT_FIREBALL *fireball;			// Un pointeur sur une structure fireball : un type d'attaque du héro

	PROJECT_TEXT_ZONE *game_over;		// Bouton de fin de jeu

	//PROJECT_ENNEMY **ennemys;			// Un pointeur sur un tableau d'ennemis, nombre indéterminé car changeant souvent
	PROJECT_ENNEMY *ennemys[256];
	int number_of_ennemys = 256;		// Le nombre d'ennemis (donc la taille du tableau)
	PROJECT_WAY **ways;					// Un pointeur sur un tableau de pointeurs sur way, ce sont tous les chemins disponibles pour les ennemis
	int number_of_ways;					// Le nombre de chemins différents (taille du tableau)

	PROJECT_BOOL key[KEY_MAX] = { FALSE };				// Les 4 flèches du clavier sont enfoncées (TRUE) ou non (FALSE)
	PROJECT_BOOL fin = FALSE, is_attacking = FALSE;

	// Pour avoir des "cooldown", c'est-à-dire des temps de recharge entre les différentes attaques
	clock_t last_frb_attack = (double)clock() / CLOCKS_PER_SEC - FRB_ATTACK_COOLDOWN;
	clock_t last_cac_attack = (double)clock() / CLOCKS_PER_SEC - CAC_ATTACK_COOLDOWN;

	int lifes = 1;						// Nombre de vies restant avant de perdre la partie : si vaut 0 : perdu
	int score = 0;						// Le score que l'on fait
	double ennemy_frequency = 5.0;		// Fréquence de réapparition d'un nouvel ennemi, diminue au fil du temps
	char filename[256];					// Simple chaîne de caractère servant à charger des bitmaps en mémoire en fonction de leur nom de fichier
	int i;								// Un compteur

	// Initialisation d'Allegro
	if (!pr_init_allegro())
		pr_exit_with_last_error();

	// Initialisation et création du display
	pr_init_display(&display);
	if (display == NULL)
		pr_exit_with_last_error();

	al_set_window_title(display, "Knight defense");

	al_hide_mouse_cursor(display);
	if (!pr_print_main_menu(display))
		pr_exit_with_last_error();

	// Minuteur pour gérer les rafraichissements d'écran
	main_timer = al_create_timer(FRAMERATE);
	// Minuteur pour gérer les animations
	animation_timer = al_create_timer(ANIMRATE);
	// Minuteur pour déplacer la boule de feu à intervalle de temps régulier
	// mais plus rapide que les autres entités, il necessite donc un minuteur spécifique
	fireball_timer = al_create_timer(FIRERATE);
	// Minuteur de mouvement des ennemis
	ennemy_timer = al_create_timer(ENNEMYRATE);
	// Minuteur qui ajoute un ennemi au jeu à chaque déclenchement, la fréquence de ce
	// minuteur augmente au fil du temps donc les ennemis sont de plus en plus nombreux
	ennemy_frequency_timer = al_create_timer(ennemy_frequency);

	if (main_timer == NULL || animation_timer == NULL || fireball_timer == NULL || ennemy_timer == NULL || ennemy_frequency_timer == NULL)
	{
		pr_error("al_create_timer()");
		pr_exit_with_last_error();
	}


	// Initialisation de la file d'évènement
	if (!pr_init_event_queue(&queue, display, main_timer, animation_timer, fireball_timer, ennemy_timer, ennemy_frequency_timer))
		pr_exit_with_last_error();

	// Création du héro
	hero = pr_create_hero();
	if (hero == NULL)
		pr_exit_with_last_error();

	// Et de la boule de feu
	fireball = pr_create_fireball("resources/images/fireball.png");
	if (fireball == NULL)
		pr_exit_with_last_error();

	// Création des chemins pour les ennemis
	ways = pr_create_all_ways(&number_of_ways);
	if (ways == NULL)
		pr_exit_with_last_error();


	// Chargement des images des ennemis
	for (i = 0; i < 4; i++)
	{
		sprintf_s(filename, 256, "resources/images/knight%d.png", i + 1);
		ennemy_bmps[i] = al_load_bitmap(filename);
		if (ennemy_bmps[i] == NULL)
		{
			pr_error("al_load_bitmap()");
			pr_exit_with_last_error();
		}
	}


	// Chargement des curseurs d'attaque en mémoire
	attack_bmps[0] = al_load_bitmap("resources/images/sword.png");
	attack_bmps[1] = al_load_bitmap("resources/images/cible_ok.png");
	attack_bmps[2] = al_load_bitmap("resources/images/cible_nonok.png");
	for (i = 0; i < 3; i++)
	{
		if (attack_bmps[i] == NULL)
		{
			for (int j = i; j > 0; j--)
				al_destroy_bitmap(attack_bmps[j]);
			pr_exit_with_last_error();
		}
		
	}

	// Chargement de la carte en mémoire
	map = al_load_bitmap("resources/images/map.jpg");
	if (map == NULL)
	{
		pr_error("al_load_bitmap()");
		pr_exit_with_last_error();
	}

	// Démarrage des minuteurs
	al_start_timer(main_timer);
	al_start_timer(animation_timer);
	al_start_timer(fireball_timer);
	al_start_timer(ennemy_timer);
	al_start_timer(ennemy_frequency_timer);


	// Création du 1er ennemi
	/*ennemys = (PROJECT_ENNEMY**)malloc(sizeof(PROJECT_ENNEMY*));
	ennemys[0] = pr_create_ennemy(ennemy_bmps, ways[rand() % number_of_ways]);
	if (ennemys[0] == NULL)
		pr_exit_with_last_error();*/

	for (i = 0; i < 256; i++)
	{
		ennemys[i] = pr_create_ennemy(ennemy_bmps, ways[rand() % number_of_ways]);
		ennemys[i]->life = 0;
	}

	ennemys[0]->life = ennemys[0]->max_life;


	// Boucle principale du jeu
	while (!fin)
	{
		// On attend un évènement
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			fin = TRUE;
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (event.keyboard.keycode)
			{
				// Pour diriger le personnage
				case ALLEGRO_KEY_UP:		case ALLEGRO_KEY_Z:			key[KEY_UP] = TRUE;			break;
				case ALLEGRO_KEY_LEFT:		case ALLEGRO_KEY_Q:			key[KEY_LEFT] = TRUE;		break;
				case ALLEGRO_KEY_DOWN:		case ALLEGRO_KEY_S:			key[KEY_DOWN] = TRUE;		break;
				case ALLEGRO_KEY_RIGHT:		case ALLEGRO_KEY_D:			key[KEY_RIGHT] = TRUE;		break;

				// Si la touche espace est pressée, alors on chaneg le mode d'attaque du héro
				case ALLEGRO_KEY_SPACE:		hero->attack_mode = !hero->attack_mode;					break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse = (PROJECT_COORD) { (short)event.mouse.x, (short)event.mouse.y };
			mouse = pr_convert_mouse(mouse, al_get_display_width(display), al_get_display_height(display));
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (event.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP:		case ALLEGRO_KEY_Z:			key[KEY_UP] = FALSE;		break;
				case ALLEGRO_KEY_LEFT:		case ALLEGRO_KEY_Q:			key[KEY_LEFT] = FALSE;		break;
				case ALLEGRO_KEY_DOWN:		case ALLEGRO_KEY_S:			key[KEY_DOWN] = FALSE;		break;
				case ALLEGRO_KEY_RIGHT:		case ALLEGRO_KEY_D:			key[KEY_RIGHT] = FALSE;		break;
			}
		}
		// Si un clic de souris est effectué
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			// Clic gauche : on attaque
			if (event.mouse.button & 1)
			{
				// Alors on vérifie le type d'attaque choisi et si le cooldown est respecté, alors on déclenche l'attaque en question
				if (hero->attack_mode == ATTACK_MODE_CAC && (double)clock() / CLOCKS_PER_SEC - last_cac_attack >= CAC_ATTACK_COOLDOWN && !hero->attack)
				{
					hero->attack = HERO_IS_ATTACKING_CAC;
					pr_hero_change_image(hero, 0);
					last_cac_attack = (double)clock() / CLOCKS_PER_SEC;

					for (i = 0; i < 256; i++)
					{
						// Pour chaque ennemi sur lequel on a cliqué
						if (mouse.x >= ennemys[i]->way->all_points[ennemys[i]->way_index].x &&
							mouse.x <= ennemys[i]->way->all_points[ennemys[i]->way_index].x + ennemys[i]->width &&
							mouse.y >= ennemys[i]->way->all_points[ennemys[i]->way_index].y &&
							mouse.y <= ennemys[i]->way->all_points[ennemys[i]->way_index].y + ennemys[i]->height)
						{
							// On vérifie s'il est à portée du héro
							if (abs(ennemys[i]->way->all_points[ennemys[i]->way_index].x - hero->position.x) <= 50 &&
								abs(ennemys[i]->way->all_points[ennemys[i]->way_index].y - hero->position.y) <= 50)
							{
								ennemys[i]->life -= hero->cac_damages;
								if (ennemys[i]->life < 0)
									score += 25;
								break;
							}
						}
					}

				}
				else if (hero->attack_mode == ATTACK_MODE_FRB && (double)clock() / CLOCKS_PER_SEC - last_frb_attack >= FRB_ATTACK_COOLDOWN && !hero->attack)
				{
					last_frb_attack = pr_fireball_attack(fireball, hero, mouse);
				}
			}
			// Clic droit : on change le mode d'attaque
			else
			{
				hero->attack_mode = !hero->attack_mode;
			}
		}
		else if (event.type == ALLEGRO_EVENT_TIMER)
		{
			if (event.timer.source == main_timer)
			{
				screen = al_create_bitmap(1280, 768);
				al_set_target_bitmap(screen);

				if (lifes < 1)
				{
					game_over = (PROJECT_TEXT_ZONE*)malloc(sizeof(PROJECT_TEXT_ZONE));

					fond = al_load_bitmap("resources/images/fond.png");
					resized_fond = pr_resize_bitmap(fond, al_get_display_width(display), al_get_display_height(display));
					pr_game_over(display, game_over, score, resized_fond);

					al_destroy_bitmap(fond);
					al_destroy_bitmap(resized_fond);
					free(game_over);

					al_flush_event_queue(queue);

					al_rest(3.0);

					al_wait_for_event(queue, &event);

					exit(0);
				}


				// On affiche la carte en fond
				al_draw_bitmap(map, 0, 0, 0);

				// On déplace le héro en fonction des touches enfoncées
				pr_move_hero(key, hero);

				// L'orientation de l'image du héro change en fonction de la position de la souris par rapport à lui
				al_draw_bitmap(hero->current_image, hero->position.x, hero->position.y, mouse.x < hero->position.x + hero->width / 2);
				
				// On affiche la boule de feu si celle-ci est active
				if (fireball->is_active)
					al_draw_bitmap(fireball->image, fireball->position.x, fireball->position.y, 0);

				// Pour chaque ennemi, s'il est encore en vie on l'affiche ainsi que sa barre de vie
				for (i = 0; i < number_of_ennemys; i++)
				{
					if (ennemys[i]->life > 0)
					{
						al_draw_bitmap(ennemys[i]->mvt[ennemys[i]->current_image], ennemys[i]->way->all_points[ennemys[i]->way_index].x, ennemys[i]->way->all_points[ennemys[i]->way_index].y, 0);
						pr_print_life_bar(ennemys[i]);
					}
				}

				switch (hero->attack_mode)
				{
					case ATTACK_MODE_CAC:
						al_draw_bitmap(attack_bmps[0], mouse.x - al_get_bitmap_width(attack_bmps[0]) / 2, mouse.y - al_get_bitmap_height(attack_bmps[0]) / 2, 0);
						break;
					case ATTACK_MODE_FRB:
						// Si l'attaque boule de feu est disponible, on affiche la cible rouge
						if ((double)clock() / CLOCKS_PER_SEC - last_frb_attack >= FRB_ATTACK_COOLDOWN)
							al_draw_bitmap(attack_bmps[1], mouse.x - al_get_bitmap_width(attack_bmps[1]) / 2, mouse.y - al_get_bitmap_height(attack_bmps[1]) / 2, 0);
						// Sinon on affiche la cible grisée
						else
							al_draw_bitmap(attack_bmps[2], mouse.x - al_get_bitmap_width(attack_bmps[2]) / 2, mouse.y - al_get_bitmap_height(attack_bmps[2]) / 2, 0);
				}

				al_set_target_backbuffer(display);

				al_draw_bitmap(screen, 0, 0, 0);

				if (al_get_display_width(display) != 1280 || al_get_display_height(display) != 768)
				{
					redim_screen = pr_resize_bitmap(screen, al_get_display_width(display), al_get_display_height(display));
					al_draw_bitmap(redim_screen, 0, 0, 0);

					al_destroy_bitmap(redim_screen);
				}
				
				al_destroy_bitmap(screen);

				al_flip_display();
			}
			else if (event.timer.source == animation_timer)
			{
				// A intervalle de temps régulier, on change les images des ennemis pour les animations
				for (i = 0; i < number_of_ennemys; i++)
					pr_ennemy_change_image(ennemys[i]);
				pr_hero_change_image(hero, -1);
			}
			else if (event.timer.source == fireball_timer && fireball->is_active)
			{
				pr_move_fireball(fireball);

				// Pour tous les ennemis touchés
				for (i = 0; i < number_of_ennemys; i++)
				{
					if (fireball->position.x >= ennemys[i]->way->all_points[ennemys[i]->way_index].x &&
						fireball->position.x <= ennemys[i]->way->all_points[ennemys[i]->way_index].x + ennemys[i]->width &&
						fireball->position.y >= ennemys[i]->way->all_points[ennemys[i]->way_index].y &&
						fireball->position.y <= ennemys[i]->way->all_points[ennemys[i]->way_index].y + ennemys[i]->height)
					{
						// On enlève de la vie à tous les ennemis touchés
						ennemys[i]->life -= fireball->damages;

						if (ennemys[i]->life < 0 && ennemys[i]->way_index > 0)
							score++;

						// Puis on désactive la boule de feu en la mettant directement à sa derniere position
						hero->attack = HERO_IS_NOT_ATTACKING;
					}
				}
			}
			else if (event.timer.source == ennemy_timer)
			{
				// Pour chaque ennemi
				for (i = 0; i < number_of_ennemys; i++)
				{
					// On l'avance de 1 pas sur son chemin
					if (ennemys[i]->way_index < ennemys[i]->way->size && ennemys[i]->life > 0)
						ennemys[i]->way_index++;

					// S'il est arrivé au bout, on perd une vie
					if (ennemys[i]->way_index == ennemys[i]->way->size)
						lifes--;
				}
			}
			else if (event.timer.source == ennemy_frequency_timer)
			{
				ennemy_frequency -= 0.5;

				//ennemys = pr_add_new_ennemy(ennemys, &number_of_ennemys, ways[rand() % number_of_ways], ennemy_bmps);
				//if (ennemys == NULL)
				//	pr_exit_with_last_error();

				for (i = 0; i < number_of_ennemys; i++)
				{
					if (ennemys[i]->life <= 0)
					{
						ennemys[i]->life = ennemys[i]->max_life;
						ennemys[i]->way_index = 0;
						ennemys[i]->way = ways[rand() % number_of_ways];
						break;
					}
				}

				if (ennemy_frequency <= 1.0)
					ennemy_frequency = 5.0;
				al_set_timer_speed(ennemy_frequency_timer, ennemy_frequency);
			}
		}
	}


	// Quitter le programme revient à libérer toute la mémoire allouée, en partant des objets (ennemis, etc)
	// puis en terminant par libérer la mémoire du display

	// Libération de la mémoire de la boule de feu et du héro
	pr_free_fireball(fireball);
	pr_free_hero(hero);
	
	// Libération de la mémoire des images des ennemis
	for (i = 0; i < 4; i++)
		al_destroy_bitmap(ennemy_bmps[i]);

	al_destroy_bitmap(attack_bmps[0]);
	al_destroy_bitmap(attack_bmps[1]);
	al_destroy_bitmap(attack_bmps[2]);
	
	// Et de la carte
	al_destroy_bitmap(map);
	//al_destroy_bitmap(screen);
	//al_destroy_bitmap(redim_screen);

	// Libération de la mémoire des minuteurs
	al_destroy_timer(main_timer);
	al_destroy_timer(animation_timer);
	al_destroy_timer(fireball_timer);
	al_destroy_timer(ennemy_timer);
	// De la file d'évènement
	al_destroy_event_queue(queue);
	// Et du display
	al_destroy_display(display);

	return 0;
}