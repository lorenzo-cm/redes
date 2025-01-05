#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include <math.h>
#include <limits.h>
#include <stdio.h>

#define MAX_NEIGHBORS 101

struct Neighbor{
    int x;
    int y;
    float distance;
};


extern struct Neighbor neighbors[MAX_NEIGHBORS];

void init_neighbors(int my_x, int my_y);

int find_empty_index();

void add_neighbor(int x, int y);

void remove_neighbor(int x, int y);

int check_neighbor_top3(int x, int y);

int alredy_exists(int x, int y);

float calcularDistancia(int x, int y, int x2, int y2);

void print_neighbors();

#endif