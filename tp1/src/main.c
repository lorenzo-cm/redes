#include <stdio.h>
#include "maze.h"

int main(int argc, char **argv){

    char* filename = "data/in.txt";

    Maze maze;

    read_maze(&(maze), filename);

    char buf[300];
    maze2string(maze, buf);

    printf("%s\n", buf);

    return 0;
}