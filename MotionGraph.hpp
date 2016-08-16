#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/utility.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include "boost/tuple/tuple.hpp"
#include <stdlib.h>
#include "Vertex.hpp"
#include "Edge.hpp"
#include <map>
#include <vector>
#include "bvh.hpp"

using namespace boost;
typedef adjacency_list<vecS, vecS, bidirectionalS,Vertex,Edge> Graph;
typedef typename graph_traits<Graph>::vertex_descriptor VertexDescriptor;
typedef typename graph_traits<Graph>::edge_descriptor EdgeDescriptor;
typedef typename graph_traits<Graph>::out_edge_iterator OutEdgeIterator;
typedef typename graph_traits<Graph>::vertex_iterator VertexIterator;

class MotionGraph{
	public:
		Graph graph;
		VertexDescriptor rootNode;
		MotionGraph(){
		graph = Graph(0);
	}
	VertexDescriptor addVertex(Vertex vertex);
	VertexDescriptor FindVertex(int framenum);
	EdgeDescriptor addEdges(Edge edge,VertexDescriptor u,VertexDescriptor v);
	void removeVertex(VertexDescriptor v);
	void removeEdge(EdgeDescriptor e);
	void removeEdge(VertexDescriptor u,VertexDescriptor v);
	void setRootNode(VertexDescriptor rootNode);
	VertexDescriptor getRootNode();
	std::vector<Frame>* randomTraversal(int depth);
	void create(vector<int>* source, vector<int>* destination, HIERARCY* skeleton);
	void print();
};

void initialize_mog();
float* getAnim();
float* getRealign();
int getNoFrames();
void nextEdge();
