#include <string>
#include <fstream>
#include <stack>
#include <climits>
#include <cmath>
#include <stdio.h>
#include "graph.h"

Node::Node(int x1, int y1, int x2, int y2, int id):x1(x1), y1(y1), x2(x2), y2(y2), id(id), color(0), grouped(false){}

float Node::getArea(){
	return (x2 - x1)*(y2 - y1);
}

float Node::area(int topBound, int bottomBound, int leftBound, int rightBound){
	if(x1 >= rightBound || x2 <= leftBound || y1 >= topBound || y2 <= bottomBound){
		return 0;
	}
	else{
		int top = y2;
		int bottom = y1;
		int left = x1;
		int right = x2;
		if(left < leftBound){
			left = leftBound;
		}
		if(right > rightBound){
			right = rightBound;
		}
		if(bottom < bottomBound){
			bottom = bottomBound;
		}
		if(top > topBound){
			top = topBound;
		}
		return (top - bottom)*(right - left);
	}
}

Group::Group():colorable(true), determinedColor(false), areaDifference(0), leftBound(INT_MAX), rightBound(INT_MIN), topBound(INT_MIN), bottomBound(INT_MAX){}

void Group::intitialColor(){
	float color1Area = 0.0;
	float color2Area = 0.0;
	for(size_t i = 0, n = color1.size(); i < n; ++i){
		color1Area += color1[i]->getArea();
	}
	for(size_t i = 0, n = color2.size(); i < n; ++i){
		color2Area += color2[i]->getArea();
	}
	if(color2Area > color1Area){
		reverseColor();
	}
}

void Group::reverseColor(){
	color1.swap(color2);
}

void Group::addColor1(Node* n){
	n->color = 1;
	n->grouped = true;
	color1.push_back(n);
	if(n->x1 < leftBound){
		leftBound = n->x1;
	}
	if(n->x2 > rightBound){
		rightBound = n->x2;
	}
	if(n->y1 < bottomBound){
		bottomBound = n->y1;
	}
	if(n->y2 > topBound){
		topBound = n->y2;
	}
}

void Group::addColor2(Node* n){
	n->color = 2;
	n->grouped = true;
	color2.push_back(n);
	if(n->x1 < leftBound){
		leftBound = n->x1;
	}
	if(n->x2 > rightBound){
		rightBound = n->x2;
	}
	if(n->y1 < bottomBound){
		bottomBound = n->y1;
	}
	if(n->y2 > topBound){
		topBound = n->y2;
	}
}

Window::Window(int id, int leftBound, int rightBound, int topBound, int bottomBound, int size):id(id), leftBound(leftBound), rightBound(rightBound), topBound(topBound), bottomBound(bottomBound), size(size), color1Density(0), color2Density(0), curDifference(0), crossGroup(0){}

void Window::calculateDensity(){
	float color1Area = 0.0; // color1
	float color2Area = 0.0; // color2
	for(size_t i = 0; i < groups.size(); ++i){
		for(size_t j = 0; j < groups[i]->color1.size(); ++j){
			color1Area += groups[i]->color1[j]->area(topBound, bottomBound, leftBound, rightBound);
		}
		for(size_t j = 0; j < groups[i]->color2.size(); ++j){
			color2Area += groups[i]->color2[j]->area(topBound, bottomBound, leftBound, rightBound);
		}
	}

	color1Density = round((color1Area/(size*size))*10000)/100;
	color2Density = round((color2Area/(size*size))*10000)/100;
}

bool Window::containNode(Node* node){
	bool bool1 = node->x2 <= leftBound;
	bool bool2 = node->x1 >= rightBound;
	bool bool3 = node->y2 <= bottomBound;
	bool bool4 = node->y1 >= topBound;
	if(bool1 || bool2 || bool3 || bool4){
		return false;
	}
	return true;
}

bool Window::containGroup(Group* group){
	for(size_t i = 0, n = group->color1.size(); i < n; ++i){
		if(containNode(group->color1[i])){
			return true;
		}
	}
	for(size_t i = 0, n = group->color2.size(); i < n; ++i){
		if(containNode(group->color2[i])){
			return true;
		}
	}
	return false;
}

