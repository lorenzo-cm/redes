#include "maze.h"

char NUM2CHAR[6] = {'#', '_', '>', 'X', '?', '+'};
char* NUM2MOVE[5] = {"", "up", "right", "down", "left"};

int direction2num(char* str){
    if(strcmp(str, "up\n") == 0){
        return 1;
    }
    else if(strcmp(str, "right\n") == 0){
        return 2;
    }
    else if(strcmp(str, "down\n") == 0){
        return 3;
    }
    else if(strcmp(str, "left\n") == 0){
        return 4;
    }
    return 0;
}

int is_move_legal(Maze maze, int new_x, int new_y, int consider_barrier) {
    if (new_x < 0 || new_x >= maze.size || new_y < 0 || new_y >= maze.size) {
        return 0;
    }

    if (maze.board[new_x][new_y] == 0 && consider_barrier) {
        return 0;
    }

    return 1;
}

void get_legal_moves(Maze* maze, int* moves) {
    int dx[4] = {-1, 0, 1, 0};  // changes in x for up, right, down, left
    int dy[4] = { 0, 1, 0,-1};  // changes in y for up, right, down, left

    int count = 0;
    for (int i = 0; i < 4; i++) {
        int new_x = maze->px + dx[i];
        int new_y = maze->py + dy[i];
        if (is_move_legal(*maze, new_x, new_y, 1)) {
            moves[count] = i+1;
            count++;
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
    memset(maze->board, -1, 10*10*4);
    memset(maze->visited, 0, 10*10*4);

    while(fgets(buffer, sizeof(buffer), file)){
        col = 0;

        for(int i = 0; i<sizeof(buffer)/sizeof(char); i++){
            if(buffer[i] >= '0' && buffer[i] <= '6'){
                maze->board[row][col] = buffer[i] - '0';
                if (maze->board[row][col] == 2) {
                    maze->px = row;
                    maze->py = col;
                    maze->visited[row][col] = 1;
                }
                col++;
            }
        }
        row++;
    }

    fclose(file);

    maze->size = row;

    reveal_around_player(maze);
}

int make_move(Maze *maze, int move_index){
    int return_value = 0;

    int dx[5] = {-100, -1, 0, 1, 0};
    int dy[5] = {-100, 0, 1, 0,-1};

    int new_x = maze->px + dx[move_index];
    int new_y = maze->py + dy[move_index];
    
    if(is_move_legal(*maze, new_x, new_y, 1)){
        maze->px = new_x;
        maze->py = new_y;
        return_value = 1;
    }

    // update visited
    if(return_value){
        reveal_around_player(maze);
    }
    
    return return_value;
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
            
            buf[count] = c;
            count++;
            
            if (j < maze.size - 1) {
                c = '\t';
                buf[count] = c;
                count++;
            }
        }
        buf[count] = '\n';
        count++;
    }
    buf[count] = '\0';
}

int is_win(Maze maze){
    if(maze.board[maze.px][maze.py] == 3){
        return 1;
    }
    return 0;
}

void gen_board_unvisited(Maze maze, int unvisited_board[10][10]){
    memset(unvisited_board, -1, 400);

    for (int i = 0; i < maze.size; i++) {
        for (int j = 0; j < maze.size; j++) {
            if(maze.visited[i][j] == 0){
                unvisited_board[i][j] = 4;
            }
            else if(i == maze.px && j == maze.py){
                unvisited_board[i][j] = 5;
            }
            else{ 
                unvisited_board[i][j] = maze.board[i][j];
            }
        }
    }
}

void reveal_all(Maze* maze){
    memset(maze->visited, 1, 4*10*10);
}

void reveal_around_player(Maze* maze){
    int dx[8] = {-1, 0, 1, 0,-1,-1, 1, 1};
    int dy[8] = { 0, 1, 0,-1,-1, 1,-1, 1};

    for(int i = 0; i<8; i++){
        int x = maze->px + dx[i];
        int y = maze->py + dy[i];
        if(is_move_legal(*maze, x, y, 0)){
            maze->visited[x][y] = 1;
        }
    }
}


void bfs(Maze maze, int *path) {
    // not passing a maze pointer cause i am afraid of changing it
    int visited[10][10] = {0};
    Position queue[100];
    int front = 0;
    int rear = 0;
    int found = 0;

    // start pos
    queue[rear++] = (Position){maze.px, maze.py, -1, -1};
    visited[maze.px][maze.py] = 1;

    while (front < rear) {
        Position current = queue[front++];

        // up, right, down, left
        int dx[4] = {-1, 0, 1, 0};
        int dy[4] = { 0, 1, 0,-1};

        // 1 for up, 2 for right, 3 for down, 4 for left
        int dir[4] = {1, 2, 3, 4};

        for (int i = 0; i < 4; i++) {
            int new_x = current.x + dx[i];
            int new_y = current.y + dy[i];

            // valid and not visited
            if (!visited[new_x][new_y] && is_move_legal(maze, new_x, new_y, 1)) {
                visited[new_x][new_y] = 1;

                // put in the queue
                queue[rear++] = (Position){new_x, new_y, front - 1, dir[i]};

                // check win -> must change the px and py
                int old_px = maze.px;
                int old_py = maze.py;
                maze.px = new_x;
                maze.py = new_y;

                if (is_win(maze)) {
                    found = 1;
                    // reconstruct the path
                    int index = rear - 1;
                    int temp_path[100];
                    int temp_length = 0;

                    while (index != -1) {
                        if (queue[index].direction != -1) {
                            temp_path[temp_length++] = queue[index].direction;
                        }
                        index = queue[index].parent_index;
                    }

                    // reverse the path to get the correct order
                    for (int j = 0; j < temp_length; j++) {
                        path[j] = temp_path[temp_length - j - 1];
                    }

                    // restore the player's position
                    maze.px = old_px;
                    maze.py = old_py;
                    return;
                }

                // restore the player's position
                maze.px = old_px;
                maze.py = old_py;
            }
        }
    }

    if (!found) {
        printf("no path found to the goal.\n");
    }
}