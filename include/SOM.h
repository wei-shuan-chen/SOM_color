#ifndef SOM_H
#define SOM_H

#include "color.h"

void SOM_Create();
void SOM_IterateOnce();
void SOM_Destroy();

extern Color** lattice;
extern Color* dataset;
extern bool is_som_finished;
extern const int map_width;
extern const int map_height;
extern int go;
#endif