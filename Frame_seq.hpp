#include "Frame.hpp"

class Frame_seq{
	std::vector<Frame> *frames;
public:
	Frame_seq(){
		frames = new std::vector<Frame>;
	}
	Frame_seq(std::vector<Frame> *frames){
		this->frames = frames;
	}

	std::vector<Frame>* getFrames();
	void setFrames(std::vector<Frame>* frames);
	void print();
};
