#include "Frame_seq.hpp"

class Edge{
	Frame_seq frames;
	float theta, x, z;
	float* animation;
	int noFrames;
public:
	Edge(){
		frames = *(new Frame_seq());
	}
	Edge(Frame_seq frames){
		this->frames = frames;
		
	}

	Frame_seq getFrame_seq();
	void setFrame_seq(Frame_seq frames);
	void setRealignment(float theta, float x, float z);
	float* getRealignment();
	void setAnimation();
	float* getAnimation();
	int getNoFrames();
	void print();
};
