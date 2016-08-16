#include "MotionGraph.hpp"
#include "transitions.hpp"

#include <iostream>
#include <vector>


void MotionGraph::create(vector<int>* source, vector<int>* destination, HIERARCY* skeleton)
{
	std::vector<int> nodes;
	for(int i=0;i<source->size();i++)
	{
		nodes.push_back(source->at(i));
		nodes.push_back(destination->at(i)+40);
	}
	nodes.push_back(0);
	nodes.push_back(skeleton->noFrames);
	sort(nodes.begin(),nodes.end());

	//Add Vertices
	std::vector<VertexDescriptor> vd;
	for(int i=0;i<nodes.size();i++)
				vd.push_back(addVertex(Vertex("mocap 09_12",nodes[i])));

	cout<<"Done with vertices\n";

	//Set Root node
	setRootNode(vd[0]);
	cout<<"Done with root-node\n";


	//Add Primary Edges
	for(int i=0;i+1<vd.size();i++)
	{
		Frame_seq frames = *(new Frame_seq);
		vector<Frame>* frameVec=new vector<Frame>();

		for(int j=nodes[i];j<nodes[i+1];j++)
		{
				vector<float> *v = new vector<float>();
				int totnumChannels = skeleton->totnumChannels;
				float* motion = skeleton->animation;
				for(int k=0;k<totnumChannels;k++)
						v->push_back(motion[j*totnumChannels+k]);
				Frame frame(v);
				frameVec->push_back(frame);
		}
		frames.setFrames(frameVec);
		Edge e(frames);
		e.setRealignment(0,0,0);
		e.setAnimation();
		addEdges(e, vd[i], vd[i+1]);

	}
	cout<<"Done with primary edges\n";

	//Add Secondary Edges
	for(int i=0;i<source->size();i++)
	{
		Frame_seq frames;
		vector<Frame>* frameVec=new vector<Frame>();

		float* motion = interp_frames(source->at(i), destination->at(i), skeleton);
		//float* motion = skeleton->animation;
		float theta,x,z;
		align(&theta, &x, &z, source->at(i), destination->at(i), skeleton);

		for(int j=0;j<40;j++)
		{
				vector<float> *v = new vector<float>();
				int totnumChannels = skeleton->totnumChannels;
				for(int k=0;k<totnumChannels;k++)
						v->push_back(motion[j*totnumChannels+k]);
				Frame frame(v);
				frameVec->push_back(frame);
		}
		frames.setFrames(frameVec);
		Edge e(frames);
		//cout<<"("<<source->at(i)<<", "<<destination->at(i)+40<<"): "<<theta<<", "<<x<<", "<<z<<endl;
		e.setRealignment(theta,x,z);
		e.setAnimation();
		VertexDescriptor vd1 = FindVertex(source->at(i));
		VertexDescriptor vd2 = FindVertex(destination->at(i)+40);
		addEdges(e, vd1, vd2);
	}
	cout<<"Done with secondary edges\n";


}


VertexDescriptor MotionGraph::addVertex(Vertex vertex){
	VertexDescriptor v = add_vertex(graph);
	graph[v] = vertex;
	return v;
}

VertexDescriptor MotionGraph::FindVertex(int framenum)
{
	std::pair<VertexIterator, VertexIterator> vp;
	//std::cout<<"Searching Vertex "<<framenum<<std::endl;
	for( vp = vertices(graph); vp.first != vp.second; ++vp.first)
	{
		if(graph[*vp.first].getFrameNumber()==framenum)
					return *vp.first;
	}
	std::cout<<"Vertex not found\n";
}

EdgeDescriptor MotionGraph::addEdges(Edge edge,VertexDescriptor u,VertexDescriptor v){
	EdgeDescriptor e;bool b;
	tie(e,b) = add_edge(u,v,graph);
	graph[e]=edge;
	return e;
}

void MotionGraph::removeVertex(VertexDescriptor v){
	clear_vertex(v,graph);
	remove_vertex(v,graph);
}

void MotionGraph::removeEdge(EdgeDescriptor e){
	remove_edge(e,graph);
}

