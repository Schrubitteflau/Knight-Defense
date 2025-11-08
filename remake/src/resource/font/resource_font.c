#include "resource.h"

static ALLEGRO_FONT* kd_fonts[KD_FONT_COUNT] = { NULL };

ALLEGRO_FONT* kd_get_font(enum KD_FONT font_index)
{
    return kd_fonts[font_index];
}

bool kd_is_font_loaded(enum KD_FONT font_index)
{
    return (kd_get_font(font_index) != NULL);
}

void kd_set_font(enum KD_FONT font_index, ALLEGRO_FONT *font)
{
    kd_fonts[font_index] = font;
}

const char* kd_get_font_filename(enum KD_FONT font_index)
{
    return kd_font_filenames[font_index];
}

bool kd_load_font(enum KD_FONT font_index)
{
    ALLEGRO_PATH *font_path;
    const char *path_str;
    ALLEGRO_FONT *font;

    if (kd_is_font_loaded(font_index)) return true;

    font_path = kd_get_resources_path(KD_FONT_FOLDER);
    al_set_path_filename(font_path, kd_get_font_filename(font_index));

    path_str = al_path_cstr(font_path, ALLEGRO_NATIVE_PATH_SEP);
    font = al_load_font(path_str, 10, 0);
    if (font == NULL)
    {
        kd_error_format("al_load_font(%s)", path_str);
        return false;
    }
    kd_set_font(font_index, font);

    return true;
}

void kd_unload_font(enum KD_FONT font_index)
{
    if (kd_is_font_loaded(font_index))
    {
        al_destroy_font(kd_get_font(font_index));
    }

    kd_set_font(font_index, NULL);
}

void kd_unload_all_fonts(void)
{
    for (int i = 0; i < KD_FONT_COUNT; i++)
    {
        kd_unload_font(i);
    }
}