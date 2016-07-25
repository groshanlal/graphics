#include<Windows.h>
#include<GL/glut.h>

#include<fstream>
#include<iostream>
#include<string.h>
#include<vector>

#include "bvh.hpp"
#include "motion_graph.hpp"
#include "graphics.hpp"

using namespace std;

HIERARCY* skeleton=new HIERARCY;

float theta0, x0, z0;

float* act_animation;
float* trans_animation;

HIERARCY* showHierarchy()
{
    return skeleton;
}

void switch_toAct()
{
    skeleton->animation=act_animation;
    skeleton->type="ACTUAL";
}

void switch_toTrans()
{
    skeleton->animation=trans_animation;
    skeleton->type="TRANSIENT";
}

void realignXZ()
{
    glTranslatef(x0,0,z0);
    glRotatef(theta0/3.14159*180,0,1,0);
    //cout<<theta0<<": "<<x0<<", "<<z0<<endl;
}
void loadBvh(ifstream* file)
{
    loadHierarchy(skeleton, file);
    loadMotion(skeleton, file);

    skeleton->type="ACTUAL";

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();



    for(int i=0;i<skeleton->noFrames;i++)
    {
        glPushMatrix();
        genPointCloud(skeleton->rootJoint->children[0],skeleton,i);
        glPopMatrix();
    }


    //errmatrix(skeleton);


    align(&theta0, &x0, &z0, 134, 2, skeleton);
    //cout<<theta0<<", "<<x0<<", "<<z0<<endl;

    act_animation=skeleton->animation;

    int transient_length = skeleton->frameRate/3;
    trans_animation = new float[(transient_length)*(skeleton->totnumChannels)];

    float alpha;

    for(int i=0;i<transient_length;i++)
    {
        alpha = 2*((i+1)/transient_length)*((i+1)/transient_length)*((i+1)/transient_length) - 3*((i+1)/transient_length)*((i+1)/transient_length) + 1;

        float x2=act_animation[(2+i)*(skeleton->totnumChannels)+0];
        float y2=act_animation[(2+i)*(skeleton->totnumChannels)+1];
        float z2=act_animation[(2+i)*(skeleton->totnumChannels)+2];

        point a(x2,y2,z2);
        point b=a.trans(theta0, x0, z0);
        x2=b.x;
        y2=b.y;
        z2=b.z;

        float x1=act_animation[(134+i)*(skeleton->totnumChannels)+0];
        float y1=act_animation[(134+i)*(skeleton->totnumChannels)+1];
        float z1=act_animation[(134+i)*(skeleton->totnumChannels)+2];

        trans_animation[(i)*(skeleton->totnumChannels)+0] = alpha*x1 + (1-alpha)*x2;
        trans_animation[(i)*(skeleton->totnumChannels)+1] = alpha*y1 + (1-alpha)*y2;
        trans_animation[(i)*(skeleton->totnumChannels)+2] = alpha*z1 + (1-alpha)*z2;

        for(int j=3;j<skeleton->totnumChannels;j++)
        {
            float theta1 = act_animation[(134+i)*(skeleton->totnumChannels)+ j];
            float theta2 = act_animation[(2+i)*(skeleton->totnumChannels)+ j];
            trans_animation[(i)*(skeleton->totnumChannels)+j] = alpha*theta1 + (1-alpha)*theta2;
        }

        /*
        for(int j=0;j<skeleton->totnumChannels;j++)
        {
            cout<<trans_animation[(i)*(skeleton->totnumChannels)+j]<<" ";
        }
        cout<<endl;
        */



    }



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
            skeleton->rootJoint->weight=1;
            skeleton->noJoints=0;
            loadJoint(skeleton->rootJoint, skeleton, file);
        }
        else
            cout<<"File doesn't contain ROOT immediately after HIERARCHY\n";

    }
    else
        cout<<"File doesn't start with HIERARCHY\n";

    //cout<<"No. of Joints: "<<skeleton->noJoints<<endl;



}


void loadJoint(JOINT* parent, HIERARCY* skeleton, ifstream* file)
{
    JOINT* joint = new JOINT;

    *file>>joint->name;

    joint->parent=parent;
    parent->children.push_back(joint);

    //joint->children.reserve(5);

    joint->weight=joint->parent->weight/2;
    skeleton->noJoints=skeleton->noJoints+1;


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

            ending->weight=ending->parent->weight/2;
            skeleton->noJoints=skeleton->noJoints+1;


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


void loadMotion(HIERARCY* skeleton ,ifstream* file)
{

    string line;
    *file>>line;

    //float* animation;
    if(line=="MOTION")
    {
        *file>>line;   //Frames:
        *file>>skeleton->noFrames;

        float frameTime;
        *file>>line>>line;   //Frame Time:
        *file>>frameTime;

        skeleton->frameRate=1/frameTime;

        skeleton->animation = new float[(skeleton->noFrames)*(skeleton->totnumChannels)];
        for(int i=0;i<skeleton->noFrames;i++)
        {
            for(int j=0;j<skeleton->totnumChannels;j++)
            {
                *file>>skeleton->animation[i*(skeleton->totnumChannels)+j];
            }
        }

    }
    else
    {
        cout<<"MOTION not found after the HIERARACHY tree\n";
    }

    //return animation;

}


void genPointCloud(JOINT* joint, HIERARCY* skeleton, int frame)
{
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


        point* a = new point(0,0,0);
        //cout<<"Frame "<<frame<<": ";
        //cout<<joint->name<<": "<<"Weight: "<<joint->weight<<": ";
        //cout<<a->x<<", "<<a->y<<", "<<a->z<<endl;
        skeleton->point_cloud.push_back(*a);
        skeleton->weight.push_back(joint->weight);

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
            genPointCloud(joint->children[i],skeleton, frame);
        }
        glPopMatrix();
}




