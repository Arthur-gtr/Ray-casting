/*
** EPITECH PROJECT, 2025
** Ray-casting
** File description:
** wolf3d
*/
#include "call_csfml.h"
#include "init.h"
#include "level.h"
#include <time.h>
#include "game.h"

void fill_map(int map[100][100])
{
    for (int i = 0; i != 100; i++)
        for (int j = 0; j != 100; j++)
            map[i][j] = 0;
    for (int i = 0; i != 10; i++){
        map[i][0] = 1;
        map[i][10] = 2;
        map[0][i] = 3;
        map[10][i] = 4;
    }

    map[1][1] = 4;
}

void rotate_player(entities_t *player, float rot_speed)
{
    // Rotation vers la droite
    if (sfKeyboard_isKeyPressed(sfKeyRight)) {
        float old_dir_x = player->dir.x;
        player->dir.x = player->dir.x * cos(-rot_speed) - player->dir.y * sin(-rot_speed);
        player->dir.y = old_dir_x * sin(-rot_speed) + player->dir.y * cos(-rot_speed);

        float old_plane_x = player->plan.x;
        player->plan.x = player->plan.x * cos(-rot_speed) - player->plan.y * sin(-rot_speed);
        player->plan.y = old_plane_x * sin(-rot_speed) + player->plan.y * cos(-rot_speed);
    }

    // Rotation vers la gauche
    if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
        float old_dir_x = player->dir.x;
        player->dir.x = player->dir.x * cos(rot_speed) - player->dir.y * sin(rot_speed);
        player->dir.y = old_dir_x * sin(rot_speed) + player->dir.y * cos(rot_speed);

        float old_plane_x = player->plan.x;
        player->plan.x = player->plan.x * cos(rot_speed) - player->plan.y * sin(rot_speed);
        player->plan.y = old_plane_x * sin(rot_speed) + player->plan.y * cos(rot_speed);
    }
}

int wolf3d(void)
{
    game_t game_handler = {{0}, {0}, NULL, {0}};
    sfVideoMode videomode = {1920, 1080, 32};
    sfRenderWindow *window = sfRenderWindow_create(videomode,"wolf3d", sfDefaultStyle, NULL);
    
    game_handler.ent.player = malloc(sizeof(player_t));
    if (!game_handler.ent.player)
        return 84;
    game_handler.ent.player->property = malloc(sizeof(entities_t));
    if (!game_handler.ent.player->property)
        return 84;
    game_handler.ent.player->property->coord = (sfVector2f){5, 5};
    game_handler.ent.player->property->dir = (sfVector2f){-1, 0};///DIRECTION
    game_handler.ent.player->property->plan = (sfVector2f){0, 0.66};
    game_handler.ent.player->property->angle = 0;
    fill_map(game_handler.level.map[0]);
    start_ray(&game_handler, window);
    return 0;
}
