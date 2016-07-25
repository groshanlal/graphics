#include <iostream>
#include <Windows.h>
#include "GL\glut.h"


using namespace std;

class point
{
   private:
        float modelview[16];
        float world_coordinates[4];
        float drawing_coordinates[4];

   public:
       float x,y,z;
        point(float dx, float dy, float dz)
        {
            drawing_coordinates[0]=dx;
            drawing_coordinates[1]=dy;
            drawing_coordinates[2]=dz;
            drawing_coordinates[3]=1;


            glPushMatrix();
            glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
            glPopMatrix();


            /*
            cout<<"Model-View Matrix:\n";
            for(int i=0;i<4;i++)
            {
                for(int j=0;j<4;j++)
                    cout<<modelview[4*j+i]<<", ";
                cout<<endl;
            }
            */


            for(int i=0;i<4;i++)
            {
                world_coordinates[i]=0;
                for(int j=0;j<4;j++)
                    world_coordinates[i] = world_coordinates[i] + modelview[4*j+i]*drawing_coordinates[j];
            }

            x = world_coordinates[0]/world_coordinates[3];
            y = world_coordinates[1]/world_coordinates[3];
            z = world_coordinates[2]/world_coordinates[3];

            //cout<<"Actual Coordinates: "<<x<<", "<<y<<", "<<z<<endl;
        }

};

void mydisplay()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glRotatef(60,0,0,1);
    glTranslatef(1,0,0);
    //glScalef(4,5,6);

    point* a = new point(0,0,0);
    cout<<a->x<<", "<<a->y<<", "<<a->z<<endl;


    glFlush();

}

int main()
{
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    //glutInitWindowSize (1600, 600);
	//glutInitWindowPosition (50, 50);
	glutCreateWindow("Get Coordinates");


    glutDisplayFunc(mydisplay);
    glutMainLoop();


}

