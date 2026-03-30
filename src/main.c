#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "intMap.h"

typedef struct s_player
{
    float   x;
    float   y;
    float   dir;
    float   fov;
}   t_player;

float   deg_to_rad(float deg)
{
    return (deg / (180.0f / M_PI));
}

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

static bool calc_map_xy(float x, float y, float _dx, float _dy, float s, t_intMap *m, float dir, int *map_x, int *map_y)
{
    if (dir < deg_to_rad(180))
    {
        *map_x = (int)((x + _dx) / s);
        *map_y = (int)((y + _dy) / s);
    }
    else
    {
        *map_x = (int)((x + _dx) / s) - 1;
        *map_y = (int)((y + _dy) / s);
    }
    if (*map_x >= m->w || *map_y >= m->h || *map_x < 0 || *map_y < 0)
        return false;
    return true;
}

static bool calc_coll_vertical(float x, float y, float dir, float s, float *dx, float *dy, t_intMap *m)
{
    float   initial_x = x;
    float   initial_y = y;
    float   _dx = *dx;
    float   _dy = *dy;
    bool    wall = false;
    int     map_x = 0;
    int     map_y = 0;

    if (dir < deg_to_rad(180))
        _dx = (int)s - ((int)x % (int)s);
    else
        _dx = 0 - ((int)x % (int)s);

    if (dir < deg_to_rad(90))
        _dy = tanf(dir + deg_to_rad(90)) * _dx;
    else
        _dy = tanf(dir - deg_to_rad(90)) * _dx;
    _dy *= -1;

//    DrawCircle(x + _dx, y + _dy, 2, GREEN);
    if (!calc_map_xy(x, y, _dx, _dy, s, m, dir, &map_x, &map_y))
        return false;
    else
    {
        if (m->map[map_y][map_x] == 1)
            wall = true;
        else
        {
            x += _dx;
            y += _dy;
            if (dir < deg_to_rad(180))
                _dx = s;
            else
                _dx = s * -1;

            if (dir < deg_to_rad(90))
                _dy = tanf(dir + deg_to_rad(90)) * _dx;
            else
                _dy = tanf(dir - deg_to_rad(90)) * _dx;
            _dy *= -1;
        }
        if (!calc_map_xy(x, y, _dx, _dy, s, m, dir, &map_x, &map_y))
            return false;
        if (m->map[map_y][map_x] == 1)
            wall = true;

        while (!wall)
        {
 //           DrawCircle(x + _dx, y + _dy, 2, GREEN);
            x += _dx;
            y += _dy;
            x = roundf(x);
            y = roundf(y);

            if (!calc_map_xy(x, y, _dx, _dy, s, m, dir, &map_x, &map_y))
                return false;
            if (m->map[map_y][map_x] == 1)
                wall = true;
        }
    }

    *dx = (x + _dx) - initial_x;
    *dy = (y + _dy) - initial_y;
    return true;
}

static bool calc_map_xy_horizontal(float x, float y, float _dx, float _dy, float s, t_intMap *m, float dir, int *map_x, int *map_y)
{
    if (dir < deg_to_rad(90) || dir > deg_to_rad(270))
    {
        *map_x = (int)((x + _dx) / s);
        *map_y = (int)((y + _dy) / s);
    }
    else
    {
        *map_x = (int)((x + _dx) / s);
        *map_y = (int)((y + _dy) / s) - 1;
    }
    if (*map_x >= m->w || *map_y >= m->h || *map_x < 0 || *map_y < 0)
        return false;
    return true;
}

