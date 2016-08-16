#include "Vertex.hpp"
#include <iostream>

std::string  Vertex::getClipIdentifier(){
	return clipIdentifier;
}
void  Vertex::setClipIdentifier(std::string clipIdentifier){
	this->clipIdentifier = clipIdentifier;
}

int  Vertex::getFrameNumber(){
	return frameNumber;
}

void  Vertex::setFrameNumber(int frameNumber){
	this->frameNumber = frameNumber;
}
