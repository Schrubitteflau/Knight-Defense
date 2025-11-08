#include "button.h"

bool kd_create_button_attributes(const KD_BUTTON_ATTRIBUTES** attrs)
{
    *attrs = malloc(sizeof(*attrs));

    if (*attrs == NULL)
    {
        return false;
    }

    kd_reset_button_attributes(*attrs);

    return true;
}

void kd_reset_button_attributes(KD_BUTTON_ATTRIBUTES* attrs)
{
    attrs->flags = 0;
}

bool kd_create_button(KD_BUTTON** button)
{
    *button = malloc(sizeof(*button));

    if (*button == NULL)
    {
        return false;
    }

    kd_init_button(*button);

    return true;
}

void kd_init_button(KD_BUTTON* button)
{
    button->x = 0;
    button->y = 0;
    button->tx = 0;
    button->ty = 0;
    button->text = NULL;
}

void kd_destroy_button(KD_BUTTON *button)
{
    if (button != NULL)
    {

    }
}