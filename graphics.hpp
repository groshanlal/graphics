#ifndef graphics_hpp
#define graphics_hpp

#include "render.hpp"

using namespace std;

void mydisplay();

void idle();

void mouse(int button, int state, int x, int y);

void motion(int x, int y);

void keyboard (unsigned char key, int x, int y);

#endif
