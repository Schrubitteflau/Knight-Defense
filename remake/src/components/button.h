#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdlib.h>
#include <stdbool.h>

struct KD_BUTTON {
    // Position
    int x, y;
    // Taille
    int tx, ty;
    // Texte
    char* text;
};

struct KD_BUTTON_ATTRIBUTES {
    int flags;
};

typedef struct KD_BUTTON KD_BUTTON;
typedef struct KD_BUTTON_ATTRIBUTES KD_BUTTON_ATTRIBUTES;

#endif // __BUTTON_H__
