/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/


#ifndef INIT_H
    #define INIT_H

    #include "call_csfml.h"
    #include "game.h"

sfRenderWindow *create_window(void);
int init_game_handler(game_handler_t *game_handler);
void init_player(player_t *player);
int init_wall(object_t *wall);
time_management_t init_clock(void);
int init_wall(object_t *wall);

#endif //INIT_H
