#include "client_maze.h"

int client_started = 0;
int client_win = 0;

void client_print_moves(struct action action, int is_hint){
    int first = 1;
    int has_move = 0;
    
    for(int i = 0; i<100; i++){
        if(action.moves[i] != 0){
            if(first){
                if(is_hint){
                    printf("Hint:");
                }
                else{
                    printf("Possible moves:");
                }
                first = 0;
                has_move = 1;
                char* move_str = NUM2MOVE[action.moves[i]];
                printf(" %s", move_str);
                continue;
            }
            char* move_str = NUM2MOVE[action.moves[i]];
            printf(", %s", move_str);
        }
    }
    if(has_move){
        printf(".\n");
    }
}


void client_print_error(const char* error){
    printf("error: %s\n", error);
}


void client_print_map(struct action action){
    // printf("\n\nRECEIVED MAP\n");
    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 10; j++) {
    //         printf("%d ", action.board[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");

    Maze maze;
    int size = 0;
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            maze.board[i][j] = action.board[i][j];
            if(i == 0 && action.board[i][j] != -1){
                size++;
            }
        }
    }

    maze.size = size;

    char buf[300];
    maze2string(maze, buf);
    printf("%s\n", buf);
}

int client_check_move_legal(struct action action, char* buf){
    int move_index = direction2num(buf);
    for(int i = 0; i<4; i++){
        if(action.moves[i] == move_index){
            return move_index;
        }
    }
    return 0;
}

int client_action_handler(struct action* recv_action, struct action* send_action, char* buf){

    // non existing type
    if(send_action->type < 0 || send_action->type > 6){
        client_print_error("command not found");
        return 0;
    }

    // se startar
    if(send_action->type == 0){
        client_started = 1;
        return 1;
    }

    // se venceu n pode digitar outro comando que nao reset e start
    if(client_win && send_action->type != 0 && send_action->type != 6){
        return 0;
    }

    // reset
    if(send_action->type == 6){
        client_win = 0;
        client_started = 1;
        return 1;
    }

    if(!client_started){
        client_print_error("start the game first");
        return 0;
    }

    // move
    if(send_action->type == 1){
        int move = client_check_move_legal(*recv_action, buf);
        if(move){
            send_action->moves[0] = move;
        }
        else{
            client_print_error("you cannot go this way");
            return 0;
        }
    }

    return 1;
}


void client_pos_action_handler(struct action recv_action, int prev_type){
    // received win
    if (recv_action.type == 5){
        printf("You escaped!\n");
        client_print_map(recv_action);
        client_win = 1;
        return;
    }

    // types are start, move, reset
    if(prev_type == 0 || prev_type == 1 || prev_type == 6){
        client_print_moves(recv_action, 0);
    }

    // map type
    if(prev_type == 2){
        client_print_map(recv_action);
    }

    // hint type
    if (prev_type == 3){
        client_print_moves(recv_action, 1);
    }
}