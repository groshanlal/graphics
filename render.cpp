#include <Windows.h>
#include "GL\glut.h"

#include "render.hpp"

using namespace std;


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


void drawHierarchy(HIERARCY* skeleton, int frame)
{
    glColor3f(1.0,0.0,0.0);

    glEnable(GL_POINT_SMOOTH);
    glLineWidth(3);
    glPointSize(10);

    glPushMatrix();
    drawJoint(skeleton->rootJoint->children[0],skeleton, frame);
    glPopMatrix();

}


void drawJoint(JOINT* joint, HIERARCY* skeleton, int frame)
{
        //cout<<joint->name<<endl;

        glPushMatrix();
        for(int i=0;i<joint->numChannels;i++)
        {
            if(joint->channels[i]=="Xposition")
            {
                glTranslatef(skeleton->animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],0,0);
            }
            else if(joint->channels[i]=="Yposition")
            {
                glTranslatef(0,skeleton->animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],0);
            }
            else if(joint->channels[i]=="Zposition")
            {
                glTranslatef(0,0,skeleton->animation[frame*(skeleton->totnumChannels) + joint->channelPos+i]);
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
                glRotatef(skeleton->animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],1,0,0);
            }
            else if(joint->channels[i]=="Yrotation")
            {
                glRotatef(skeleton->animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],0,1,0);
            }
            else if(joint->channels[i]=="Zrotation")
            {
                glRotatef(skeleton->animation[frame*(skeleton->totnumChannels) + joint->channelPos+i],0,0,1);
            }
        }

        for(int i=0;i<joint->children.size();i++)
        {
            drawJoint(joint->children[i],skeleton, frame);
        }
        glPopMatrix();
}

