/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/

#ifndef RAY_CASTING_H
    #define RAY_CASTING_H
    #include <stddef.h>
    #include "call_csfml.h"
    #define CLOSE_DIST 20000.0f

/*
** Node contient tous l arbre binaire
** qui contient lui meme tous ce
** qu' il faut afficher
*/
typedef struct buffer_z_s {
    double distance;
    double id;
    size_t type;
    sfColor shadow;
}buffer_z_t;

int start_ray(game_t *game_handler, sfRenderWindow *window);

#endif //RAY_CASTING_H
