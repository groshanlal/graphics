#ifndef motion_graph_hpp
#define motion_graph_hpp

#include<iostream>
#include<string.h>
#include<fstream>
#include<vector>

#include "bvh.hpp"

using namespace std;

void align(float* theta, float* x, float* z, int i, int j, HIERARCY* skeleton);
void errmatrix(HIERARCY* skeleton);
float* interp_frames(int f1, int f2, HIERARCY* skeleton);
#endif
