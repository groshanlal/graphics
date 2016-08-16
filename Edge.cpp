#include "Edge.hpp"
#include <iostream>

Frame_seq Edge::getFrame_seq(){
	return frames;
}
void Edge::setFrame_seq(Frame_seq frames){
	this->frames = frames;
}

void Edge::print()
{
	std::cout<<theta<<" "<<x<<" "<<z<<std::endl;
	frames.print();
}

void Edge::setRealignment(float theta, float x, float z)
{
	this->theta=theta;
	this->x=x;
	this->z=z;
}
float* Edge::getRealignment()
{
	float* realign = new float[3];
	realign[0]=theta;
	realign[1]=x;
	realign[2]=z;
	return realign;
}

void Edge::setAnimation()
{
	std::vector<Frame>* vf = frames.getFrames();
	Frame f = vf->at(0);
	std::vector<float>* qf= f.getChannelValues();

	animation = new float[(qf->size())*(vf->size())];

	for(int i=0;i<vf->size();i++)
	{
		Frame f = vf->at(i);
		std::vector<float>* qf= f.getChannelValues();

		for(int j=0;j<qf->size();j++)
		{
				animation[i*(qf->size())+j] = qf->at(j);

		}

	}
	noFrames=vf->size();
}

float* Edge::getAnimation()
{
	return animation;
}

int Edge::getNoFrames()
{
	return noFrames;
}
