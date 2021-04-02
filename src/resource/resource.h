#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <allegro5/allegro.h>

#include "resource_font.h"

#define     KD_RESOURCE_FOLDER       "resources"

ALLEGRO_PATH* kd_get_resources_path(const char* folder);

#endif