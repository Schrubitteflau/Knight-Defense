#ifndef __RESOURCE_FONT_H__
#define __RESOURCE_FONT_H__

#include <stdbool.h>

#define     KD_FONT_FOLDER      "fonts"

enum KD_FONT
{
    KD_FONT_DONKEYSTONE,
    KD_FONT_LOADCOAT,

    KD_FONT_COUNT
};

extern const char* kd_font_filenames[];

bool kd_load_font(enum KD_FONT font_index);

#endif