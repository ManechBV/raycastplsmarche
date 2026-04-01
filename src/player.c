#include <math.h>
#include <stdlib.h>
#include "raylib.h"

#include "utils.h"
#include "player.h"

t_player    *create_player(float x, float y, float dir, float fov)
{
    t_player    *new;

    new = malloc(sizeof(t_player));
    if (!new)
        return (NULL);
    new->x = x;
    new->y = y;
    new->dir = dir;
    new->fov = fov;
    return (new);
}

void    free_player(t_player *player)
{
    free(player);
}

void    update_player(t_player *player)
{
    float   speed = 2.0f;
    float   dir_speed = deg_to_rad(4.0f);
    
    if (IsKeyDown(KEY_W))
    {
        player->x += sinf(player->dir) * speed;
        player->y += cosf(player->dir) * speed;
    }
    if (IsKeyDown(KEY_S))
    {
        player->x -= sinf(player->dir) * speed;
        player->y -= cosf(player->dir) * speed;
    }
    if (IsKeyDown(KEY_A))
    {
        player->dir += dir_speed;
        if (player->dir > deg_to_rad(360))
            player->dir = player->dir - deg_to_rad(360);
    }
    if (IsKeyDown(KEY_D))
    {
        player->dir -= dir_speed;
        if (player->dir < 0)
            player->dir = deg_to_rad(360) + player->dir;
    }
}

void    draw_player(t_player *player)
{
    float line_lenght = 30;

    DrawLineEx((Vector2){player->x, player->y},
            (Vector2){player->x + (sinf(player->dir + (player->fov) / 2) * line_lenght),
            player->y + (cosf(player->dir + (player->fov / 2)) * line_lenght)},
            2.0f,
            RED);
    DrawLineEx((Vector2){player->x, player->y},
            (Vector2){player->x + (sinf(player->dir - (player->fov / 2)) * line_lenght),
            player->y + (cosf(player->dir - (player->fov / 2)) * line_lenght)},
            2.0f,
            RED);
    DrawLineEx((Vector2){player->x, player->y},
            (Vector2){player->x + (sinf(player->dir) * line_lenght),
            player->y + (cosf(player->dir) * line_lenght)},
            4.0f,
            BLUE);
    DrawCircle(player->x, player->y, 10, GREEN);
}
