#ifndef render_hpp
#define render_hpp

#include "bvh.hpp"

using namespace std;

void drawFloor();

void drawHierarchy(HIERARCY* skeleton, int frame);

void drawJoint(JOINT* joint, HIERARCY* skeleton, int frame);

#endif
