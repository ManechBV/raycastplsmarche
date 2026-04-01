#include "intMap.h"
#include "raylib.h"
#include <stdlib.h>


t_intMap    *create_intMap(unsigned int w, unsigned int h, int scale)
{
    t_intMap    *new;

    new = calloc(1, sizeof(t_intMap));
    if (!new)
        return (NULL);
    new->w = w;
    new->h = h;
    new->scale = scale;
    new->map = malloc(sizeof(int *) * h);
    if (!new->map) 
    {
        free(new);
        return (NULL);
    }
    {
        int i = 0;
        while (i < h)
        {
            new->map[i] = calloc(w, sizeof(int));
            if (!new->map[i])
            {
                i--;
                while (i >= 0)
                {
                    free(new->map[i]);
                    i--;
                }
            }
            i++;
        }
    }
    return (new);
}

void    free_intMap(t_intMap *map)
{
    for (int i = 0; i < map->h; i++)
        free(map->map[i]);
    free(map->map);
    free(map);
}

void    draw_intMap(t_intMap *map)
{
    int x = 0;
    int y = 0;
    int scale = map->scale;
    Color   col = {
        .r = 255, .g = 255, .b = 255, .a = 255
    };

    while (y < map->h)
    {
        x = 0;
        while (x < map->w)
        {
            if (map->map[y][x] == 1)
                col.a = 255;
            else
                col.a = 155;
            DrawRectangle(x * scale, y * scale, scale - 1, scale - 1, col);
            x++;
        }
        y++;
    }
}

void    generate_random_intMap(t_intMap *map, int max_val, int occ)
{
    int x = 0;
    int y = 0;
    while (y < map->h)
    {
        x = 0;
        while (x < map->w)
        {
            if (GetRandomValue(0, occ) == 0)
                map->map[y][x] = GetRandomValue(0, max_val);
            x++;
        }
        y++;
    }
}

void    generate_border_walls(t_intMap *map)
{
    for (int y = 0; y < map->h; y++)
    {
        for (int x = 0; x < map->w; x++)
        {
            if (x == 0 || y == 0 || x == map->w - 1 || y == map->h - 1)
                map->map[y][x] = 1;
        }
    }
}
