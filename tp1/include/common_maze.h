#pragma once
#include <string.h>

struct action {
    int type;
    int moves[100];
    int board[10][10];
};

int action2type(char* action);
void reset_action(struct action* action);