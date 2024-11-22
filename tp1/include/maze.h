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

int is_move_legal(Maze maze, int new_x, int new_y, int consider_barrier);

void get_legal_moves(Maze* maze, int* legal_moves);

void read_maze(Maze *maze, char* filename);

int make_move(Maze *maze, int move_index);

void maze2string(Maze maze, char* buf);

int is_win(Maze maze);

void gen_board_unvisited(Maze maze, int unvisited_board[10][10]);

void reveal_all(Maze* maze);

void reveal_around_player(Maze* maze);



typedef struct {
    int x;
    int y;
    int parent_index;
    int direction;
} Position;

void bfs(Maze maze, int* path);