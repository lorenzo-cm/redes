#include "neighbors.h"

struct Neighbor neighbors[MAX_NEIGHBORS];

float calcularDistancia(int x, int y, int x2, int y2) {
    int deltaX = x2 - x;
    int deltaY = y2 - y;

    return sqrt(deltaX * deltaX + deltaY * deltaY);
}

void init_neighbors(int my_x, int my_y){
    for(int i = 1; i<MAX_NEIGHBORS; i++){
        neighbors[i].x = -1;
        neighbors[i].y = -1;
        neighbors[i].distance = -1;
    }

    struct Neighbor new_neigh;
    new_neigh.x = my_x;
    new_neigh.y = my_y;
    new_neigh.distance = 0;
    neighbors[0] = new_neigh;
}

int find_empty_index(){
    int index = -1;
    for(int i = 1; i<MAX_NEIGHBORS; i++){
        if(neighbors[i].x == -1){
            return i;
        }
    }
    return index;
}

void add_neighbor(int x, int y){
    if(alredy_exists(x, y)){
        return;
    }

    int index = find_empty_index();
    struct Neighbor new_neigh;
    new_neigh.x = x;
    new_neigh.y = y;
    new_neigh.distance = calcularDistancia(x, y, neighbors[0].x, neighbors[0].y);
    neighbors[index] = new_neigh;
}

void remove_neighbor(int x, int y){
    for(int i = 1; i<MAX_NEIGHBORS; i++){
        if(neighbors[i].x == x && neighbors[i].y == y){
            neighbors[i].x = -1;
            neighbors[i].y = -1;
            neighbors[i].distance = -1;
        }
    }
}

int check_neighbor_top3(int x, int y) {
    float top3[3] = {1000, 1000, 1000};
    int top3_indices[3] = {-1, -1, -1};

    for (int i = 1; i < MAX_NEIGHBORS; i++) {
        if (neighbors[i].distance < top3[2] &&
            neighbors[i].distance != -1) {
            top3[2] = neighbors[i].distance;
            top3_indices[2] = i;

            for (int j = 2; j > 0; j--) {
                if (top3[j] < top3[j - 1]) {
                    int temp_dist = top3[j];
                    top3[j] = top3[j - 1];
                    top3[j - 1] = temp_dist;

                    int temp_index = top3_indices[j];
                    top3_indices[j] = top3_indices[j - 1];
                    top3_indices[j - 1] = temp_index;
                }
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        int index = top3_indices[i];
        if (index != -1 && neighbors[index].x == x && neighbors[index].y == y) {
            return 1;
        }
    }

    return 0;
}

int alredy_exists(int x, int y){
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if(neighbors[i].x == x && neighbors[i].y == y){
            return 1;
        }       
    }

    return 0;
}

void print_neighbors(){
    printf("{");
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if(neighbors[i].distance != -1){
            printf(" [i=%d, x=%d, y=%d, d=%f]", 
            i, 
            neighbors[i].x,
            neighbors[i].y,
            neighbors[i].distance);
        }
        
    }
    printf("}\n");
}