#include "server_maze.h"

Maze server_maze;
int started = 0;

void server_start(int argc, char** argv){
    read_maze(&(server_maze), argv[4]);
    printf("starting new game\n");
}

void server_get_moves(struct action* action){
    memset(action->moves, 0, sizeof(action->moves));
    get_legal_moves(&(server_maze), action->moves);
}

void server_move(int move_index){
    make_move(&(server_maze), move_index);
}

void server_hint(struct action* action){
    memset(action->moves, 0, 400);
    bfs(server_maze, action->moves);
}

int server_is_win(){
    return is_win(server_maze);
}

void server_reset(int argc, char** argv){
    printf("starting new game\n");
    memset(&server_maze, 0, sizeof(server_maze));
    read_maze(&(server_maze), argv[4]);
}

void server_action_handler(int argc, char** argv, struct action* recv_action, struct action* send_action){

    // start
    if(recv_action->type == 0){
        server_start(argc, argv);

        send_action->type = 4;
        // gen_board_unvisited(server_maze, send_action->board);
        server_get_moves(send_action);
    }

    // move
    else if(recv_action->type == 1){
        int move = recv_action->moves[0];

        server_move(move);

        if(server_is_win()){
            send_action->type = 5;
            reveal_all(&(server_maze));
            server_maze.px = server_maze.py = -1;
            gen_board_unvisited(server_maze, send_action->board);
            return;
        }

        send_action->type = 4;
        gen_board_unvisited(server_maze, send_action->board);
        server_get_moves(send_action);
    }

    // map
    else if(recv_action->type == 2){
        send_action->type = 4;
        gen_board_unvisited(server_maze, send_action->board);
        // server_get_moves(send_action);
    }

    // hint
    else if(recv_action->type == 3){
        send_action->type = 4;
        // gen_board_unvisited(server_maze, send_action->board);
        server_hint(send_action);
    }

    // n tem 4 nem 5 aqui no servidor para tratar

    // reset
    else if(recv_action->type == 6){
        send_action->type = 4;
        server_reset(argc, argv);
        memset(send_action->moves, 0, 400);
        memset(send_action->board, -1, 400);
        server_get_moves(send_action);
        // gen_board_unvisited(server_maze, send_action->board);
    }

    // n precisa tratar o 7 pq ele ja eh tratado por padrao
}