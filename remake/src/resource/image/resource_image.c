#include "resource.h"

static ALLEGRO_BITMAP* kd_images[KD_IMAGE_COUNT] = { NULL };

ALLEGRO_BITMAP* kd_get_image(enum KD_IMAGE image_index)
{
    return kd_images[image_index];
}

bool kd_is_image_loaded(enum KD_IMAGE image_index)
{
    return (kd_get_image(image_index) != NULL);
}

void kd_set_image(enum KD_IMAGE image_index, ALLEGRO_BITMAP *image)
{
    kd_images[image_index] = image;
}

const char* kd_get_image_filename(enum KD_IMAGE image_index)
{
    return kd_image_filenames[image_index];
}

bool kd_load_image(enum KD_IMAGE image_index)
{
    ALLEGRO_PATH *image_path;
    const char *path_str;
    ALLEGRO_BITMAP *image;

    if (kd_is_image_loaded(image_index)) return true;

    image_path = kd_get_resources_path(KD_IMAGE_FOLDER);
    al_set_path_filename(image_path, kd_get_image_filename(image_index));

    path_str = al_path_cstr(image_path, ALLEGRO_NATIVE_PATH_SEP);
    image = al_load_bitmap(path_str);
    if (image == NULL)
    {
        kd_error_format("al_load_bitmap(%s)", path_str);
        return false;
    }
    kd_set_image(image_index, image);

    return true;
}

void kd_unload_image(enum KD_IMAGE image_index)
{
    if (kd_is_image_loaded(image_index))
    {
        al_destroy_bitmap(kd_get_image(image_index));
    }

    kd_set_image(image_index, NULL);
}

void kd_unload_all_images(void)
{
    for (int i = 0; i < KD_IMAGE_COUNT; i++)
    {
        kd_unload_image(i);
    }
}