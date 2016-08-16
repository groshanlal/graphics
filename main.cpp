//#include <Windows.h>
#include "GL/glut.h"
#include<iostream>
#include<fstream>
#include <vector>

#include "graphics.hpp"
#include "MotionGraph.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    cout<<"P for Play/Pause"<<endl;
    cout<<"Q for Quit"<<endl;
    cout<<"Drag Mouse Left button to rotate"<<endl;
    cout<<"Drag Mouse Right button to zoom"<<endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize (1600, 600);
	  glutInitWindowPosition (50, 50);
    glutCreateWindow("MOCAP Skeletons from CMU Database");

    ifstream file;
    //file.open(argv[1]);
    file.open("mocap 09_12.bvh");
    loadBvh(&file);
    int noFrames=showHierarchy()->noFrames;

    initialize_mog();


    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutDisplayFunc(mydisplay);
    glutIdleFunc(idle);
    glutMainLoop();



}
