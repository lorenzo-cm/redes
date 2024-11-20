#include "maze.h"

char NUM2CHAR[6] = {'#', '_', '>', 'X', '?', '+'};
char* NUM2MOVE[5] = {"", "up", "right", "down", "left"};

int direction2num(char* str){
    if(strcmp(str, "up")){
        return 1;
    }
    else if(strcmp(str, "right")){
        return 2;
    }
    else if(strcmp(str, "down")){
        return 3;
    }
    else if(strcmp(str, "left")){
        return 4;
    }
    return -1;
}

int is_move_legal(Maze* maze, int new_x, int new_y) {
    if (new_x < 0 || new_x >= maze->size || new_y < 0 || new_y >= maze->size) {
        return -1;
    }

    if (maze->board[new_x][new_y] == 0) {
        return -1;
    }

    return 1;
}

void get_legal_moves(Maze* maze, int* legal_moves) {
    int dx[4] = {-1, 0, 1, 0};  // changes in x for up, right, down, left
    int dy[4] = { 0, 1, 0,-1};  // changes in y for up, right, down, left

    for (int i = 0; i < 4; i++) {
        int new_x = maze->px + dx[i];
        int new_y = maze->py + dy[i];
        if (is_move_legal(maze, new_x, new_y)) {
            legal_moves[i] = 1; 
        }
        else{
            legal_moves[i] = 0;
        }
    }
}

void read_maze(Maze *maze, char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    int row = 0, col = 0;
    char buffer[80];
    memset(buffer, 0, 80);

    while(fgets(buffer, sizeof(buffer), file)){
        col = 0;

        for(int i = 0; i<sizeof(buffer)/sizeof(char); i++){
            if(buffer[i] >= '0' && buffer[i] <= '6'){
                maze->board[row][col] = buffer[i] - '0';
                col++;
            }

            if (maze->board[row][col] == 2) {
                    maze->px = row;
                    maze->py = col;
            }
        }
        row++;
    }

    fclose(file);

    maze->size = row;
}

int make_move(Maze *maze, int new_x, int new_y){
    if(is_move_legal(maze, new_x, new_y)){
        maze->board[new_x][new_y] = 1;
        maze->px = new_x;
        maze->py = new_y;
        return 1;
    }

    return -1;
}

void maze2string(Maze maze, char* buf) {
    char c;
    int count = 0;

    for (int i = 0; i < maze.size; i++) {
        for (int j = 0; j < maze.size; j++) {
            int val = maze.board[i][j];
            if (val >= 0 && val < 6) {
                c = NUM2CHAR[val];
            }
            else if (j < maze.size - 1) {
                c = '\t';
            }

            buf[count] = c;
            count++;
        }
        buf[count] = '\n';
        count++;
    }
    buf[count] = '\0';
}