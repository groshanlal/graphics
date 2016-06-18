#ifndef bvh_hpp
#define bvh_hpp

#include<string.h>
#include<fstream>
#include<vector>

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

typedef struct
{
    JOINT* rootJoint;
    int totnumChannels;
    float* animation;
    int noFrames;
    int frameRate;

}
HIERARCY;

HIERARCY* showHierarchy();

void loadBvh(ifstream* file);

void loadHierarchy(HIERARCY* skeleton, ifstream* file);

void loadJoint(JOINT* parent, HIERARCY* skeleton, ifstream* file);

void displayJoint(JOINT* joint);

void loadMotion(HIERARCY* skeleton ,ifstream* file);


#endif
