#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

class Node{
	public:
		int x1, y1, x2, y2;
		int color; // 0: not defined   1: colorA   2: colorB
		int id;
		bool grouped;
		Node* parent;
		set<Node*> neighbors;
		Node(int x1, int y1, int x2, int y2, int id);
		float getArea();
		float area(int topBound, int bottomBound, int leftBound, int rightBound);

		friend ostream& operator<< (ostream& os, const Node& node);
};

class Group{
	public:
		int leftBound, rightBound, topBound, bottomBound;
		bool colorable;
		bool determinedColor;
		float areaDifference; // color1 - color2
		vector<Node*> color1;
		vector<Node*> color2;
		Group();
		void intitialColor();
		void reverseColor();
		void addColor1(Node* n);
		void addColor2(Node* n);

		friend ostream& operator<< (ostream& os, const Group& group);
};

class Window{
	public:
		int id;
		int leftBound, rightBound, topBound, bottomBound;
		int size;
		float color1Density, color2Density;
		float curDifference;
		int crossGroup;
		vector<Group*> groups;
		Window(int id, int leftBound, int rightBound, int topBound, int bottomBound, int size);
		void calculateDensity();
		bool containNode(Node* node);
		bool containGroup(Group* group);
		bool isCrossGroup(Group* group);
		void initial();
		void adjustColor();
		float calculateGroupsAreaInWindow();
		void sortGroupsByDifference();

		friend ostream& operator<< (ostream& os, const Window& window);
};

class Graph{
	public:
		~Graph();
		int xspace, yspace;
		int windowSize;
		int boxTop, boxBottom, boxRight, boxLeft; // for coloring bounding box
		vector<Node*> nodes;
		vector<Group*> groups;
		vector<Window*> windows;
		Graph();
		void addNode(int x1, int y1, int x2, int y2, int id);
		void addEdge(Node* n1, Node* n2);
		void sortNodesById();
		void sortNodesByX();
		void sortNodesByY();
		void sortWindows();
		void sortWindowsById();
		void readInput(char* filename);
		void constructEdge();
		void groupNode();
		void constructBox();
		void constructWindow();
		void adjustWindowsColor();
		void calculateWindowsDensity();
		void showGroups() const;
		void showWindows() const;
		void outputResult(const char *filename);
};

#endif