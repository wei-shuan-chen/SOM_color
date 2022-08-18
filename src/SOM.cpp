#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>

#include "SOM.h"

Color** lattice;
Color* dataset;

int go = 0;
bool is_som_finished = false;
const int map_width = 200;
const int map_height = 200;
const int max_iter = 20000;
const int num_color_type = 20;
int iter = 0;
bool stop = true;
double learning_rate = 0.1;
double radius = 100;
double n_learning_rate = 0.1;
double neighbor = 100;
Color** createMap(int width, int height);
Color* createInputDataset(int size);
void destroyMap(Color** lattice, int width, int height);
void destroyInputDataset(Color* dataset, int size);
bool isInNeighborhood(glm::ivec2 bmuIdx, glm::ivec2 nodeIdx, double radius);
void updateNode(Color** lattice, Color nowInput, glm::ivec2 bmuIdx, glm::ivec2 nodeIdx, double radius, double learning_rate);
const Color& getInput(Color* dataset, int size);
double compute(int iter, double fun);
double computeSacle(double sigma, double dist);

void SOM_Create() {
    srand( time(NULL) );

    // 1. Create lattice
    lattice = createMap(map_width, map_height);
    // 2. Create input dataset
    dataset = createInputDataset(num_color_type);
}

void SOM_IterateOnce() {
    // 1. Get one input from the dataset
    // 2. Find BMU
    // 3. Update BMU and the neighbors
    n_learning_rate = compute(iter, learning_rate);
    neighbor = compute(iter, radius);
    //cout << neighbor <<endl;
    const Color& nowInput = getInput(dataset, num_color_type);
    double minDist = -1.0;
    glm::ivec2 bmu;
    //compute winner point
    for(int i = 0; i < map_width; i++){
        for(int j = 0; j < map_height; j++){
            double tmp = 0.0;
            tmp = pow(lattice[i][j].r - nowInput.r,2) + pow(lattice[i][j].g - nowInput.g,2) + pow(lattice[i][j].b - nowInput.b,2);
            
            if(minDist < 0.0){
                minDist = tmp;
                bmu.x = 0;
                bmu.y = 0;
            }else{
                if(minDist > tmp){
                    minDist = tmp;
                    bmu.x = i;
                    bmu.y = j;
                }
            }
        }
    }
    // renew winner point and neighnorhood

    for(int i = 0; i < map_width; i++){
        for(int j = 0; j < map_height; j++){
            glm::ivec2 node = glm::ivec2(i,j);
            glm::ivec2 diff = node - bmu;
            double squaredDist = static_cast<double>(diff.x * diff.x + diff.y * diff.y);

            if( isInNeighborhood(bmu, node, neighbor)){
                double n_radius = computeSacle(neighbor,squaredDist);
                updateNode(lattice, nowInput, bmu, node, n_radius, n_learning_rate);
            }
        }
    }
    iter++;
    is_som_finished = (iter > max_iter);
}

void SOM_Destroy() {
    // 1. Destroy lattice
    destroyMap(lattice,map_width,map_height);
    // 2. Destroy input dataset
    destroyInputDataset(dataset,num_color_type);
}

Color** createMap(int width, int height) {
    Color** lattice = (Color**)malloc(sizeof(Color*) * width);
    for(int i = 0; i < width; i++){
        lattice[i] = (Color*)malloc(sizeof(Color) * height);
    } 
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            float r = (float)(rand()/(RAND_MAX + 1.0));
            float b = (float)(rand()/(RAND_MAX + 1.0));
            float g = (float)(rand()/(RAND_MAX + 1.0));
            lattice[i][j] = {r, g, b};
        }
    }
    return lattice;
}

Color* createInputDataset(int size) {
    Color* dataset = static_cast<Color*>(malloc(size * sizeof(Color)));
    
    for (int i = 0; i < size; i++) {
        float r = static_cast<float>(rand() / (RAND_MAX + 1.0));
        float g = static_cast<float>(rand() / (RAND_MAX + 1.0));
        float b = static_cast<float>(rand() / (RAND_MAX + 1.0));
        dataset[i] = {r, g, b};
        
    }
    return dataset;
}

void destroyMap(Color** lattice,int width, int height){
    for(int i = 0; i < width; i++){
        free(lattice[i]);
    }
}
void destroyInputDataset(Color* dataset, int size){
    free(dataset);
}

bool isInNeighborhood(glm::ivec2 bmuIdx, glm::ivec2 nodeIdx, double radius) {
    glm::ivec2 diff = nodeIdx - bmuIdx;
    if ((diff.x * diff.x + diff.y * diff.y) <= (radius * radius)) {
        return true;
    }
    return false;
}

void updateNode(Color** lattice, Color nowInput, glm::ivec2 bmuIdx, glm::ivec2 nodeIdx, double radius, double learning_rate) {
    const Color& bmu = lattice[bmuIdx.x][bmuIdx.y];
    Color& node = lattice[nodeIdx.x][nodeIdx.y];
    node.r = node.r + radius * learning_rate * (nowInput.r - node.r);
    node.g = node.g + radius * learning_rate * (nowInput.g - node.g);
    node.b = node.b + radius * learning_rate * (nowInput.b - node.b);

    // node.r = node.r + radius * learning_rate * (bmu.r - node.r);
    // node.g = node.g + radius * learning_rate * (bmu.g - node.g);
    // node.b = node.b + radius * learning_rate * (bmu.b - node.b);
}

const Color& getInput(Color* dataset, int size) {
    int num = rand() % size;
    return dataset[num];
}
double compute(int iter, double fun){
    double lamda = ((double)(max_iter))/ log(fun);
    double sigma = fun*exp(-1* ((double)iter)/ lamda);
    return sigma;
}
double computeSacle(double sigma, double dist){
    double theta = exp((-1 *dist)/(2*pow(sigma,2)));

    return theta;
}