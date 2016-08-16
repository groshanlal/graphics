#ifndef bvh_hpp
#define bvh_hpp

#include<iostream>
#include<math.h>
#include<string.h>
#include<fstream>
#include<vector>
#include<GL/glut.h>


using namespace std;

#define MAX_NUM_OF_CHANNELS 6
#define PI 3.14159

class point
{
   public:
       float x,y,z;
        point(float dx, float dy, float dz)
        {
            float modelview[16];
            float world_coordinates[4];
            float drawing_coordinates[4];

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

        point trans(float theta, float x0, float z0)
        {
            point a(0,0,0);
            a.x = z * sin(theta) + x * cos(theta) + x0;
            a.y = y;
            a.z = z * cos(theta) - x * sin(theta) + z0;
            return a;
        }


};


typedef struct JOINT
{
    string name;
    JOINT* parent;
    float x_off,y_off,z_off;
    int numChannels;
    int channelPos;
    string channels[MAX_NUM_OF_CHANNELS];
    vector <JOINT*> children;
    float weight;
}
JOINT;

typedef struct
{
    JOINT* rootJoint;
    int totnumChannels;
    float* animation;
    int noFrames;
    int frameRate;
    int noJoints;
    vector <point> point_cloud;
    vector <float> weight;
}
HIERARCY;

HIERARCY* showHierarchy();

//void switch_toTrans();

//void switch_toAct();

void realignXZ();

void loadBvh(ifstream* file);

void loadHierarchy(HIERARCY* skeleton, ifstream* file);

void loadJoint(JOINT* parent, HIERARCY* skeleton, ifstream* file);

void displayJoint(JOINT* joint);

void loadMotion(HIERARCY* skeleton ,ifstream* file);

void genPointCloud(JOINT* joint, HIERARCY* skeleton, int frame);

#endif
