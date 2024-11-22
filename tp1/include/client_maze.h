#pragma once

#include "common_maze.h"
#include "maze.h"

extern int client_started;
extern int client_win;

void client_print_moves(struct action action, int is_hint);
void client_print_error(const char* error);
void client_print_map(struct action action);
int client_check_move_legal(struct action action, char* buf);
int client_action_handler(struct action* recv_action, struct action* send_action, char* buf);
void client_pos_action_handler(struct action recv_action, int prev_type);