#include "common_maze.h"

int action2type(char* action) {
    if (0 == strcmp(action, "start\n")) {
        return 0;
    } else if (0 == strcmp(action, "up\n") || 0 == strcmp(action, "right\n") || 0 == strcmp(action, "down\n") || 0 == strcmp(action, "left\n")) {
        return 1;
    } else if (0 == strcmp(action, "map\n")) {
        return 2;
    } else if (0 == strcmp(action, "hint\n")) {
        return 3;
    } else if (0 == strcmp(action, "update\n")) {
        return 4;
    } else if (0 == strcmp(action, "win\n")) {
        return 5;
    } else if (0 == strcmp(action, "reset\n")) {
        return 6;
    } else if (0 == strcmp(action, "exit\n")) {
        return 7;
    } else {
        return -1;
    }
}

void reset_action(struct action* action){
    memset(&(action), 0, sizeof(action));
}