bool Window::isCrossGroup(Group* group){
	if(group->rightBound <= rightBound && group->leftBound >= leftBound && group->topBound <= topBound && group->bottomBound >= bottomBound){
		return false;
	}
	else{
		return true;
	}
}

void Window::initial(){
	curDifference = 0.0;
	float area;
	for(size_t i = 0; i < groups.size(); ++i){
		if(!groups[i]->determinedColor){
			float color1Area = 0.0;
			float color2Area = 0.0;
			for(size_t j = 0; j < groups[i]->color1.size(); ++j){
				area = groups[i]->color1[j]->area(topBound, bottomBound, leftBound, rightBound);
				color1Area += area;
			}
			for(size_t j = 0; j < groups[i]->color2.size(); ++j){
				area = groups[i]->color2[j]->area(topBound, bottomBound, leftBound, rightBound);
				color2Area += area;
			}
			groups[i]->areaDifference = color1Area - color2Area;
		}
		// those group whose color is determined
		else{
			for(size_t j = 0; j < groups[i]->color1.size(); ++j){
				area = groups[i]->color1[j]->area(topBound, bottomBound, leftBound, rightBound);
				curDifference += area;
			}
			for(size_t j = 0; j < groups[i]->color2.size(); ++j){
				area = groups[i]->color2[j]->area(topBound, bottomBound, leftBound, rightBound);
				curDifference -= area;
			}
		}
	}
}

float Window::calculateGroupsAreaInWindow(){
	float totalArea = 0.0;
	for(size_t i = 0; i < groups.size(); ++i){
		float area;
		for(size_t j = 0; j < groups[i]->color1.size(); ++j){
			area = groups[i]->color1[j]->area(topBound, bottomBound, leftBound, rightBound);
			totalArea += area;
		}
		for(size_t j = 0; j < groups[i]->color2.size(); ++j){
			area = groups[i]->color2[j]->area(topBound, bottomBound, leftBound, rightBound);
			totalArea += area;
		}
	}
	return totalArea;
}

bool compareByDifference(Group* A, Group* B){
	return (abs(A->areaDifference) > abs(B->areaDifference));
}

void Window::sortGroupsByDifference(){
	sort(groups.begin(), groups.end(), compareByDifference);
}

void Window::adjustColor(){
	initial();
	sortGroupsByDifference();
	for(size_t i = 0; i < groups.size(); ++i){
		if(!groups[i]->determinedColor){
			// color1 is more
			if(curDifference > 0){
				if(groups[i]->areaDifference > 0){
					groups[i]->reverseColor();
				}
				curDifference -= abs(groups[i]->areaDifference);
			}
			// color2 is more
			else{
				if(groups[i]->areaDifference < 0){
					groups[i]->reverseColor();
				}
				curDifference += abs(groups[i]->areaDifference);
			}
		}
	}
}

Graph::Graph():boxTop(INT_MIN), boxBottom(INT_MAX), boxRight(INT_MIN), boxLeft(INT_MAX){}

void Graph::addNode(int x1, int y1, int x2, int y2, int id){
	Node* newNode = new Node(x1, y1, x2, y2, id);
	nodes.push_back(newNode);
}

Graph::~Graph(){
	for(size_t i = 0, n = windows.size(); i < n; ++i){
		delete windows[i];
	}

	for(size_t i = 0, n = groups.size(); i < n; ++i){
		delete groups[i];
	}

	for(size_t i = 0, n = nodes.size(); i < n; ++i){
		delete nodes[i];
	}
}

void Graph::addEdge(Node* n1, Node* n2){
	n1->neighbors.insert(n2);
	n2->neighbors.insert(n1);
}

bool compareByX(Node* A, Node* B){
	if(A->x1 < B->x1){
		return true;
	}
	else if(A->x1 == B->x1 && A->x2 < B->x2){
		return true;
	}
	else{
		return false;
	}
}

