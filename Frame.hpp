#include<vector>

class Frame {
	std::vector<float> *channelValues;
public:
	Frame(){
		channelValues = new std::vector<float>;
	}
	Frame(std::vector<float> *channelValues){
		this->channelValues = channelValues;
	}

	std::vector<float> * getChannelValues();
	void setChannelValues(std::vector<float> * channelValues);
	void print();
};
