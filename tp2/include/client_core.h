#ifndef CLIENT_CORE_H
#define CLIENT_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <pthread.h>

#include "common_protocol.h"
#include "neighbors.h"

struct listener_thread_data{
    int sock;
    struct sensor_message sensor;
    float new_measurement;
};

float gerarMedicao(const char *type);

int obterIntervaloTempo(const char *type);

int obterMinMedicao(const char *type);

int obterMaxMedicao(const char *type);

void dormirIntervalo(const char *type);

float clipAtualizacaoMedicao(float atualizacao, const char *type);

float atualizaMedicao(float medicao_atual, float medicao_remota, float d);

void* listener_thread(void* arg);

#endif