#include <Windows.h>
#include "GL\glut.h"
#include "time.h"

#include "graphics.hpp"

using namespace std;

int frame = 0;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
int mouseButton = 0;
int moving, startx, starty;
bool play = true;
int realign=0;


void mydisplay()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-400,400,-100,200,-1000,1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
	glScalef(zoom,zoom,zoom);


    //glRotatef(15,0,1,0);
    //glRotatef(15,1,0,0);

    drawFloor();


    glColor3f(0,1,0);
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
        glVertex3f(10,100,0);
        glVertex3f(10,0,0);
    glEnd();


    for(int i=0;i<realign;i++)
    {
        realignXZ();
    }

    /*
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
        glVertex3f(10,100,0);
        glVertex3f(10,0,0);
    glEnd();
    */

    //cout<<showHierarchy()->type<<": "<<frame<<endl;
    drawHierarchy(showHierarchy(), frame);

    //drawTransFrame(showHierarchy(),10, 200);

    glFlush();

}

void idle()
{
    clock_t current_time = clock();
    clock_t end_time = current_time + CLOCKS_PER_SEC/showHierarchy()->frameRate;
    //clock_t end_time = current_time + CLOCKS_PER_SEC/1;


    while(current_time<end_time)
        current_time=clock();

    if(play)
        frame++;





    /*
    if((frame>134)&&(showHierarchy()->type == "ACTUAL"))
    {
        switch_toTrans();
        frame=0;
        //exit(0);
    }

    if((frame>39)&&(showHierarchy()->type == "TRANSIENT"))
    {
        switch_toAct();
        frame=42;
        realign++;
        //exit(0);
    }
    */



    if(frame>=showHierarchy()->noFrames)
    {
        frame=0;
        //exit(0);
    }


    glutPostRedisplay();

}

void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
	mouseButton = button;
    moving = 1;
    startx = x;
    starty = y;
  }
  if (state == GLUT_UP) {
	mouseButton = button;
    moving = 0;
  }
}

void motion(int x, int y)
{
  if (moving) {
	if(mouseButton==GLUT_LEFT_BUTTON)
	{
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
	}
	else zoom += ((y-starty)*0.001);
    startx = x;
    starty = y;
	glutPostRedisplay();
  }

}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'p':
		case 'P':
			play = !play;
		break;

		case 'Q':
		case 'q':
			exit(0);
		break;

		default:
		break;
	}

	glutPostRedisplay();
}


