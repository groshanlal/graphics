#include <Windows.h>
#include "GL\glut.h"
#include "time.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>

using namespace std;

#define MAX_NUM_OF_CHANNELS 6


typedef struct JOINT
{
    string name;
    JOINT* parent;
    float x_off,y_off,z_off;
    int numChannels;
    int channelPos;
    string channels[MAX_NUM_OF_CHANNELS];
    vector <JOINT*> children;
}
JOINT;

void displayJoint(JOINT* joint)
{
    cout<<"Name: "<<joint->name<<endl;
    cout<<"Parent: "<<joint->parent->name<<endl;
    cout<<"OFFSET: "<<joint->x_off<<", "<<joint->y_off<<", "<<joint->z_off<<endl;
    cout<<"CHANNELS: "<<joint->numChannels<<": ";

    for(int i=0;i<joint->numChannels;i++)
        cout<<joint->channels[i]<<", ";

    cout<<endl;
    cout<<"Position in Total Channels: "<<joint->channelPos<<endl;

    cout<<"Children: ";
    for(int i=0;i<joint->children.size();i++)
        cout<<joint->children[i]->name<<" ";

    cout<<endl<<"----------------------------"<<endl;

}


typedef struct
{
    JOINT* rootJoint;
    int totnumChannels;
}
HIERARCY;


HIERARCY* skeleton;

int noFrames, frameRate;
float* animation;
int frame = 0;

GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
int mouseButton = 0;
int moving, startx, starty;
bool play = true;


float* loadMotion(HIERARCY* skeleton ,ifstream* file, int* noFrames, int* frameRate)
{

    string line;
    *file>>line;

    float* animation;
    if(line=="MOTION")
    {
        *file>>line;   //Frames:
        *file>>*noFrames;

        float frameTime;
        *file>>line>>line;   //Frame Time:
        *file>>frameTime;

        *frameRate=1/frameTime;

        animation = new float[(*noFrames)*(skeleton->totnumChannels)];
        for(int i=0;i<*noFrames;i++)
        {
            for(int j=0;j<skeleton->totnumChannels;j++)
            {
                *file>>animation[i*(skeleton->totnumChannels)+j];
            }
        }

    }
    else
    {
        cout<<"MOTION not found after the HIERARACHY tree\n";
    }

    return animation;

}

void loadJoint(JOINT* parent, HIERARCY* skeleton, ifstream* file)
{
    JOINT* joint = new JOINT;

    *file>>joint->name;

    joint->parent=parent;
    parent->children.push_back(joint);

    //joint->children.reserve(5);

    string brace;
    *file>>brace;

    if(brace=="{")
    {
        string line;
        //int i=0;
        while(true)
        {
        //i++;
        *file>>line;

        if(line=="OFFSET")
        {
            *file>>joint->x_off;
            *file>>joint->y_off;
            *file>>joint->z_off;

            //cout<<joint->x_off<<", "<<joint->y_off<<", "<<joint->z_off<<endl;

        }

        else if(line=="CHANNELS")
        {
            *file>>joint->numChannels;
            joint->channelPos = skeleton->totnumChannels;
            skeleton->totnumChannels=skeleton->totnumChannels+joint->numChannels;

            for(int i=0;i<joint->numChannels;i++)
                *file>>joint->channels[i];

            /*
            for(int i=0;i<joint->numChannels;i++)
                cout<<joint->channels[i]<<", ";
            cout<<endl;
            */
        }
        else if(line=="JOINT")
        {
            loadJoint(joint, skeleton, file);
        }
        else if(line=="End")
        {
            *file>>line>>line>>line;//Site, {, OFFSET
            JOINT* ending = new JOINT;
            ending->parent=joint;
            joint->children.push_back(ending);

            ending->name="End Site";

            *file>>ending->x_off;
            *file>>ending->y_off;
            *file>>ending->z_off;

            ending->numChannels = 0;
            *file>>line; //}
        }
        else if(line == "}")
        {
            //cout<<joint->name<<","<<joint->children.size()<<endl;
            //displayJoint(joint);
            return;
        }

        else
        {
            cout<<"Unable to parse: "<<line<<endl;
        }



        }

    }
    else
        cout<<brace<<" found instead of braces\n";

    //cout<<joint->name<<","<<joint->children.size()<<endl;
    //displayJoint(joint);
    return;


}

