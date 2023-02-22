#ifndef SOM_H
#define SOM_H

#include "color.h"

void SOM_Create();
void SOM_IterateOnce();
void SOM_Destroy();

extern Color** lattice;
extern Color* dataset;
extern const int map_width;
extern const int map_height;
extern const int num_color_type;
extern int go;
extern bool stop;
extern int max_iter;
extern int iter;
extern double n_learning_rate;
extern double neighbor;
#endif