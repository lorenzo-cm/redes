#pragma once
#include "maze.h"
#include "common_maze.h"

extern Maze server_maze;
extern int started;

void server_start(int argc, char** argv);
void server_get_moves(struct action* action);
void server_move(int move_index);
void server_hint(struct action* action);
int server_is_win();
void server_reset(int argc, char** argv);

void server_action_handler(int argc, char** argv, struct action* recv_action, struct action* send_action);