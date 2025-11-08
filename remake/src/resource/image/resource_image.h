#ifndef __RESOURCE_IMAGE_H__
#define __RESOURCE_IMAGE_H__

#include <stdbool.h>
#include <allegro5/allegro_image.h>

#define     KD_IMAGE_FOLDER      "images"

enum KD_IMAGE
{
    // Sprites d'attaque
    KD_IMAGE_ATTACK_1,
    KD_IMAGE_ATTACK_2,
    KD_IMAGE_ATTACK_3,
    KD_IMAGE_ATTACK_4,

    // Sprites du déplacement du héro
    KD_IMAGE_HERO_MOVEMENT_1,
    KD_IMAGE_HERO_MOVEMENT_2,
    KD_IMAGE_HERO_MOVEMENT_3,
    KD_IMAGE_HERO_MOVEMENT_4,

    // Sprites du déplacement d'un chevaliers
    KD_IMAGE_KNIGHT_MOVEMENT_1,
    KD_IMAGE_KNIGHT_MOVEMENT_2,
    KD_IMAGE_KNIGHT_MOVEMENT_3,
    KD_IMAGE_KNIGHT_MOVEMENT_4,

    // Sprites du cuseur de visée
    KD_IMAGE_TARGET_GREY,
    KD_IMAGE_TARGET_RED,

    // Sprite de la boule de feu
    KD_IMAGE_FIREBALL,

    // Sprite de l'épée
    KD_IMAGE_SWORD,

    // Fond et curseur du menu
    KD_IMAGE_MENU_BACKGROUND,
    KD_IMAGE_MENU_CURSOR,

    // Fond de la carte
    KD_IMAGE_MAP_BACKGROUND,

    // Scénario
    KD_IMAGE_SCENARIO,

    KD_IMAGE_COUNT
};

extern const char* kd_image_filenames[];

ALLEGRO_BITMAP* kd_get_image(enum KD_IMAGE image_index);
bool kd_is_image_loaded(enum KD_IMAGE image_index);
void kd_set_image(enum KD_IMAGE image_index, ALLEGRO_BITMAP *image);
const char* kd_get_image_filename(enum KD_IMAGE image_index);
bool kd_load_image(enum KD_IMAGE image_index);
void kd_unload_image(enum KD_IMAGE image_index);
void kd_unload_all_images(void);

#endif // __RESOURCE_IMAGE_H__
