#include<Windows.h>
#include<GL/glut.h>

#include<fstream>
#include<math.h>
#include<iostream>
#include<string.h>
#include<vector>

#include "motion_graph.hpp"

#define PI 3.14159

using namespace std;

int transition_length;
float* error;

vector<float>local_min_i;
vector<float>local_min_j;
vector<float>local_min_val;

vector<float>sw_local_min_i;
vector<float>sw_local_min_j;
vector<float>sw_local_min_val;

void align(float* theta, float* x, float* z, int first, int second, HIERARCY* skeleton)
{

    transition_length=skeleton->frameRate/3;

    float xf=0,zf=0,xs=0,zs=0;

    for(int i=first*(skeleton->noJoints);i<(first+transition_length)*(skeleton->noJoints);i++)
    {
        xf=xf+(skeleton->point_cloud[i].x)*(skeleton->weight[i]);
        zf=zf+(skeleton->point_cloud[i].z)*(skeleton->weight[i]);
    }

    for(int i=second*(skeleton->noJoints);i<(second+transition_length)*(skeleton->noJoints);i++)
    {
        xs=xs+(skeleton->point_cloud[i].x)*(skeleton->weight[i]);
        zs=zs+(skeleton->point_cloud[i].z)*(skeleton->weight[i]);
    }

    float total_weight=0;

    for(int i=0;i<(transition_length)*(skeleton->noJoints);i++)
    {
        total_weight = total_weight + skeleton->weight[i];
    }


    float num=0, den=0;

    for(int i=0;i<(transition_length)*(skeleton->noJoints);i++)
    {
       float w = skeleton->weight[i];

       float x1 = skeleton->point_cloud[first*(skeleton->noJoints)+i].x;
       float z1 = skeleton->point_cloud[first*(skeleton->noJoints)+i].z;

       float x2 = skeleton->point_cloud[second*(skeleton->noJoints)+i].x;
       float z2 = skeleton->point_cloud[second*(skeleton->noJoints)+i].z;

       num = num + (w)*((x1)*(z2)-(x2)*(z1));
       den = den + (w)*((x1)*(x2)-(z2)*(z1));
    }

    num = num - (xf*zs-zf*xs)/total_weight;
    den = den - (xf*xs-zf*zs)/total_weight;

    *theta = atan(num/den);

    *x = (xf - xs*cos(*theta) - zs*sin(*theta))/total_weight;
    *z = (zf - zs*cos(*theta) + xs*sin(*theta))/total_weight;

}

void errmatrix(HIERARCY* skeleton)
{
    transition_length=skeleton->frameRate/3;

    //cout<<transition_length<<endl;
    //cout<<skeleton->noJoints<<endl;

    error = new float[(skeleton->noFrames - transition_length + 1)*(skeleton->noFrames - transition_length + 1)];

    float theta0, x0, z0;
    /*
    align(&theta0, &x0, &z0, 0, 10, skeleton);
    cout<<theta0<<", "<<x0<<", "<<z0<<endl;
    align(&theta0, &x0, &z0, 0, 20, skeleton);
    cout<<theta0<<", "<<x0<<", "<<z0<<endl;
    align(&theta0, &x0, &z0, 0, 30, skeleton);
    cout<<theta0<<", "<<x0<<", "<<z0<<endl;
    align(&theta0, &x0, &z0, 0, 40, skeleton);
    cout<<theta0<<", "<<x0<<", "<<z0<<endl;

    point a(1,2,3);
    cout<<a.x<<", "<<a.y<<", "<<a.z<<endl;
    point b = a.trans(PI/6, 3,5);
    cout<<b.x<<", "<<b.y<<", "<<b.z<<endl;
    */



    //cout<<skeleton->noFrames<<", "<<transition_length<<", "<<skeleton->noFrames-transition_length+1<<endl;
    for(int i=0;i<skeleton->noFrames-transition_length+1;i++)
    {
        for(int j=0;j<skeleton->noFrames-transition_length+1;j++)
        {
            align(&theta0, &x0, &z0, i, j, skeleton);
            float e=0;
            for(int k=0;k<transition_length*(skeleton->noJoints);k++)
            {
                     float w = skeleton->weight[k];
                     point p1 = skeleton->point_cloud[i*(skeleton->noJoints)+k];
                     point p2 = skeleton->point_cloud[j*(skeleton->noJoints)+k];
                     point p3 = p2.trans(theta0,x0,z0);

                     e = e + w*(p1.x - p3.x)*(p1.x - p3.x);
                     e = e + w*(p1.y - p3.y)*(p1.y - p3.y);
                     e = e + w*(p1.z - p3.z)*(p1.z - p3.z);

            }
            //cout<<i<<": "<<j<<endl;
            //cout<<e<<",";
            error[i*(skeleton->noFrames-transition_length+1)+j]=e;
        }
        //cout<<endl;
    }


    /*
    for(int i=0;i<skeleton->noFrames-transition_length+1;i=i+1)
    {
        //cout<<error[2*(skeleton->noFrames-transition_length+1)+i]<<endl;
        align(&theta0, &x0, &z0, 0, i, skeleton);
        cout<<theta0<<"; "<<x0<<", "<<z0<<endl;
    }
    */


    for(int i=0;i<skeleton->noFrames-transition_length+1;i++)
    {
        for(int j=0;j<skeleton->noFrames-transition_length+1;j++)
        {
            float e = error[i*(skeleton->noFrames-transition_length+1)+j];
            if(i==j)
                continue;
            if((i-1>=0)&&(e>error[(i-1)*(skeleton->noFrames-transition_length+1)+j]))
                continue;
            if((i+1<skeleton->noFrames-transition_length+1)&&(e>error[(i+1)*(skeleton->noFrames-transition_length+1)+j]))
                continue;
            if((j-1>=0)&&(e>error[i*(skeleton->noFrames-transition_length+1)+j-1]))
                continue;
            if((j+1<skeleton->noFrames-transition_length+1)&&(e>error[i*(skeleton->noFrames-transition_length+1)+j+1]))
                continue;
            local_min_i.push_back(i);
            local_min_j.push_back(j);
            local_min_val.push_back(e);
        }
    }

    /*
    for(int i=0;i<local_min_i.size();i++)
        cout<<local_min_i[i]<<", "<<local_min_j[i]<<": "<<local_min_val[i]<<endl;
    */

    float thresh = 0.5*(skeleton->noJoints)*transition_length;
    for(int i=0;i<local_min_i.size();i++)
    {
        if(local_min_val[i]<thresh)
        {
            sw_local_min_i.push_back(local_min_i[i]);
            sw_local_min_j.push_back(local_min_j[i]);
            sw_local_min_val.push_back(local_min_val[i]);
        }
    }




    for(int i=0;i<sw_local_min_i.size();i++)
        cout<<sw_local_min_i[i]<<", "<<sw_local_min_j[i]<<": "<<sw_local_min_val[i]<<endl;




}

