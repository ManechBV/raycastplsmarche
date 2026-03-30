#include "intMap.h"
#include "raylib.h"
#include <stdlib.h>


t_intMap    *create_intMap(unsigned int w, unsigned int h)
{
    t_intMap    *new;

    new = calloc(1, sizeof(t_intMap));
    if (!new)
        return (NULL);
    new->w = w;
    new->h = h;
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

void    draw_intMap(t_intMap *map, int scale)
{
    int x = 0;
    int y = 0;
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

void    generate_random_intMap(t_intMap *map, int occ)
{
    int x = 0;
    int y = 0;
    while (y < map->h)
    {
        x = 0;
        while (x < map->w)
        {
            if (GetRandomValue(0, occ) == 0)
                map->map[y][x] = 1;
            x++;
        }
        y++;
    }
}
