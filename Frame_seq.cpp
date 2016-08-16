#include "Frame_seq.hpp"
#include <iostream>

std::vector<Frame>* Frame_seq::getFrames(){
	return frames;
}

void Frame_seq::setFrames(std::vector<Frame>* frames){
	this->frames = frames;
}

void Frame_seq::print()
{
	//std::cout<<"No. of Frames: "<<(*frames).size()<<": ";
	for(int i=0;i<(*frames).size();i++)
		(*frames)[i].print();
}