static bool calc_coll_horizontal(float x, float y, float dir, float s, float *dx, float *dy, t_intMap *m)
{
    float   initial_x = x;
    float   initial_y = y;
    float   _dx = *dx;
    float   _dy = *dy;
    bool    wall = false;
    int     map_x = 0;
    int     map_y = 0;

    if (dir < deg_to_rad(90) || dir > deg_to_rad(270))
        _dy = (int)s - ((int)y % (int)s);
    else
        _dy = 0 - ((int)y % (int)s);
    _dx = tanf(dir)  * _dy;

  //  DrawCircle(x + _dx, y + _dy, 2, GREEN);
    if (!calc_map_xy_horizontal(x, y, _dx, _dy, s, m, dir, &map_x, &map_y))
        return false;
    else
    {
        if (m->map[map_y][map_x] == 1)
            wall = true;
        else
        {
            x += _dx;
            y += _dy;
            if (dir < deg_to_rad(90) || dir > deg_to_rad(270))
                _dy = s;
            else
                _dy = s * -1;
            _dx = tanf(dir)  * _dy;
        }
        if (!calc_map_xy_horizontal(x, y, _dx, _dy, s, m, dir, &map_x, &map_y))
            return false;
        if (m->map[map_y][map_x] == 1)
            wall = true;

        while (!wall)
        {
   //         DrawCircle(x + _dx, y + _dy, 2, GREEN);
            x += _dx;
            y += _dy;
            x = roundf(x);
            y = roundf(y);

            if (!calc_map_xy_horizontal(x, y, _dx, _dy, s, m, dir, &map_x, &map_y))
                return false;
            if (m->map[map_y][map_x] == 1)
                wall = true;
        }
    }

    *dx = (x + _dx) - initial_x;
    *dy = (y + _dy) - initial_y;
    return true;
}

static float  dist(float x1, float y1, float x2, float y2)
{
    return sqrtf(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

int    cast_ray(float x, float y, float dir, int s, t_intMap *m)
{
    float vx = 0;
    float vy = 0;
    float hx = 0;
    float hy = 0;

    bool rayh = calc_coll_horizontal(x, y, dir, s, &hx, &hy, m);
    bool rayv = calc_coll_vertical(x, y, dir, s, &vx, &vy, m);
    int distv = abs(dist(x, y, x + vx, y + vy));
    int disth = abs(dist(x, y, x + hx, y + hy));
    int best = 0;

    if (rayh == false)
    {
        DrawLineEx((Vector2){x, y}, (Vector2){x + vx, y + vy}, 1.0f, BLUE);
        if (rayv)
            best = distv;
    }
    if (rayv == false)
    {
        DrawLineEx((Vector2){x, y}, (Vector2){x + hx, y + hy}, 1.0f, BLUE);
        if (rayh)
            best = disth;
    }
    else if (abs(dist(x, y, x + vx, y + vy)) < abs(dist(x, y, x + hx, y + hy)))
    {
        DrawLineEx((Vector2){x, y}, (Vector2){x + vx, y + vy}, 1.0f, BLUE);
        if (rayv)
            best = distv;
    }
    else
    {
        DrawLineEx((Vector2){x, y}, (Vector2){x + hx, y + hy}, 1.0f, BLUE);
        if (rayh)
            best = disth;
    }
    return best;
}

void    cast_unique_ray(t_player *p, t_intMap *m, int s)
{
    float x = p->x;
    float y = p->y;
    float dx = 0.0f;
    float dy = 0.0f;
    float dir = p->dir - (p->fov / 2);

    if (dir < 0)
        dir = deg_to_rad(360) + dir;

    int max_i = 200;
    int i = 0;
    int len;
    Color col = {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = 255,
    };
    while (i < max_i)
    {
        len = cast_ray(x, y, dir, s, m);
        len = 400 - len;
        col.r = len - 260;
        col.g = col.r;
        col.b= col.r;
        if (len != 400)
            DrawLineEx((Vector2){(i * 800) / max_i, 400 - len / 2}, (Vector2){(i * 800) / max_i, 400 + len / 2}, 4, col);

        dir += p->fov / max_i;
        if (dir > deg_to_rad(360))
            dir = deg_to_rad(360) - dir;
        i++;
    }
}

int main(void)
{
    InitWindow(1280, 720, "Raycaster by Namek");
    SetTargetFPS(60);

    int Scale = 40;

    t_intMap    *Map;
    Map = create_intMap(16, 16);
    if (!Map)   return (-1);

    generate_random_intMap(Map, 4);

    t_player    *Player;
    Player = create_player(200.0f, 200.0f, 0.0f, deg_to_rad(60.0f)); 
    if (!Player)   return (-1);

    while (!WindowShouldClose())
    {
        update_player(Player);
        BeginDrawing();
            ClearBackground(BLACK);
            draw_intMap(Map, Scale);
            draw_player(Player);
            cast_unique_ray(Player, Map, Scale);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
