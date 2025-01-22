#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "common_protocol.h"
#include "client_core.h"

#define BUFSZ 1024

void usage(){
    printf("Usage: ./client <server IP> <server port> -type <temperature|humidity|air_quality> -coords <x> <y>");
}

int main(int argc, char **argv){

    if (8 != argc){
        puts("Error: Invalid number of arguments");
        usage();
        exit(EXIT_FAILURE);
    }

    if (0 != strcmp(argv[3], "-type")){
        puts("Error: Expected '-type' argument");
        usage();
        exit(EXIT_FAILURE);
    }

    char type[12];
    strcpy(type, argv[4]);
    if (0 != strcmp(type, "temperature") &&
        0 != strcmp(type, "humidity")    &&
        0 != strcmp(type, "air_quality")){
        puts("Error: Invalid sensor type");
        usage();
        exit(EXIT_FAILURE);
    }
    
    if (0 != strcmp(argv[5], "-coords")){
        puts("Error: Expected '-coords' argument");
        usage();
        exit(EXIT_FAILURE);
    }

    int x = atoi(argv[6]);
    int y = atoi(argv[7]);
    if (x < 0 || x > 9 ||
        y < 0 || y > 9) {
        puts("Error: Coordinates must be in the range 0-9");
        usage();
        exit(EXIT_FAILURE);
    }


    struct sockaddr_storage storage;
    if (0 != addrparse(argv[1], argv[2], &storage)){
        usage();
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

    struct sensor_message csensor;
    size_t size_message = sizeof(csensor);

    float medicao_inicial = gerarMedicao(type);

    csensor.coords[0] = x;
    csensor.coords[1] = y;
    csensor.measurement = medicao_inicial;
    strcpy(csensor.type, type);

    struct sensor_message send_sensor;
    
    struct listener_thread_data listen_data;
    listen_data.sock = s;
    listen_data.sensor = csensor;
    listen_data.new_measurement = medicao_inicial;

    pthread_t tid;
    pthread_create(&tid, NULL, listener_thread, &listen_data);

    // send
    while(1){
        dormirIntervalo(type);

        memcpy(&send_sensor, &csensor, sizeof(send_sensor));

        send_sensor.measurement = listen_data.new_measurement;

        size_t count = send(s, &(send_sensor), size_message, 0);

        if (count != size_message) {
            logexit("send");
        }
    }

    printf("connection closed\n");
    close(s);

    pthread_cancel(tid);
    pthread_join(tid, NULL);

    exit(EXIT_SUCCESS);

}