void Graph::sortNodesByX(){
	sort(nodes.begin(), nodes.end(), compareByX);
}

bool compareByY(Node* A, Node* B){
	if(A->y1 < B->y1)
		return true;
	else if(A->y1 == B->y1 && A->y2 < B->y2)
		return true;
	else
		return false;
}

void Graph::sortNodesByY(){
	sort(nodes.begin(), nodes.end(), compareByY);
}

bool compareNodeById(Node* A, Node* B){
	return (A->id < B->id);
}

void Graph::sortNodesById(){
	sort(nodes.begin(), nodes.end(), compareNodeById);
}

bool compareWindowById(Window* A, Window* B){
	return (A->id < B->id);
}

void Graph::sortWindowsById(){
	sort(windows.begin(), windows.end(), compareWindowById);
}

bool compareWindow(Window* A, Window* B){
	// if(A->crossGroup > B->crossGroup){
	// 	return true;
	// }
	// else if (A->crossGroup < B->crossGroup){
	// 	return false;
	// }
	// else{
	// 	return A->calculateGroupsAreaInWindow() > B->calculateGroupsAreaInWindow();
	// }
	return A->calculateGroupsAreaInWindow() > B->calculateGroupsAreaInWindow();
}

void Graph::sortWindows(){
	sort(windows.begin(), windows.end(), compareWindow);
}

void Graph::readInput(char* filename){
	ifstream input;
	input.open(filename);
	string str;
	//get the X-spacing
	getline(input, str);
	int pos = str.find("=");
	str = str.substr(pos+1,str.length()-pos-1);
	xspace = atoi(str.c_str());
	//get the Y-spacing
	getline(input, str);
	pos = str.find("=");
	str = str.substr(pos+1,str.length()-pos-1);
	yspace = atoi(str.c_str());
	//get the color window size
	getline(input, str);
	pos = str.find("=");
	str = str.substr(pos+1,str.length()-pos-1);
	windowSize = atoi(str.c_str());
	//construct the nodes
	int x1, x2, y1, y2;
	int id = 1;
	while(!input.eof()){
		getline(input, str);
		if(str != ""){
			int prepos = 0;
			pos = str.find(",");
			x1 = atoi(str.substr(0, pos).c_str());
			prepos = pos;
			pos = str.find(",", prepos+1);
			y1 = atoi(str.substr(prepos+1, pos-prepos-1).c_str());
			prepos = pos;
			pos = str.find(",", prepos+1);
			x2 = atoi(str.substr(prepos+1, pos-prepos-1).c_str());
			prepos = pos;
			y2 = atoi(str.substr(prepos+1, str.length()-prepos-1).c_str());
			addNode(x1, y1, x2, y2, id);
			id++;
		}
	}
	input.close();
}

void Graph::constructEdge(){
	sortNodesByX();
	for(size_t i = 0; i < nodes.size(); ++i){
		for(size_t j = i+1; j < nodes.size(); ++j){
			if(nodes[i]->x2 <= nodes[j]->x1){
				break;
			}
			else{
				//node i is above
				if(nodes[i]->y1 >= nodes[j]->y2){
					if((nodes[i]->y1 - nodes[j]->y2) < yspace){
						addEdge(nodes[i], nodes[j]);
					}
				}
				//node i is below
				else{
					if((nodes[j]->y1 - nodes[i]->y2) < yspace){
						addEdge(nodes[i], nodes[j]);
					}
				}
			}
		}
	}
	sortNodesByY();
	for(size_t i = 0; i < nodes.size(); ++i){
		for(size_t j = i+1; j < nodes.size(); ++j){
			if(nodes[i]->y2 <= nodes[j]->y1){
				break;
			}
			else{
				//node i is left
				if(nodes[j]->x1 >= nodes[i]->x2){
					if((nodes[j]->x1 - nodes[i]->x2) < xspace){
						addEdge(nodes[i], nodes[j]);
					}
				}
				//node i is right
				else{
					if((nodes[i]->x1 - nodes[j]->x2) < xspace){
						addEdge(nodes[i], nodes[j]);
					}
				}
			}
		}
	}
}

