#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "common_maze.h"
#include "client_maze.h"

#define BUFSZ 1024

void usage(int argc, char **argv){
    printf("usage: %s <server IP> <server port>", argv[0]);
    printf("example: %s 122.0.0.1 51511", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv){

    if (argc < 3){
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != addrparse(argv[1], argv[2], &storage)){
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family , SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != connect(s, addr, sizeof(storage))) {
        logexit("connect");
    }


    struct action send_action;
    struct action recv_action;
    size_t size_action = sizeof(send_action);
    char buf[BUFSZ];
    int aux_type = -1;
    
    while(1){
        reset_action(&(send_action));
        aux_type = -1;

        printf("> ");
        fgets(buf, size_action-1, stdin);

        if(strcmp(buf, "exit\n") == 0){
            break;
        }

        send_action.type = aux_type = action2type(buf);
        
        if(0 == client_action_handler(&(recv_action), &(send_action), buf)){
            printf("\n");
            continue;
        }

        size_t count = send(s, &(send_action), size_action, 0);

        if (count != size_action) {
            logexit("send");
        }

        reset_action(&(recv_action));
        count = recv(s, &(recv_action), size_action, 0);

        if (count == 0){
            // connection terminated
            break;
        }

        client_pos_action_handler(recv_action, aux_type);
        printf("\n");
    }

    printf("connection closed\n");
    close(s);

    exit(EXIT_SUCCESS);

}