/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/

#ifndef ENTITIES_H
    #define ENTITIES_H
    #include "call_csfml.h"
    #include "time.h"
    #include "utils.h"
    #include "my.h"
    #define ID_WALL 1
    #define ID_PLAYER 2
    #define ID_SS 3
    #define ID_MUTANT 4
    #define ID_BRUT 5
    #define ID_PLAYER_MOVE 0
    #define ID_ENNEMIE_MOVE 1
    #define ID_OBJECT_MOVE 2
    #define W_ID_KNIFE 0
    #define W_ID_GUN 1
    #define W_ID_SHOTGUN 2
    #define W_ID_MINIGUN 3

typedef struct game_s game_t;
typedef struct ennemies_s ennemies_t;
typedef struct game_handler_s game_handler_t;

typedef struct entities_s {
    sfVector2f coord;
    sfVector2f dir;
    sfVector2f plan;
    img_t img;
    time_management_t clock;
    sfVector2f hb_2d;
    double angle;
    int id;
}entities_t;

typedef struct gun_s{
    int ammo;
    int magazine;
    int (*gun_shot)(game_t *, entities_t *);
    time_management_t clock;
    img_t img;
    sfSound *shoot_sound;
} gun_t;

typedef struct inventory{
    int nb_of_gun;
    int actual_gun;
    gun_t gun[4];
} inventory_t;

typedef struct player_s {
    entities_t *property;
    inventory_t *inventory;
    int speed;
    int life;
    sfSound *step;
    sfSoundBuffer *sound_buff;
    time_management_t clock_sound;
}player_t;

typedef struct ennemies_s {
    entities_t *property;
    int (*gun_shot)(list_t *, player_t *, ennemies_t *);
    int speed;
    int life;
    sfSound *step;
    sfSoundBuffer *sound_buff;
    time_management_t clock_sound;
}ennemies_t;

typedef struct object_s {
    entities_t property;
    int (*effect)(game_handler_t *);
}object_t;

typedef struct entities_handler_s {
    player_t *player;
    list_t *object;
    list_t *ennemies;
    list_t *entities;
}entities_handler_t;

#endif //ENTITIES_H
