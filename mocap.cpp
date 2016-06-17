#include <Windows.h>
#include "GL\glut.h"
#include <iostream>
#include <fstream>
#include "time.h"

using namespace std;


GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
int mouseButton = 0;
int moving, startx, starty;
int joint_radius=1;
int FPS = 120;
int frames=0;
int MAX_FRAMES;

bool play = true;

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

void idle()
{
    clock_t current_time = clock();
    clock_t end_time = current_time + CLOCKS_PER_SEC/FPS;


    while(current_time<end_time)
        current_time=clock();

    frames++;

    if(frames>MAX_FRAMES)
    {
        //float last;
        //cin>>last;
        //cout<<"The End: "<<last<<endl;
        exit(0);
    }

    //cout<<frames<<endl;

    glutPostRedisplay();

}

void root(float x, float y, float z )
{
        float xt,yt,zt;
        cin>>xt;
        cin>>yt;
        cin>>zt;

        //cout<<x<<", "<<y<<", "<<z<<endl;

        glTranslatef(xt,yt,zt);


        glTranslatef(x,y,z);
        glBegin(GL_POINTS);
            glVertex3f(0,0,0);
        glEnd();
        glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(-x,-y,-z);
        glEnd();


        float zr,xr,yr;

        cin>>zr;
        cin>>yr;
        cin>>xr;


        glRotatef(zr,0,0,1);
        glRotatef(yr,0,1,0);
        glRotatef(xr,1,0,0);




}



void joint(float x, float y, float z )
{
    glTranslatef(x,y,z);
    glBegin(GL_POINTS);
        glVertex3f(0,0,0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(-x,-y,-z);
    glEnd();


        float zr,xr,yr;

        cin>>zr;
        cin>>xr;
        cin>>yr;


        glRotatef(zr,0,0,1);
        glRotatef(xr,1,0,0);
        glRotatef(yr,0,1,0);



}

void endsite(float x, float y, float z )
{
    glColor3f(0,1,0);
    glTranslatef(x,y,z);
    glBegin(GL_POINTS);
        glVertex3f(0,0,0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(-x,-y,-z);
    glEnd();
    glColor3f(1,0,0);


}



void drawRightEye()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-3.6576, 8.04674, 8.04672);
    endsite(1,0,0);
    glPopMatrix();

}
void drawLeftEye()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(4.14528, 8.04674, 8.04672);
    endsite(1,0,0);
    glPopMatrix();

}
void drawHead()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-0.24384, 7.07133, 1.2192);

    drawLeftEye();
    drawRightEye();
    glPopMatrix();

}


void drawNeck()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(0, 22.1894, -2.19456);

    drawHead();
    glPopMatrix();

}

void drawrThumb2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-3.37035, -0.52449, 3.41376);
    endsite(-1.78271, -1.18214, 1.43049);

    glPopMatrix();

}

void drawrIndex2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-2.54057, -0.884171, 1.56538);
    endsite(-1.62519, -0.234802, 1.16502);

    glPopMatrix();

}

void drawrMid2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-3.10165, -0.590103, 1.0647);
    endsite(-2.48547, -0.328903, 0.83742);

    glPopMatrix();

}


void drawrThumb1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-1.2192, -0.487915, 3.41376);
    drawrThumb2();
    glPopMatrix();

}

void drawrIndex1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-7.75947, 0.938293, 5.60832);
    drawrIndex2();
    glPopMatrix();

}

void drawrMid1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-8.24714, 1.18213, 3.41376);
    drawrMid2();
    glPopMatrix();

}

void drawrRing2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-2.60934, -0.819778, -0.0198488);
    endsite(-2.33842, -0.294052, 0.168128);
    glPopMatrix();

}

void drawrRing1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-8.82822, 0.546677, 1.51678);
    drawrRing2();
    glPopMatrix();

}

void drawrPinky2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-1.82734, -0.647385, -0.700984);
    endsite(-1.69225, -0.51767, -0.607171);
    glPopMatrix();

}

void drawrPinky1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-8.27202, -0.0477905, -0.4584);
    drawrPinky2();
    glPopMatrix();

}


void drawrHand()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-22.5879, 0.773209, 7.07136);
    drawrThumb1();
    drawrIndex1();
    drawrMid1();
    drawrRing1();
    drawrPinky1();
    glPopMatrix();

}

void drawrForeArm()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(-28.1742, -1.7115, 0.48768);

    drawrHand();
    glPopMatrix();

}


void drawrShldr()
{
    glPushMatrix();
    joint(-8.77824, -1.95073, 1.46304);

    drawrForeArm();
    glPopMatrix();

}


void drawrCollar()
{
    glPushMatrix();
    joint(-2.68224, 19.2634, -4.8768);

    drawrShldr();

    glPopMatrix();

}


void drawlThumb2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(3.37035, -0.52449, 3.41376);
    endsite(1.78271, -1.18214, 1.43049);

    glPopMatrix();

}

void drawlIndex2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(2.54057, -0.884171, 1.56538);
    endsite(1.62519, -0.234802, 1.16502);

    glPopMatrix();

}

void drawlMid2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(3.10165, -0.590103, 1.0647);
    endsite(2.48547, -0.328903, 0.83742);

    glPopMatrix();

}


