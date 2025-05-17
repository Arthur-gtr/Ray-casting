/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/


#ifndef LEVEL_H
    #define LEVEL_H
    # define TILE_SIZE 60
    # define MAP_WIDTH 100
    # define MAP_HEIGHT 100
    #include <stdbool.h>

/*
** Variable externe
** qui sera changé
** par des fichier de
** config
*/
extern int const map[MAP_HEIGHT][MAP_WIDTH];

/* define 3 map 100 x 100 will be set in config file or
 set in the .h if no cofig file */
typedef struct level_s {
    int actual_level;
    int map[3][MAP_HEIGHT][MAP_WIDTH];
} level_t;
bool is_wall(int x, int y);

#endif //MAP_H
