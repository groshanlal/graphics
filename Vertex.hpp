#include <string>
class Vertex{
	std::string clipIdentifier;
	int frameNumber;
public:
	Vertex(){}
	Vertex(std::string clipIdentifier,int frameNumber){
		this->clipIdentifier = clipIdentifier;
		this->frameNumber = frameNumber;
	}

	std::string  getClipIdentifier();
	void  setClipIdentifier(std::string clipIdentifier);

	int  getFrameNumber();
	void  setFrameNumber(int frameNumber);

};
