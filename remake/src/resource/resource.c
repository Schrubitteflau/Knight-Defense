#include "resource.h"

ALLEGRO_PATH* kd_get_resources_path(const char *folder)
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, KD_RESOURCE_FOLDER);
    al_append_path_component(path, folder);
    return path;
}