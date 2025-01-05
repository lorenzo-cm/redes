#include "server_core.h"

static struct client_data *clients_temp[MAX_CLIENTS];
static struct client_data *clients_hum[MAX_CLIENTS];
static struct client_data *clients_air[MAX_CLIENTS];

static int num_temp = 0;
static int num_hum  = 0;
static int num_air  = 0;

static pthread_mutex_t mutex_temp = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_hum  = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_air  = PTHREAD_MUTEX_INITIALIZER;

static int same_sensor(const struct client_data *a, const struct client_data *b) {
    if(a->csock == b->csock){
        return 1;
    }
    return 0;
}

static void add_client(struct client_data *new_sensor) {
    const char *tipo = new_sensor->sensor_info.type;

    if (strcmp(tipo, "temperature") == 0) {
        pthread_mutex_lock(&mutex_temp);

        for (int i = 0; i < num_temp; i++) {
            if (same_sensor(new_sensor, clients_temp[i])) {
                pthread_mutex_unlock(&mutex_temp);
                return;
            }
        }

        if (num_temp < MAX_CLIENTS) {
            clients_temp[num_temp] = new_sensor;
            num_temp++;
        } 
        
        pthread_mutex_unlock(&mutex_temp);
    }


    else if (strcmp(tipo, "humidity") == 0) {
        pthread_mutex_lock(&mutex_hum);

        for (int i = 0; i < num_hum; i++) {
            if (same_sensor(new_sensor, clients_hum[i])) {
                pthread_mutex_unlock(&mutex_hum);
                return;
            }
        }

        if (num_hum < MAX_CLIENTS) {
            clients_hum[num_hum] = new_sensor;
            num_hum++;
        }

        pthread_mutex_unlock(&mutex_hum);
    }


    else if (strcmp(tipo, "air_quality") == 0) {
        pthread_mutex_lock(&mutex_air);

        for (int i = 0; i < num_air; i++) {
            if (same_sensor(new_sensor, clients_air[i])) {
                pthread_mutex_unlock(&mutex_air);
                return;
            }
        }

        if (num_air < MAX_CLIENTS) {
            clients_air[num_air] = new_sensor;
            num_air++;
        } 

        pthread_mutex_unlock(&mutex_air);
    }
}

static void remove_client(struct client_data *cdata) {
    const char *tipo = cdata->sensor_info.type;

    if (strcmp(tipo, "temperature") == 0) {
        pthread_mutex_lock(&mutex_temp);
        for (int i = 0; i < num_temp; i++) {
            if (clients_temp[i]->csock == cdata->csock) {
                clients_temp[i] = clients_temp[num_temp - 1];
                clients_temp[num_temp - 1] = NULL;
                num_temp--;
                break;
            }
        }
        pthread_mutex_unlock(&mutex_temp);
    }

    else if (strcmp(tipo, "humidity") == 0) {
        pthread_mutex_lock(&mutex_hum);
        for (int i = 0; i < num_hum; i++) {
            if (clients_hum[i]->csock == cdata->csock) {
                clients_hum[i] = clients_hum[num_hum - 1];
                clients_hum[num_hum - 1] = NULL;
                num_hum--;
                break;
            }
        }
        pthread_mutex_unlock(&mutex_hum);
    }

    else if (strcmp(tipo, "air_quality") == 0) {
        pthread_mutex_lock(&mutex_air);
        for (int i = 0; i < num_air; i++) {
            if (clients_air[i]->csock == cdata->csock) {
                clients_air[i] = clients_air[num_air - 1];
                clients_air[num_air - 1] = NULL;
                num_air--;
                break;
            }
        }
        pthread_mutex_unlock(&mutex_air);
    }
}

static void broadcast_message(const struct sensor_message *msg) {
    if (strcmp(msg->type, "temperature") == 0) {
        pthread_mutex_lock(&mutex_temp);
        for (int i = 0; i < num_temp; i++) {
            send(clients_temp[i]->csock, msg, sizeof(*msg), 0);
        }
        pthread_mutex_unlock(&mutex_temp);
    }
    else if (strcmp(msg->type, "humidity") == 0) {
        pthread_mutex_lock(&mutex_hum);
        for (int i = 0; i < num_hum; i++) {
            send(clients_hum[i]->csock, msg, sizeof(*msg), 0);
        }
        pthread_mutex_unlock(&mutex_hum);
    }
    else if (strcmp(msg->type, "air_quality") == 0) {
        pthread_mutex_lock(&mutex_air);
        for (int i = 0; i < num_air; i++) {
            send(clients_air[i]->csock, msg, sizeof(*msg), 0);
        }
        pthread_mutex_unlock(&mutex_air);
    }
}

void* client_thread(void *arg) {
    struct client_data *cdata = (struct client_data *)arg;
    int csock = cdata->csock;

    struct sensor_message sensor;

    while(1){
        ssize_t count = recv(csock, &sensor, sizeof(sensor), 0);

        if (count <= 0) {
            remove_client(cdata);

            struct sensor_message leaving_msg;
            memset(&leaving_msg, 0, sizeof(leaving_msg));

            strcpy(leaving_msg.type, cdata->sensor_info.type);
            leaving_msg.coords[0] = cdata->sensor_info.coords[0];
            leaving_msg.coords[1] = cdata->sensor_info.coords[1];

            leaving_msg.measurement = -1.0f;

            broadcast_message(&leaving_msg);
            
            printf("log\n");
            printf("%s sensor in (%d,%d)\n",
                cdata->sensor_info.type,
                cdata->sensor_info.coords[0], 
                cdata->sensor_info.coords[1]);
            printf("measurement: %.4f\n\n", leaving_msg.measurement);

            break;
        }

        memcpy(&(cdata->sensor_info), &sensor, sizeof(sensor));

        // tenta add o sensor
        add_client(cdata);

        broadcast_message(&sensor);

        printf("log\n");
        printf("%s sensor in (%d,%d)\n",
                cdata->sensor_info.type,
                cdata->sensor_info.coords[0], 
                cdata->sensor_info.coords[1]);
        printf("measurement: %.4f\n\n", cdata->sensor_info.measurement);
    }
    
    close(csock);

    pthread_exit(EXIT_SUCCESS);
}
