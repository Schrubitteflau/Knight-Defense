#ifndef __MENU_H__
#define __MENU_H__

#define		COLUMNS_TO_PRINT	3

#define		PRINT_BUTTON(a)		al_draw_bitmap(a.image, a.x, a.y, 0)

typedef struct PROJECT_TEXT_ZONE
{
	ALLEGRO_BITMAP *image;
	int x, y;
	int tx, ty;
} PROJECT_TEXT_ZONE;

void pr_destroy_text_zone(PROJECT_TEXT_ZONE *block);
PROJECT_BOOL pr_create_button(PROJECT_TEXT_ZONE *button, const char *font_path, ALLEGRO_COLOR font_color, int tx, int ty, const char *txt, ALLEGRO_COLOR background_color, int posx, int posy);
PROJECT_BOOL pr_create_all_resolutions_buttons(PROJECT_TEXT_ZONE **buttons, const char *font_path);
PROJECT_BOOL pr_print_resolution_menu(PROJECT_TEXT_ZONE *choose_button, PROJECT_TEXT_ZONE *fullscreen_button, PROJECT_TEXT_ZONE *continue_button, PROJECT_TEXT_ZONE **resolution_buttons);
int pr_get_index_button(int x, int y, PROJECT_TEXT_ZONE **buttons);
PROJECT_BOOL pr_choose_resolution(ALLEGRO_DISPLAY *d);
PROJECT_BOOL pr_print_main_menu(ALLEGRO_DISPLAY *d);
PROJECT_BOOL pr_print_scenario(ALLEGRO_DISPLAY *d);


#endif