void Graph::groupNode(){
	stack<Node*> s;
	set<Node*>::iterator it;
	sortNodesById();
	for(size_t i = 0; i < nodes.size(); ++i){
		if(!nodes[i]->grouped){
			Group* newGroup = new Group();
			newGroup->addColor1(nodes[i]);
			s.push(nodes[i]);
			while(!s.empty()){
				bool unvisitedNeighbor = false;
				Node* top = s.top();
				for(it = top->neighbors.begin(); it != top->neighbors.end(); ++it){
					if(!(*it)->grouped){
						(*it)->parent = top;
						if(top->color == 1){
							newGroup->addColor2(*it);
							s.push(*it);
						}
						else{
							newGroup->addColor1(*it);
							s.push(*it);
						}
						unvisitedNeighbor = true;
						break;
					}
					else{
						//conflict
						if((*it)->color == top->color && *it != top->parent){
							newGroup->colorable = false;
						}
					}
				}
				if(!unvisitedNeighbor){
					s.pop();
				}
			}
			if(newGroup->colorable){
				newGroup->intitialColor();
			}
			groups.push_back(newGroup);
		}
	}
}

void Graph::constructBox(){
	for(size_t i = 0; i < groups.size(); ++i){
		if(groups[i]->colorable){
			if(groups[i]->topBound > boxTop){
				boxTop = groups[i]->topBound;
			}
			if(groups[i]->bottomBound < boxBottom){
				boxBottom = groups[i]->bottomBound;
			}
			if(groups[i]->rightBound > boxRight){
				boxRight = groups[i]->rightBound;
			}
			if(groups[i]->leftBound < boxLeft){
				boxLeft = groups[i]->leftBound;
			}
		}
	}
}

void Graph::constructWindow(){
	float boxWidth = boxRight - boxLeft;
	float boxLength = boxTop - boxBottom;
	int widthCount = ceil(boxWidth/windowSize);
	int lengthCount = ceil(boxLength/windowSize);
	int x1;
	int x2;
	int y1 = boxBottom;
	int y2 = y1 + windowSize;
	int windowId = 1;
	for(int i = 0; i < lengthCount; ++i){
		x1 = boxLeft;
		x2 = x1 + windowSize;
		for(int j = 0; j < widthCount; ++j){
			Window* newWindow = new Window(windowId, x1, x2, y2, y1, windowSize);
			windowId ++;
			for(size_t k = 0; k < groups.size(); ++k){
				if(groups[k]->colorable && newWindow->containGroup(groups[k])){
					if(newWindow->isCrossGroup(groups[k])){
						newWindow->crossGroup++;
					}
					newWindow->groups.push_back(groups[k]);
				}
			}
			windows.push_back(newWindow);
			if(j == widthCount-2){
				x2 = boxRight;
				x1 = x2 - windowSize;
			}
			else{
				x1 += windowSize;
				x2 += windowSize;
			}
		}
		if(i == lengthCount-2){
			y2 = boxTop;
			y1 = y2 - windowSize;
		}
		else{
			y1 += windowSize;
			y2 += windowSize;
		}
	}
}

void Graph::adjustWindowsColor(){
	//sort the windows
	//sortWindows();
	sortWindowsById();
	for(size_t i = 0; i < windows.size(); ++i){
		windows[i]->adjustColor();
	}
}

void Graph::calculateWindowsDensity(){
	for(size_t i = 0; i < windows.size(); ++i){
		windows[i]->calculateDensity();
	}
}

void Graph::showGroups() const{
	for(size_t i = 0, n = groups.size(); i < n; ++i){
		cout << *(groups[i]);
	}
}

void Graph::showWindows() const{
	for(size_t i = 0, n = windows.size(); i < n; ++i){
		cout << *(windows[i]);
	}
}

