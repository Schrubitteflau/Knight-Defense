#ifndef __RESOURCE_FONT_H__
#define __RESOURCE_FONT_H__

#include <stdbool.h>
#include <allegro5/allegro_font.h>

#define     KD_FONT_FOLDER      "fonts"

enum KD_FONT
{
    KD_FONT_DONKEYSTONE,
    KD_FONT_LOADCOAT,

    KD_FONT_COUNT
};

extern const char* kd_font_filenames[];

ALLEGRO_FONT* kd_get_font(enum KD_FONT font_index);
bool kd_is_font_loaded(enum KD_FONT font_index);
void kd_set_font(enum KD_FONT font_index, ALLEGRO_FONT *font);
const char* kd_get_font_filename(enum KD_FONT font_index);
bool kd_load_font(enum KD_FONT font_index);
void kd_unload_font(enum KD_FONT font_index);
void kd_unload_all_fonts(void);

#endif // __RESOURCE_FONT_H__
