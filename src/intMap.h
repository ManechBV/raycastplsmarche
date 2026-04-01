#ifndef INT_MAP_H
# define INT_MAP_H

typedef struct s_intMap
{
    int             **map;
    unsigned int    w;
    unsigned int    h;
    int             scale;
}   t_intMap;

t_intMap    *create_intMap(unsigned int w, unsigned int h, int scale);
void        free_intMap(t_intMap *map);
void        draw_intMap(t_intMap *map);
void        generate_random_intMap(t_intMap *map, int max_val, int occ);
void        generate_border_walls(t_intMap *map);

#endif