void drawlThumb1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(1.2192, -0.487915, 3.41376);
    drawlThumb2();
    glPopMatrix();

}

void drawlIndex1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(7.75947, 0.938293, 5.60832);
    drawlIndex2();
    glPopMatrix();

}

void drawlMid1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(8.24714, 1.18213, 3.41376);
    drawlMid2();
    glPopMatrix();

}

void drawlRing2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(2.60934, -0.819778, -0.0198488);
    endsite(2.33842, -0.294052, 0.168128);
    glPopMatrix();

}

void drawlRing1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(8.82822, 0.546677, 1.51678);
    drawlRing2();
    glPopMatrix();

}

void drawlPinky2()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(1.82734, -0.647385, -0.700984);
    endsite(1.69225, -0.51767, -0.607171);
    glPopMatrix();

}

void drawlPinky1()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(8.27202, -0.0477905, -0.4584);
    drawlPinky2();
    glPopMatrix();

}




void drawlHand()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(22.5879, 0.773209, 7.07136);
    drawlThumb1();
    drawlIndex1();
    drawlMid1();
    drawlRing1();
    drawlPinky1();
    glPopMatrix();

}



void drawlForeArm()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(28.1742, -1.7115, 0.48768);

    drawlHand();
    glPopMatrix();

}



void drawlShldr()
{
    glPushMatrix();
    joint(8.77824, -1.95073, 1.46304);

    drawlForeArm();

    glPopMatrix();

}

void drawlCollar()
{
    glPushMatrix();
    //glColor3f(0,1,0);

    joint(2.68224, 19.2634, -4.8768);


    drawlShldr();

    glPopMatrix();

}

void drawChest()
{
    glPushMatrix();
    //glColor3f(0,1,0);
    joint(0, 11.7043, -0.48768);

    drawNeck();
    drawrCollar();
    drawlCollar();

    glPopMatrix();

}


void drawAbdomen()
{
    glPushMatrix();
    joint(0, 20.6881, -0.73152);

    drawChest();

    glPopMatrix();

}

void drawrFoot()
{
    glPushMatrix();
    joint(0.73152, -45.1104, -5.12064);
    endsite(-1.1221, -3.69964, 12.103);

    glPopMatrix();

}

void drawlFoot()
{
    glPushMatrix();
    joint(-0.73152, -45.1104, -5.12064);
    endsite(1.1221, -3.69964, 12.103);

    glPopMatrix();

}



void drawrShin()
{
    glPushMatrix();
    joint(0, -36.8199, 0.73152);

    drawrFoot();

    glPopMatrix();

}

void drawlShin()
{
    glPushMatrix();
    joint(0, -36.8199, 0.73152);

    drawlFoot();

    glPopMatrix();

}


void drawrThigh()
{
    glPushMatrix();
    joint(0, -1.70687, -2.19456);

    drawrShin();
    glPopMatrix();

}

void drawlThigh()
{
    glPushMatrix();
    joint(0, -1.70687, -2.19456);

    drawlShin();
    glPopMatrix();

}



void drawrBut()
{
    glPushMatrix();
    joint(-8.77824, 4.35084, 1.2192);

    drawrThigh();
    glPopMatrix();

}

void drawlBut()
{
    glPushMatrix();
    joint(8.77824, 4.35084, 1.2192);

    drawlThigh();
    glPopMatrix();

}

void drawHip()
{
    glPushMatrix();
    root(0,0,0);

    drawAbdomen();
    drawrBut();
    drawlBut();


    glPopMatrix();

}


void drawSkeleton()
{

    glColor3f(1.0,0.0,0.0);

    glEnable(GL_POINT_SMOOTH);
    glLineWidth(1);
    glPointSize(5);

    glPushMatrix();

    drawHip();
    glPopMatrix();


}

void drawFloor()
{
    glPushMatrix();
    glColor3f(1,1,0);
    glBegin(GL_POLYGON);
        glVertex3f(-300,0,-300);
        glVertex3f( 300,0,-300);
        glVertex3f( 300,0, 300);
        glVertex3f(-300,0, 300);
    glEnd();

    glColor3f(0,0,0);
    for(int i=-300;i<300;i=i+30)
    {
        for(int j=-300;j<300;j=j+30)
        {
            glBegin(GL_LINE_LOOP);
                glVertex3f(i,0,j);
                glVertex3f(i+30,0,j);
                glVertex3f(i+30,0,j+30);
                glVertex3f(i,0,j+30);
            glEnd();


        }
    }
    glPopMatrix();
}


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


    glRotatef(15,0,1,0);
    glRotatef(15,1,0,0);

    drawFloor();

    drawSkeleton();

    glFlush();

}

int main()
{
    cout<<"Controls:\n";
    cout<<"Left Mouse Button: Rotate\n";
    cout<<"Right Mouse Button: Zoom\n";

    cin>>MAX_FRAMES;

    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize (1600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow("MOCAP Skeletons from CMU Database");

    glutMouseFunc(mouse);
	glutMotionFunc(motion);

    glutDisplayFunc(mydisplay);

    glutIdleFunc(idle);

    glutMainLoop();
}
