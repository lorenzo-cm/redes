#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <pthread.h>

#include "common_protocol.h"

#define MAX_CLIENTS  20

struct client_data {
    int csock;
    struct sockaddr_storage storage; 
    struct sensor_message sensor_info;
};

void* client_thread(void *arg);

#endif