void MotionGraph::removeEdge(VertexDescriptor u,VertexDescriptor v){
	remove_edge(u,v,graph);
}

void MotionGraph::setRootNode(VertexDescriptor rootNode){
	this->rootNode = rootNode;
}

VertexDescriptor MotionGraph::getRootNode(){
	return rootNode;
}

std::vector<Frame>* MotionGraph::randomTraversal(int depth){
	std::vector<Frame> *v = new std::vector<Frame>() ;
	VertexDescriptor curr = rootNode;
	for(int i=0;i<depth;i++)
	{
		int deg = out_degree(curr,graph);
		if(deg==0)
		{
			delete v;
			return NULL;
		}
		OutEdgeIterator j,end;
		tie(j,end) = out_edges(curr,graph);
		int chosen = rand()%deg;
		j+=chosen;
		EdgeDescriptor e = *j;


		v->insert(v->end(),graph[e].getFrame_seq().getFrames()->begin(),graph[e].getFrame_seq().getFrames()->end());
		VertexDescriptor targetVertex = target(e,graph);
		curr = targetVertex;
	}
	return v;
}

void MotionGraph::print()
{
	std::cout<<"Vertices:\n";
	std::pair<VertexIterator, VertexIterator> vp;
  for (vp = vertices(graph); vp.first != vp.second; ++vp.first)
  	std::cout << graph[*vp.first].getFrameNumber() <<  " ";
  std::cout << std::endl;

	std::cout<<"Edges:\n";
	OutEdgeIterator ei, ei_end;
	for (vp = vertices(graph); vp.first != vp.second; ++vp.first)
  {
		VertexDescriptor curr=*vp.first;
		for (tie(ei, ei_end) = out_edges(curr,graph); ei != ei_end; ++ei)
		{
			std::cout << "(" << graph[source(*ei, graph)].getFrameNumber()<< ","
											<< graph[target(*ei, graph)].getFrameNumber() << "): \n";
			EdgeDescriptor ed = *ei;
			graph[ed].print();
		}
		std::cout << std::endl;
	}
}

MotionGraph motionGraph;
VertexDescriptor curr;
OutEdgeIterator curr_edge;

void initialize_mog()
{
	int n;
	cin>>n;

	vector<int>fr_source;
	vector<int>fr_dest;

	int f1,f2;
	for(int i=0;i<n;i++)
	{
	    cin>>f1;
	    cin>>f2;
	    fr_source.push_back(f1);
	    fr_dest.push_back(f2);
	}

	motionGraph.create(&fr_source, &fr_dest, showHierarchy());
	//motionGraph.print();

	curr = motionGraph.getRootNode();
	int deg = out_degree(curr,motionGraph.graph);
	OutEdgeIterator j,end;
	tie(j,end) = out_edges(curr,motionGraph.graph);
	int chosen = rand()%deg;
	curr_edge=j+chosen;

}

float* getAnim()
{
	EdgeDescriptor ed = *curr_edge;
	Edge e = motionGraph.graph[ed];
	return e.getAnimation();
}

float* getRealign()
{
	EdgeDescriptor ed = *curr_edge;
	Edge e = motionGraph.graph[ed];
	return e.getRealignment();
}

int getNoFrames()
{
	EdgeDescriptor ed = *curr_edge;
	Edge e = motionGraph.graph[ed];
	return e.getNoFrames();
}

void nextEdge()
{

	EdgeDescriptor ed = *curr_edge;
	VertexDescriptor targetVertex = target(ed,motionGraph.graph);
	curr = targetVertex;

	int deg = out_degree(curr,motionGraph.graph);
	OutEdgeIterator j,end;
	tie(j,end) = out_edges(curr,motionGraph.graph);
	int chosen = rand()%deg;
	curr_edge=j+chosen;

	ed = *curr_edge;
	std::cout << "(" << motionGraph.graph[source(ed, motionGraph.graph)].getFrameNumber()<< ","
									<< motionGraph.graph[target(ed, motionGraph.graph)].getFrameNumber() << "): \n";


}
