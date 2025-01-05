#include "client_core.h"

float gerarMedicao(const char *type) {
    srand(time(NULL));
    if (strcmp(type, "temperature") == 0) {
        return 20.0 + (rand() / (float)RAND_MAX) * (40.0 - 20.0);
    } else if (strcmp(type, "humidity") == 0) {
        return 10.0 + (rand() / (float)RAND_MAX) * (90.0 - 10.0);
    } else if (strcmp(type, "air_quality") == 0) {
        return 15.0 + (rand() / (float)RAND_MAX) * (30.0 - 15.0);
    }
    return 0.0;
}

int obterIntervaloTempo(const char *type) {
    if (strcmp(type, "temperature") == 0) {
        return 5;
    } else if (strcmp(type, "humidity") == 0) {
        return 7;
    } else if (strcmp(type, "air_quality") == 0) {
        return 10;
    }
    return -1;
}

int obterMinMedicao(const char *type) {
    if (strcmp(type, "temperature") == 0) {
        return 20;
    } else if (strcmp(type, "humidity") == 0) {
        return 10;
    } else if (strcmp(type, "air_quality") == 0) {
        return 15;
    }
    return -1;
}

int obterMaxMedicao(const char *type) {
    if (strcmp(type, "temperature") == 0) {
        return 40;
    } else if (strcmp(type, "humidity") == 0) {
        return 90;
    } else if (strcmp(type, "air_quality") == 0) {
        return 30;
    }
    return -1;
}

void dormirIntervalo(const char *type){
    sleep(obterIntervaloTempo(type));
}

float clipAtualizacaoMedicao(float atualizacao, const char *type){
    int min = obterMinMedicao(type);
    int max = obterMaxMedicao(type);
    
    if(atualizacao < min){
        return min;
    }

    if(atualizacao > max){
        return max;
    }

    return atualizacao;
}

float atualizaMedicao(float medicao_atual, float medicao_remota, float d){
    return medicao_atual + 0.1 * (medicao_remota - medicao_atual) / (d+1);
}

void* listener_thread(void* arg){
    struct listener_thread_data *data = (struct listener_thread_data*) arg;
    int sock = data->sock;
    int x = data->sensor.coords[0];
    int y = data->sensor.coords[1];
    float my_sensor_measurement = data->sensor.measurement;

    struct sensor_message msg;
    char status[40];

    init_neighbors(x, y);

    while(1){
        size_t count = recv(sock, &(msg), sizeof(msg), 0);

        if(count == 0){
            // server disconnected
            break;
        }

        if(msg.measurement == -1){
            remove_neighbor(msg.coords[0], msg.coords[1]);
            strcpy(status, "removed");
        }

        else if(msg.coords[0] == x &&
                msg.coords[1] == y){
            strcpy(status, "same location");
        }

        else {
            add_neighbor(msg.coords[0], msg.coords[1]);
            int in_top3 = check_neighbor_top3(msg.coords[0], msg.coords[1]);
            

            if(in_top3){
                float d = calcularDistancia(x, y, msg.coords[0], msg.coords[1]);
                float nova_medicao = atualizaMedicao(my_sensor_measurement, msg.measurement, d);
                float clipped = clipAtualizacaoMedicao(nova_medicao, msg.type);
                float delta_atualizacao = nova_medicao - my_sensor_measurement;

                snprintf(status, sizeof(status), "correction of value %.4f", (float)delta_atualizacao);

                data->new_measurement = clipped;
                my_sensor_measurement = clipped;
            }

            else{
                strcpy(status, "not neighbor");
            }
        }
        
        printf("log:\n");
        printf("%s sensor in (%d,%d)\n", msg.type, msg.coords[0], msg.coords[1]);
        printf("measurement: %.4f\n", msg.measurement);
        printf("action: %s\n\n", status);
    }

    close(sock);
    pthread_exit(NULL);
}