void Graph::outputResult(const char *filename){
	sortWindowsById();

	FILE *fp = fopen(filename, "w");
	// output window
	for(size_t i = 0, n = windows.size(); i < n; ++i){
		int x1 = windows[i]->leftBound;
		int y1 = windows[i]->bottomBound;
		int x2 = windows[i]->rightBound;
		int y2 = windows[i]->topBound;
		double d1 = windows[i]->color1Density;
		double d2 = windows[i]->color2Density;
		fprintf(fp, "WIN[%lu]=%d,%d,%d,%d(%.2f %.2f)\n", i+1, x1, y1, x2, y2, d1, d2);
	}

	// output non-colorable group
	for(size_t i = 0, n = groups.size(); i < n; ++i){
		if(!groups[i]->colorable){
			fprintf(fp, "GROUP\n");
			// need to be checked, whether all the node are outputed
			size_t id = 1;
			for(size_t j = 0, k = groups[i]->color1.size(); j < k; ++j){
				int x1 = groups[i]->color1[j]->x1;
				int y1 = groups[i]->color1[j]->y1;
				int x2 = groups[i]->color1[j]->x2;
				int y2 = groups[i]->color1[j]->y2;
				fprintf(fp, "NO[%lu]=%d,%d,%d,%d\n", id, x1, y1, x2, y2);
				++ id;
			}
			for(size_t j = 0, k = groups[i]->color2.size(); j < k; ++j){
				int x1 = groups[i]->color2[j]->x1;
				int y1 = groups[i]->color2[j]->y1;
				int x2 = groups[i]->color2[j]->x2;
				int y2 = groups[i]->color2[j]->y2;
				fprintf(fp, "NO[%lu]=%d,%d,%d,%d\n", id, x1, y1, x2, y2);
				++ id;
			}
		}
	}
	// output colorable group
	for(size_t i = 0, n = groups.size(); i < n; ++i){
		if(groups[i]->colorable){
			fprintf(fp, "GROUP\n");
			for(size_t j = 0, k = groups[i]->color1.size(); j < k; ++j){
				int x1 = groups[i]->color1[j]->x1;
				int y1 = groups[i]->color1[j]->y1;
				int x2 = groups[i]->color1[j]->x2;
				int y2 = groups[i]->color1[j]->y2;
				fprintf(fp, "CA[%lu]=%d,%d,%d,%d\n", j+1, x1, y1, x2, y2);
			}
			for(size_t j = 0, k = groups[i]->color2.size(); j < k; ++j){
				int x1 = groups[i]->color2[j]->x1;
				int y1 = groups[i]->color2[j]->y1;
				int x2 = groups[i]->color2[j]->x2;
				int y2 = groups[i]->color2[j]->y2;
				fprintf(fp, "CB[%lu]=%d,%d,%d,%d\n", j+1, x1, y1, x2, y2);
			}
		}
	}
	fclose(fp);
	cout << "Finished !!!" << endl;
}

ostream& operator<< (ostream& os, const Node& node){
	os << "node" << node.id << " color" << node.color << " ";
	os << node.x1 << " " << node.y1 << " " << node.x2 << " " << node.x2 << endl;
	return os;
}

ostream& operator<< (ostream& os, const Group& group){
	os << "=========== Group ===========" << endl;
	if (!group.colorable){
		os << "Non-colorable !!!" << endl;
	}
	os << "Color 1 :" << endl;
	for(size_t i = 0, n = group.color1.size(); i < n; ++i){
		os << *(group.color1[i]);
	}
	os << "Color 2 :" << endl;
	for(size_t i = 0, n = group.color2.size(); i < n; ++i){
		os << *(group.color2[i]);
	}
	return os;
}

ostream& operator<< (ostream& os, const Window& window){
	os << "/////////// Window //////////////////////" << endl;
	os << "crossGroup :" << window.crossGroup << endl;
	os << "Color 1 density :" << window.color1Density << endl;
	os << "Color 2 density :" << window.color2Density << endl;
	for(size_t i = 0, n = window.groups.size(); i < n; ++i){
		cout << *(window.groups[i]);
	}
	return os;
}