void loadHierarchy(HIERARCY* skeleton, ifstream* file)
{
    string line;
    *file>>line;

    if(line=="HIERARCHY")
    {
        skeleton->totnumChannels=0;
        *file>>line;
        if(line=="ROOT")
        {
            skeleton->rootJoint = new JOINT;
            skeleton->rootJoint->name = "ROOT";
            loadJoint(skeleton->rootJoint, skeleton, file);
        }
        else
            cout<<"File doesn't contain ROOT immediately after HIERARCHY\n";

    }
    else
        cout<<"File doesn't start with HIERARCHY\n";

}



void drawJoint(JOINT* joint, float* animation)
{
        //cout<<joint->name<<endl;

        glPushMatrix();
        for(int i=0;i<joint->numChannels;i++)
        {
            if(joint->channels[i]=="Xposition")
            {
                glTranslatef(animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],0,0);
            }
            else if(joint->channels[i]=="Yposition")
            {
                glTranslatef(0,animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],0);
            }
            else if(joint->channels[i]=="Zposition")
            {
                glTranslatef(0,0,animation[frame*(skeleton->totnumChannels) + joint->channelPos+i]);
            }
        }

        glTranslatef(joint->x_off,joint->y_off,joint->z_off);

        glBegin(GL_POINTS);
            glVertex3f(0,0,0);
        glEnd();

        glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(-(joint->x_off),-(joint->y_off),-(joint->z_off));
        glEnd();


        for(int i=0;i<joint->numChannels;i++)
        {
            if(joint->channels[i]=="Xrotation")
            {
                glRotatef(animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],1,0,0);
            }
            else if(joint->channels[i]=="Yrotation")
            {
                glRotatef(animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],0,1,0);
            }
            else if(joint->channels[i]=="Zrotation")
            {
                glRotatef(animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],0,0,1);
            }
        }

        for(int i=0;i<joint->children.size();i++)
        {
            drawJoint(joint->children[i],animation);
        }
        glPopMatrix();
}

void drawHierarchy(HIERARCY* skeleton, float* animation)
{
    glColor3f(1.0,0.0,0.0);

    glEnable(GL_POINT_SMOOTH);
    glLineWidth(3);
    glPointSize(10);


    /*
    glBegin(GL_POLYGON);
        glVertex3f(-50,0,0);
        glVertex3f( 50,0,0);
        glVertex3f( 50,100,0);
        glVertex3f(-50,100,0);
    glEnd();

    glBegin(GL_POINTS);
        glVertex3f(-50,0,0);
        glVertex3f( 50,0,0);
        glVertex3f( 50,100,0);
        glVertex3f(-50,100,0);
    glEnd();
    */



    glPushMatrix();
    drawJoint(skeleton->rootJoint->children[0],animation);
    glPopMatrix();


}


void drawFloor()
{
    glLineWidth(1);
    glPointSize(1);

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

    drawHierarchy(skeleton, animation);

    glFlush();

}


void idle()
{
    clock_t current_time = clock();
    clock_t end_time = current_time + CLOCKS_PER_SEC/frameRate;
    //clock_t end_time = current_time + CLOCKS_PER_SEC/1;


    while(current_time<end_time)
        current_time=clock();

    if(play)
        frame++;

    if(frame>noFrames)
    {
        frame=0;
        //exit(0);
    }


    //cout<<frame<<endl;
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


int main(int argc, char* argv[])
{

    char filename[80];
    strcpy(filename, argv[1]);
    strcat (filename," ");
    for(int i=2;i<argc;i++)
    {
        strcat (filename,argv[i]);
        strcat (filename," ");
    }

    ifstream file;
    //file.open("mocap 08_01.bvh");
    file.open(filename);

    skeleton = new HIERARCY;

    loadHierarchy(skeleton, &file);
    animation = loadMotion(skeleton, &file, &noFrames, &frameRate);
    //cout<<noFrames<<endl;
    //cout<<frameRate<<endl;


    /*
    cout<<"Yet to be parsed:\n";

    for(int i=0;i<1;i++)
    {
        string line;
        file>>line;
        cout<<line<<endl;
    }
    */


    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize (1600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow("MOCAP Skeletons from CMU Database");

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);

    glutDisplayFunc(mydisplay);
    glutIdleFunc(idle);
    glutMainLoop();

    /*
    JOINT hips;
    hips.children.reserve(5);
    cout<<hips.children.capacity()<<endl;
    */
}
