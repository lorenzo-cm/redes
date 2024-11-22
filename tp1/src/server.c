#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "common_maze.h"
#include "server_maze.h"

#define BUFSZ 1024

void usage(int argc, char **argv){
    printf("usage: %s <v4|v6> <server port> -i <input_file>\n", argv[0]);
    printf("example: %s v4 51511 -i data/in.txt\n", argv[0]);
}

int main(int argc, char **argv){

    if (argc < 4){
        usage(argc, argv);
        return EXIT_FAILURE;
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage)){
        usage(argc, argv);
        return EXIT_FAILURE;
    }

    int s;
    s = socket(storage.ss_family , SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }

    int enable = 1;
    if(0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))){
        logexit("setsocketop");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage))){
        logexit("bind");
    }

    if (0 != listen(s, 10)){
        logexit("listen");
    }

    while(1){
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        // communicate through csock
        int csock = accept(s, caddr, &caddrlen);

        if (csock == -1){
            logexit("accept");
        }

        printf("client connected\n");

        struct action send_action;
        struct action recv_action;
        size_t size_action = sizeof(send_action);
        while(1){
            reset_action(&(recv_action));
            size_t count = recv(csock, &(recv_action), size_action, 0);
            
            if(count == 0){
                // client disconnected
                break;
            }

            reset_action(&(send_action));
            server_action_handler(argc, argv, &(recv_action), &(send_action));
            count = send(csock, &(send_action), size_action, 0);

            if(count != size_action){
                logexit("send");
            }

        }

        printf("client disconnected\n");
        close(csock);
    }

    exit(EXIT_SUCCESS);
}