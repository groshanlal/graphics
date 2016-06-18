#include<fstream>
#include<iostream>
#include<string.h>
#include<vector>

#include "bvh.hpp"

using namespace std;

HIERARCY* skeleton=new HIERARCY;

HIERARCY* showHierarchy()
{
    return skeleton;
}
void loadBvh(ifstream* file)
{
    loadHierarchy(skeleton, file);
    loadMotion(skeleton, file);

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



