#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H

struct sensor_message {
    char type[12];
    int coords[2];
    float measurement;
};

#endif

// type
// Essa propriedade armazena o tipo do sensor. Neste trabalho serão 3 diferentes:
// ● temperature, que vai medir a temperatura do ambiente;
// ● humidity, para sensores de umidade;
// ● air_quality, para medir a qualidade do ar;

// coords
// Essa propriedade representa as coordenadas do sensor no espaço. Neste trabalho
// será considerado um grid 10x10, onde o primeiro valor do vetor representa em que
// “linha” o sensor está, e o segundo a “coluna”.

// measurement
// Já essa propriedade representa a medição de cada sensor, que ele mantém e envia
// em cada intervalo de tempo, além de atualizar conforme os vizinhos.