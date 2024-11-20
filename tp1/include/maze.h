#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int visited[10][10];
    int board[10][10];
    int moves[100];
    int px;
    int py;
    int size;
} Maze;

extern char NUM2CHAR[6];
extern char* NUM2MOVE[5];

int direction2num(char* str);

int is_move_legal(Maze* maze, int new_x, int new_y);

void get_legal_moves(Maze* maze, int* legal_moves);

void read_maze(Maze *maze, char* filename);

int make_move(Maze *maze, int new_x, int new_y);

void maze2string(Maze maze, char* buf);
