#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include "graph.h"

int main(int argc, char** argv){
	Graph graph;
	//readfile and construct the nodes
	graph.readInput(argv[1]);
	//construct the edges
	graph.constructEdge();
	//group the nodes
	graph.groupNode();
	//construct the bounding box
	graph.constructBox();
	//construct the color windows
	graph.constructWindow();
	//adjust the color
	graph.adjustWindowsColor();
	//calculate color density
	graph.calculateWindowsDensity();

	#ifdef _DEBUG_ON_
		graph.showGroups();
		graph.showWindows();
	#endif

	graph.outputResult(argv[2]);
}
