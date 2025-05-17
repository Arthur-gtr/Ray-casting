/*
** EPITECH PROJECT, 2025
** Wolf3D
** File description:
** menu
*/

#ifndef MENU_H_
    #define MENU_H_
    #include "call_csfml.h"
    #include "utils.h"
    #define M_START 0
    #define M_PAUSE 1
    #define M_SETTING 2
    #define M_END 3
    #define NB_MENU 4
typedef struct game_handler_s game_handler_t;

typedef struct boutons_s {
    img_t *released;
    sfRectangleShape *press;
    void (*action)(game_handler_t *);
} bouton_t;

typedef struct menu_s {
    img_t *background;
    sfText *text;
    list_t *boutons;
    bouton_t *actual_bouton;
} menu_t;

void init_menu(menu_t *menu);
#endif /* !MENU_H_ */
