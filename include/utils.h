/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** utils
*/

#ifndef INCLUDED_UTILS_H
    #define INCLUDED_UTILS_H
    #include "call_csfml.h"
    #define T_BRUT 1
    #define T_MUTANT 2
    #define T_SS 3
    #define T_M_BG 4
    #define T_M2D 5
    #define T_M3D 6
    #define T_P_2D 7
    #define T_B_S1 8
    #define T_B_S2 9
    #define T_B_S3 10
    #define T_B_EXIT 11
    #define T_B_SETTINGS 12
    #define T_ARR_L 13
    #define T_ARR_R 14
    #define T_B_HOME 15

typedef struct game_handler_s game_handler_t;

typedef struct img_s {
    sfVector2f pos;
    sfIntRect rect;
    sfTexture *texture;
    sfSprite *sprite;
    size_t texture_id;
} img_t;

//tab where texture are malloc;
typedef struct tab_texture_s {
    sfTexture *texture;
    size_t id;
    char *file;
} tab_texture_t;

sfTexture *get_texture(tab_texture_t *tab, int token);
img_t *set_img(size_t texture_token, sfIntRect rect, sfVector2f pos,
    game_handler_t *g_h);
sfText *set_txt(size_t size, sfColor color, sfVector2f position, char *str);
#endif
