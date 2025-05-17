/*
** EPITECH PROJECT, 2025
** Ray-casting
** File description:
** start_ray
*/
#include "call_csfml.h"
#include "init.h"
#include "level.h"
#include <time.h>
#include "game.h"

/*
typedef struct game_s {
    entities_handler_t ent;
    level_t level;
    p_uid_t *uid;
    score_t score;
} game_t;
*/

#define FOV 66

/*
** -1 => camera_x => 1
*/

typedef struct ray_cast_s{
    double camera_x;
    double raydir_x;
    double raydir_y;
    int map_x;
    int map_y;
    double delta_dist_x;
    double delta_dist_y;
    double perp_wall_dist;
    int step_x;
    int step_y;
    double side_dist_x;
    double side_dist_y;
    int hit;
    int side;
    //step for draw

    int draw_start;
    int draw_end;
}ray_cast_t;

void init_ray_cast(ray_cast_t *ray_cast, entities_t *player, int x)
{
    ray_cast->camera_x = 2 * x / (double)(1980) - 1; 
    ray_cast->raydir_x = player->dir.x + player->plan.x * ray_cast->camera_x;
    ray_cast->raydir_y = player->dir.y + player->plan.y * ray_cast->camera_x;
    ray_cast->map_x = (int)(player->coord.x);
    ray_cast->map_y = (int)(player->coord.y);
    ray_cast->delta_dist_x = (ray_cast->raydir_x == 0) ? 1e30 : fabs(1 / ray_cast->raydir_x);
    ray_cast->delta_dist_y = (ray_cast->raydir_y == 0) ? 1e30 : fabs(1 / ray_cast->raydir_y);
    ray_cast->hit = 0;
}

void prep_dda(ray_cast_t *ray_cast, entities_t *player)
{
    if (ray_cast->raydir_x < 0){
      ray_cast->step_x = -1;
      ray_cast->side_dist_x = (player->coord.x - ray_cast->map_x) * ray_cast->delta_dist_x;
    }
    else {
        ray_cast->step_x  = 1;
        ray_cast->side_dist_x = (ray_cast->map_x + 1.0 - player->coord.x ) * ray_cast->delta_dist_x;
    }
    if (ray_cast->raydir_y < 0){
        ray_cast->step_y = -1;
        ray_cast->side_dist_y  = (player->coord.y - ray_cast->map_y) * ray_cast->delta_dist_y;
    }
    else{
        ray_cast->step_y = 1;
        ray_cast->side_dist_y = (ray_cast->map_y + 1.0 - player->coord.y) * ray_cast->delta_dist_y;
    } 
}

void execute_dda(ray_cast_t *ray_cast, entities_t *player, int map[100][100])
{
    while (ray_cast->hit == 0) {
      if (ray_cast->side_dist_x < ray_cast->side_dist_y) {
        ray_cast->side_dist_x += ray_cast->delta_dist_x;
        ray_cast->map_x += ray_cast->step_x;
        ray_cast->side = 0;
      }
      else {
        ray_cast->side_dist_y += ray_cast->delta_dist_y;
        ray_cast->map_y += ray_cast->step_y;
        ray_cast->side = 1;
      }
      if (map[ray_cast->map_x][ray_cast->map_y] > 0) 
        ray_cast->hit = 1;
    }
}

void cancel_fisheye(ray_cast_t *ray_cast)
{
    if (ray_cast->side == 0)
        ray_cast->perp_wall_dist = (ray_cast->side_dist_x - ray_cast->delta_dist_x);
    else 
        ray_cast->perp_wall_dist = (ray_cast->side_dist_y - ray_cast->delta_dist_y);
}

#define H 1080

void det_line(ray_cast_t *ray_cast)
{
    int lineHeight = (int)(H / ray_cast->perp_wall_dist);
    int drawStart = -lineHeight / 2 + H / 2;
    int drawEnd = lineHeight / 2 + H / 2;

    ray_cast->draw_start = (drawStart < 0) ? 0 : drawStart;
    ray_cast->draw_end = (drawEnd >= H) ? H - 1 : drawEnd;
}

void draw_line(sfRenderWindow *window, int x, int drawStart, int drawEnd, sfColor color)
{
    sfRectangleShape *line = sfRectangleShape_create();
    int height = drawEnd - drawStart + 1;

    sfRectangleShape_setSize(line, (sfVector2f){1, height});
    sfRectangleShape_setPosition(line, (sfVector2f){x, drawStart});
    sfRectangleShape_setFillColor(line, color);

    sfRenderWindow_drawRectangleShape(window, line, NULL);
    sfRectangleShape_destroy(line);
}

void choose_color_wall(sfRenderWindow *window, ray_cast_t *ray_cast, int map[100][100], int x)
{
    sfColor color;

    switch(map[ray_cast->map_x][ray_cast->map_y])
    {
      case 1:  color = sfRed;  break; //red
      case 2:  color = sfGreen;  break; //green
      case 3:  color = sfBlue;   break; //blue
      case 4:  color = sfWhite;  break; //white
      default: color = sfYellow; break; //yellow
    }

    if (ray_cast->side == 1){
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
    }
    //draw the pixels of the stripe as a vertical line
    draw_line(window, x,ray_cast->draw_start, ray_cast->draw_end, color);
        
}

int execute_ray(entities_t *player, sfRenderWindow *window, int map[100][100])
{
    ray_cast_t ray_cast = {0};

    for (int x = 0; x < 1980; x++){
        init_ray_cast(&ray_cast, player, x);
        prep_dda(&ray_cast, player);
        execute_dda(&ray_cast, player, map);
        cancel_fisheye(&ray_cast);
        det_line(&ray_cast);
        choose_color_wall(window, &ray_cast, map, x);
    }
    return 0;
}

static
void moove_up(entities_t *entities)
{
    entities->coord.x += entities->dir.x / 100;
    entities->coord.y += entities->dir.y / 100;
}

void moove_player(entities_t *player)
{
    if (sfKeyboard_isKeyPressed(sfKeyZ))
        moove_up(player);
}

int start_ray(game_t *game_handler, sfRenderWindow *window)
{
    player_t *player = game_handler->ent.player;
    entities_t *player_e = player->property;
    sfEvent event;

    while (sfRenderWindow_isOpen(window)){
        //sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_pollEvent(window, &event);
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(window);
        //game_handler->level.map[0];
        sfRenderWindow_clear(window, sfBlack); 
        execute_ray(player_e, window, game_handler->level.map[0]);
        rotate_player(game_handler->ent.player->property, 0.01);
        moove_player(game_handler->ent.player->property);
        sfRenderWindow_display(window);
        
    }
    return 0;
}