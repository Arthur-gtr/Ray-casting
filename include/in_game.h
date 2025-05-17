/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** in_game
*/

#ifndef INCLUDED_IN_GAME_H
    #define INCLUDED_IN_GAME_H
    #include "entities.h"
    #include "time.h"
    #include "level.h"

typedef struct score_s {
    int score;
    time_management_t time_to_end;
} score_t;

typedef struct p_uid_s {
    img_t bg_player_info;
    sfText *time;
    sfText *ammo;
    inventory_t *inventory;
} p_uid_t;

typedef struct game_s {
    entities_handler_t ent;
    level_t level;
    p_uid_t *uid;
    score_t score;
} game_t;

#endif
