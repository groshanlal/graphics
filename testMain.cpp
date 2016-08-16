#include <iostream>
#include <vector>
#include "MotionGraph.hpp"

using namespace std;

int main(){
/*
	MotionGraph motionGraph;
	Vertex v1("mocap 08_01",0);
	Vertex v2("mocap 08_02",40);
	Frame_seq frames;
	vector<Frame> frameVec;


	for(int i=0;i<40;i++){
		vector<float> *v = new vector<float>();
		v->push_back(i);
		Frame frame(v);
		frameVec.push_back(frame);
	}

	frames.setFrames(&frameVec);
	Edge e12(frames);
	Edge e21(frames);
	VertexDescriptor vd1 = motionGraph.addVertex(v1);
	VertexDescriptor vd2 = motionGraph.addVertex(v2);
	EdgeDescriptor ed12 = motionGraph.addEdges(e12,vd1,vd2);
	EdgeDescriptor ed21 = motionGraph.addEdges(e21,vd2,vd1);

	motionGraph.setRootNode(vd1);
	vector<Frame> * v = motionGraph.randomTraversal(3);

	for(int i=0;i<v->size();i++){
		cout<<(*((*v)[i].getChannelValues()))[0]<<" ";
	}
	cout<<endl;

*/

	int n;
	cin>>n;

	vector<int>fr_source;
	vector<int>fr_dest;
	vector<int>fr;


	int f1,f2;
	for(int i=0;i<n;i++)
    {
        cin>>f1;
        cin>>f2;
        fr_source.push_back(f1);
        fr_source.push_back(f2);
        fr.push_back(f1);
        fr.push_back(f1+40);
    }
    sort(fr.begin(),fr.end());

    MotionGraph motionGraph;
    VertexDescriptor vd1, vd2;
    vd1 = motionGraph.addVertex(Vertex("mocap 09_12",fr[0]));
    motionGraph.setRootNode(vd1);
		for(int i=1;i<fr.size();i++)
    {
				vd2 = motionGraph.addVertex(Vertex("mocap 09_12",fr[i]));
				Frame_seq frames;
        vector<Frame>* frameVec=new vector<Frame>();

        for(int j=fr[i-1];j<fr[i];j++)
        {
            vector<float> *v = new vector<float>();
            v->push_back(j);
            Frame frame(v);
						frameVec->push_back(frame);
        }
        frames.setFrames(frameVec);
				Edge e(frames);
				EdgeDescriptor ed = motionGraph.addEdges(e,vd1,vd2);
				vd1 = vd2;
				//cout<<"Current Frames\n";
				//e.print();
				//motionGraph.graph[ed].print();
		}


		//motionGraph.print();
		vector<Frame> * v = motionGraph.randomTraversal(3);
		for(int i=0;i<v->size();i++){
			v->at(i).print();
		}



}
